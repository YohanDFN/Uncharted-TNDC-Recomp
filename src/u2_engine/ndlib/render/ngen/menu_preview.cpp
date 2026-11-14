#include "menu_preview.h"
#include "u2_engine.h"
#include <algorithm>
#include <cstring>
#include <new>
#include <string_view>
#include <utility>

namespace {
constexpr size_t kBudget = 8 * 1024 * 1024;
uint64_t be(const uint8_t *p, size_t n) {
    uint64_t v = 0;
    for(size_t i = 0; i < n; ++i) v = (v << 8) | p[i];
    return v;
}
uint64_t le(const uint8_t *p, size_t n) {
    uint64_t v = 0;
    for(size_t i = 0; i < n; ++i) v |= uint64_t(p[i]) << (8 * i);
    return v;
}
bool utf8(std::string_view text) {
    for(size_t at = 0; at < text.size();) {
        const unsigned first = static_cast<unsigned char>(text[at++]);
        if(first < 0x80) continue;
        unsigned count = 0; uint32_t value = 0, minimum = 0;
        if(first >= 0xC2 && first <= 0xDF) { count = 1; value = first & 31; minimum = 0x80; }
        else if(first >= 0xE0 && first <= 0xEF) { count = 2; value = first & 15; minimum = 0x800; }
        else if(first >= 0xF0 && first <= 0xF4) { count = 3; value = first & 7; minimum = 0x10000; }
        else return false;
        if(count > text.size() - at) return false;
        for(unsigned i = 0; i < count; ++i) {
            const unsigned c = static_cast<unsigned char>(text[at++]);
            if((c & 0xC0) != 0x80) return false;
            value = (value << 6) | (c & 63);
        }
        if(value < minimum || value > 0x10FFFF || (value >= 0xD800 && value <= 0xDFFF)) return false;
    }
    return true;
}
bool cstring(const uint8_t *data, size_t size, uint64_t offset, size_t minimum,
             size_t limit, std::string &out) {
    if(offset < minimum || offset >= size) return false;
    const size_t start = size_t(offset), available = std::min(size - start, limit + 1);
    const auto *end = static_cast<const uint8_t *>(std::memchr(data + start, 0, available));
    if(!end) return false;
    const std::string_view text(reinterpret_cast<const char *>(data + start), size_t(end - (data + start)));
    if(!utf8(text)) return false;
    out.assign(text); return true;
}
bool resolve(const std::map<uint32_t, std::string> &table, uint32_t id, std::string &out) {
    const auto it = table.find(id);
    if(it == table.end()) return false;
    out = it->second; return true;
}
struct File {
    int fd;
    explicit File(const char *path) : fd(int(runtime_file_open(path, 0, 0))) {}
    ~File() { if(fd >= 0) runtime_file_close(fd); }
    File(const File &) = delete;
    File &operator=(const File &) = delete;
    bool read(void *out, size_t bytes, uint64_t offset) const {
        return runtime_file_pread(fd, out, bytes, int64_t(offset)) == int64_t(bytes);
    }
};
}

bool u2_native_ReadLocalizationTable(std::map<uint32_t, std::string> *out,
                                     const uint8_t *data, size_t size) {
    if(!out || !data || size < 4 || size > kBudget) return false;
    const uint32_t count = uint32_t(be(data, 4));
    if(count == 0 || count > 65536 || 4 + size_t(count) * 8 > size) return false;
    const size_t pool = 4 + size_t(count) * 8;
    try {
        std::map<uint32_t, std::string> result;
        for(size_t i = 0; i < count; ++i) {
            const uint8_t *record = data + 4 + i * 8;
            const uint32_t id = uint32_t(be(record, 4)), rel = uint32_t(be(record + 4, 4));
            std::string text;
            if(rel >= size - pool || !cstring(data, size, pool + rel, pool, 4096, text) ||
               !result.emplace(id, std::move(text)).second) return false;
        }
        *out = std::move(result); return true;
    } catch(const std::bad_alloc &) { return false; }
}

bool u2_native_ReadPostStartMenuPreview(U2NativePostStartMenuPreview *out,
                                      const uint8_t *menu_bin, size_t menu_size,
                                      const uint8_t *common_bin, size_t common_size) {
    if(!out) return false;
    std::map<uint32_t, std::string> table;
    if(!u2_native_ReadLocalizationTable(&table, common_bin, common_size)) return false;
    if(!menu_bin || menu_size < 0x80 || menu_size > kBudget) return false;
    const uint64_t count = le(menu_bin + 0x50, 8), offset = le(menu_bin + 0x58, 8);
    if(count == 0 || count > 4096 || offset < 0x60 || offset >= menu_size ||
       count > (menu_size - offset) / 0x70) return false;
    try {
        U2NativePostStartMenuPreview result;
        if(!resolve(table, 0x68730362u, result.select_label) ||
           !resolve(table, 0xD9791B19u, result.back_label) ||
           !resolve(table, 0xFD8B6749u, result.title_menu_label) ||
           !resolve(table, 0xA947CC78u, result.play_label) ||
           !resolve(table, 0x9D39D946u, result.continue_label)) return false;
        for(size_t i = 0; i < count; ++i) {
            const uint8_t *p = menu_bin + size_t(offset) + i * 0x70;
            const uint32_t id = uint32_t(le(p + 8, 4));
            U2NativeMenuPagePreview *page = nullptr; const char *name = nullptr;
            if(id == 0x37656388u) { page = &result.root_main; name = "PAGE_ROOT_MAIN"; }
            else if(id == 0x82D5C00Bu) { page = &result.profile; name = "PAGE_PROFILE"; }
            else if(id == 0x11FEFB39u) { page = &result.new_profile; name = "PAGE_NEWPROFILE"; }
            else continue;
            if(page->id != 0) return false;
            page->id = id;
            if(!cstring(menu_bin, menu_size, le(p, 8), 0x60, 256, page->name) || page->name != name) return false;
            const uint64_t title_name = le(p + 0x10, 8);
            if(title_name && !cstring(menu_bin, menu_size, title_name, 0x60, 256, page->title_string_name)) return false;
            page->title_string_id = uint32_t(le(p + 0x18, 4));
            if(page->title_string_id && !resolve(table, page->title_string_id, page->title)) return false;
            const uint64_t header = le(p + 0x38, 8);
            if(header < 0x60 || header > menu_size - 16) return false;
            const uint64_t controls = le(menu_bin + size_t(header), 8);
            const uint64_t start = le(menu_bin + size_t(header) + 8, 8);
            if(controls > 256 || start < 0x60 || start > menu_size || controls > (menu_size - start) / 0x70) return false;
            page->controls.reserve(size_t(controls));
            for(size_t j = 0; j < controls; ++j) {
                const uint8_t *c = menu_bin + size_t(start) + j * 0x70;
                U2NativeMenuControlPreview control;
                const uint64_t control_name = le(c, 8), string_name = le(c + 0x20, 8);
                if((control_name && !cstring(menu_bin, menu_size, control_name, 0x60, 256, control.name)) ||
                   (string_name && !cstring(menu_bin, menu_size, string_name, 0x60, 256, control.string_name))) return false;
                control.id = uint32_t(le(c + 8, 4));
                control.type = uint32_t(le(c + 0x10, 4));
                control.subtype = uint32_t(le(c + 0x14, 4));
                control.string_id = uint32_t(le(c + 0x28, 4));
                control.datasource_type = uint32_t(le(c + 0x30, 4));
                control.datasource_index = uint32_t(le(c + 0x34, 4));
                if(control.string_id) {
                    if(!resolve(table, control.string_id, control.label)) return false;
                } else if(control.id == 0x161522F1u && control.datasource_type == 16 && control.datasource_index == 0) {
                    // ELF 0x005ACAB0 chooses MENU_CONTINUE or MENU_PLAY; no save state is inferred here.
                    control.label = result.continue_label + " / " + result.play_label;
                }
                page->controls.push_back(std::move(control));
            }
        }
        if(!result.root_main.id || !result.profile.id || !result.new_profile.id) return false;
        *out = std::move(result); return true;
    } catch(const std::bad_alloc &) { return false; }
}

bool u2_native_LoadPostStartMenuPreview(U2NativePostStartMenuPreview *out) {
    if(!out) return false;
    try {
        File menu("/app0/u2data/build/main/dc1/ps4/menu.bin");
        if(menu.fd < 0) return false;
        const int64_t menu_size = runtime_file_lseek(menu.fd, 0, 2);
        if(menu_size <= 0 || uint64_t(menu_size) > kBudget) return false;
        std::vector<uint8_t> menu_bytes(size_t(menu_size), uint8_t{0});
        if(!menu.read(menu_bytes.data(), menu_bytes.size(), 0)) return false;
        File archive("/app0/u2data/build/main/core.psarc");
        if(archive.fd < 0) return false;
        const int64_t file_size = runtime_file_lseek(archive.fd, 0, 2);
        uint8_t header[32];
        if(file_size < 32 || !archive.read(header, sizeof(header), 0) ||
           std::memcmp(header, "PSAR", 4) || std::memcmp(header + 8, "zlib", 4)) return false;
        const uint32_t toc_len = uint32_t(be(header + 12, 4)), entry_sz = uint32_t(be(header + 16, 4));
        const uint32_t entries = uint32_t(be(header + 20, 4)), block_sz = uint32_t(be(header + 24, 4));
        if(toc_len < 32 || toc_len > 16 * 1024 * 1024 || entry_sz != 30 || !entries ||
           block_sz != 65536 || uint64_t(entries) * 30 + 32 > toc_len || toc_len > uint64_t(file_size)) return false;
        std::vector<uint8_t> toc(toc_len - 32);
        if(!archive.read(toc.data(), toc.size(), 32)) return false;
        constexpr uint8_t digest[16] = {0x06,0xcf,0x8f,0x04,0x9a,0x0b,0xb7,0xd2,0x9d,0xd1,0xcf,0xe8,0xfa,0x4e,0xf9,0x00};
        const uint8_t *entry = nullptr;
        for(size_t i = 0; i < entries; ++i) {
            const uint8_t *candidate = toc.data() + i * 30;
            if(std::memcmp(candidate, digest, 16) == 0) {
                if(entry) return false;
                entry = candidate;
            }
        }
        if(!entry) return false;
        const uint64_t block_idx = be(entry + 16, 4), bytes = be(entry + 20, 5), offset = be(entry + 25, 5);
        if(!bytes || bytes > kBudget || offset < toc_len || offset > uint64_t(file_size) || bytes > uint64_t(file_size) - offset) return false;
        const size_t block_table = size_t(entries) * 30;
        const uint64_t total_blocks = (toc.size() - block_table) / 2;
        const uint64_t blocks = (bytes + block_sz - 1) / block_sz;
        if(block_idx > total_blocks || blocks > total_blocks - block_idx) return false;
        for(uint64_t k = 0; k < blocks; ++k) {
            const uint64_t rem = std::min(uint64_t(block_sz), bytes - k * block_sz);
            const uint64_t stored = be(toc.data() + block_table + size_t(block_idx + k) * 2, 2);
            if(stored != 0 && (rem == block_sz || stored != rem)) return false;
        }
        std::vector<uint8_t> common(size_t(bytes), uint8_t{0});
        if(!archive.read(common.data(), common.size(), offset)) return false;
        return u2_native_ReadPostStartMenuPreview(out, menu_bytes.data(), menu_bytes.size(), common.data(), common.size());
    } catch(const std::bad_alloc &) { return false; }
}
