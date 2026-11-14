#include "menu_preview.h"
#include "u2_engine.h"
#include <algorithm>
#include <cstring>
#include <cstdio>
#include <new>
#include <string_view>
#include <utility>

bool u2_native_ComputeMenuPageLayout(const U2NativeMenuPagePreview &page, U2NativeMenuPageLayout *out) {
    if(!out) return false;
    U2NativeMenuPageLayout result;
    result.show_logo = page.shows_title_logo();
    result.show_box = page.draws_page_box();
    result.has_title = !result.show_logo && !page.title.empty();
    result.show_prompts = !page.hides_standard_prompts();
    // MenuHost::Draw and MenuDrawer::BuildIcon use this virtual frame and logo rect.
    result.logo_rect = {128.0f, 25.0f, 1024.0f, 256.0f};
    const float rows = float(std::max(size_t{1}, page.controls.size()));
    result.row_step = std::min(32.0f, (result.show_logo ? 160.0f : 360.0f) / rows);
    result.row_height = result.row_step;
    result.font_height = std::min(20.0f, result.row_step * 0.75f);
    result.selected_font_height = std::min(21.0f, result.row_step * 0.8f);
    result.shadow_offset = 3.0f;
    result.copyright_bottom_y = 642.0f;
    const float controls_height = rows * result.row_step;
    if(result.show_logo) {
        result.content_rect = {80.0f, std::min(469.5f, 649.5f - controls_height * 0.5f) - controls_height * 0.5f,
                               1120.0f, controls_height};
        result.page_box_rect = result.content_rect;
        result.copyright_y = 560.0f;
    } else {
        // Font metrics and runtime status aren't in page data: fit text in the viewer,
        // reserving two footer lines so dynamic-list/status notes cannot cover controls.
        size_t longest = page.title.size();
        for(const auto &ctrl : page.controls) longest = std::max(longest, ctrl.label.size());
        const float width = std::clamp(40.0f + float(longest) * 12.0f, 580.0f, 680.0f);
        const float title_height = result.has_title ? 50.0f : 0.0f;
        const float height = 40.0f + title_height + controls_height + 64.0f;
        result.page_box_rect = {(1280.0f - width) * 0.5f, (720.0f - height) * 0.5f, width, height};
        const auto &box = result.page_box_rect;
        result.title_rect = {box.x + 20.0f, box.y + 20.0f, width - 40.0f, title_height};
        result.content_rect = {box.x + 20.0f, box.y + 20.0f + title_height, width - 40.0f, controls_height};
    }
    result.prompts_y = result.page_box_rect.y + result.page_box_rect.h + 15.0f;
    result.prompts_right_x = result.page_box_rect.x + result.page_box_rect.w;
    *out = result;
    return true;
}

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
            else if(id == 0xB793B4B2u) { page = &result.ingame_options; name = "PAGE_INGAME_OPTIONS"; }
            else if(id == 0x22D667C0u) { page = &result.bonuses; name = "PAGE_BONUSES"; }
            else if(id == 0xBE2C8B4Bu) { page = &result.chapters; name = "PAGE_CHAPTERS"; }
            else if(id == 0xACE1A9D5u) { page = &result.multimedals; name = "PAGE_MULTIMEDALS"; }
            else continue;
            if(page->id != 0) return false;
            page->id = id;
            if(!cstring(menu_bin, menu_size, le(p, 8), 0x60, 256, page->name) || page->name != name) return false;
            const uint64_t title_name = le(p + 0x10, 8);
            if(title_name && !cstring(menu_bin, menu_size, title_name, 0x60, 256, page->title_string_name)) return false;
            page->title_string_id = uint32_t(le(p + 0x18, 4));
            if(page->title_string_id && !resolve(table, page->title_string_id, page->title)) return false;
            const uint64_t subtitle_name = le(p + 0x20, 8);
            if(subtitle_name && !cstring(menu_bin, menu_size, subtitle_name, 0x60, 256, page->subtitle_string_name)) return false;
            page->subtitle_string_id = uint32_t(le(p + 0x28, 4));
            if(page->subtitle_string_id && !resolve(table, page->subtitle_string_id, page->subtitle)) return false;
            page->page_flags = uint32_t(le(p + 0x34, 4));
            page->layout_x = int32_t(uint32_t(le(p + 0x40, 4)));
            page->layout_y = int32_t(uint32_t(le(p + 0x44, 4)));
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
                const uint64_t target_name = le(c + 0x48, 8);
                if(target_name && !cstring(menu_bin, menu_size, target_name, 0x60, 256, control.target_page_name)) return false;
                control.target_page_id = uint32_t(le(c + 0x50, 4));
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
        U2NativePostStartMenuPreview result;
        if(!u2_native_ReadPostStartMenuPreview(&result, menu_bytes.data(), menu_bytes.size(), common.data(), common.size()) ||
           result.ingame_options.id != 0xB793B4B2u || result.ingame_options.controls.empty() ||
           result.bonuses.id != 0x22D667C0u || result.bonuses.controls.empty() ||
           result.chapters.id != 0xBE2C8B4Bu || result.chapters.controls.empty() ||
           result.multimedals.id != 0xACE1A9D5u || result.multimedals.controls.empty()) return false;
        *out = std::move(result);return true;
    } catch(const std::bad_alloc &) { return false; }
}

bool u2_native_HasActiveSavedataSlot(const uint8_t *data, size_t size, uint32_t *out_sum) {
    if(!data || size < 0xD7u) return false;
    uint32_t sum = 0;
    for(size_t i = 0xCD; i <= 0xD6; ++i) sum += data[i];
    if(out_sum) *out_sum = sum;
    return sum != 0;
}

bool u2_native_DetectSavedataStatus(U2NativeSavedataStatus *out) {
    if(!out) return false;
    U2NativeSavedataStatus result;
    auto read_slot = [](const char *path, const char *fallback, uint32_t &sum) {
        // Host files only; read a bounded prefix, never deserialize guest objects.
        std::FILE *file = std::fopen(path, "rb");
        if(!file) file = std::fopen(fallback, "rb");
        if(!file) return false;
        uint8_t bytes[0xD7]{};
        const size_t count = std::fread(bytes, 1, sizeof(bytes), file);
        const bool valid = count == sizeof(bytes) && !std::ferror(file);
        std::fclose(file);
        return valid && u2_native_HasActiveSavedataSlot(bytes, count, &sum);
    };
    result.has_profile = read_slot("user/savedata/1/CUSA02343/SAVEDATA02P/USR-DATA",
        "../user/savedata/1/CUSA02343/SAVEDATA02P/USR-DATA", result.profile_slot_sum);
    result.has_campaign_save = read_slot("user/savedata/1/CUSA02343/SAVEDATA020/USR-DATA",
        "../user/savedata/1/CUSA02343/SAVEDATA020/USR-DATA", result.campaign_slot_sum);
    *out = result;return true;
}

namespace {
size_t first_selectable(const U2NativeMenuPagePreview &page) {
    for(size_t i = 0; i < page.controls.size(); ++i) if(page.controls[i].is_selectable()) return i;
    return 0;
}
}

bool U2NativeMenuStateMachine::init(const U2NativePostStartMenuPreview *menu, const U2NativeSavedataStatus &save) {
    if(!menu || !menu->root_main.id) return false;
    std::vector<U2NativeMenuStackEntry> initial{{&menu->root_main, menu->root_main.id, first_selectable(menu->root_main)}};
    m_stack = std::move(initial);m_menu = menu;m_save = save;
    m_transition_state = U2NativeMenuTransitionState::Reset;m_last_status_note.clear();return true;
}
const U2NativeMenuPagePreview *U2NativeMenuStateMachine::find_page(uint32_t id) const {
    if(!m_menu || !id) return nullptr;
    for(const auto *page : {&m_menu->root_main, &m_menu->profile, &m_menu->new_profile,
                           &m_menu->ingame_options, &m_menu->bonuses, &m_menu->chapters, &m_menu->multimedals}) {
        if(page->id == id) return page;
    }
    return nullptr;
}
const U2NativeMenuPagePreview *U2NativeMenuStateMachine::current_page() const {
    return m_stack.empty() ? nullptr : m_stack.back().page;
}
std::string U2NativeMenuStateMachine::display_control_label(const U2NativeMenuControlPreview &ctrl) const {
    if(m_menu && ctrl.id == 0x161522F1u) {
        const auto &label = m_save.has_campaign_save ? m_menu->continue_label : m_menu->play_label;
        if(!label.empty()) return label;
    }
    return ctrl.label;
}
bool U2NativeMenuStateMachine::set_cursor(size_t index) {
    const auto *page = current_page();
    if(!page || index >= page->controls.size() || !page->controls[index].is_selectable()) return false;
    m_stack.back().cursor = index;return true;
}
bool U2NativeMenuStateMachine::move_cursor(int delta) {
    const auto *page = current_page();
    if(!page || delta == 0) return false;
    size_t count = 0;
    for(const auto &ctrl : page->controls) if(ctrl.is_selectable()) ++count;
    if(!count) return false;
    // Widen before negation so INT_MIN remains defined. Reduce full revolutions.
    const int64_t wide = delta;
    const size_t steps = size_t(wide < 0 ? -wide : wide) % count;
    size_t cursor = current_cursor();
    for(size_t step = 0; step < steps; ++step) {
        do {
            cursor = delta > 0 ? (cursor + 1) % page->controls.size() :
                (cursor == 0 ? page->controls.size() - 1 : cursor - 1);
        } while(!page->controls[cursor].is_selectable());
    }
    return set_cursor(cursor);
}
const U2NativeMenuControlPreview *U2NativeMenuStateMachine::selected_control() const {
    const auto *page = current_page();
    return page && current_cursor() < page->controls.size() && page->controls[current_cursor()].is_selectable()
        ? &page->controls[current_cursor()] : nullptr;
}
U2NativeMenuTransitionResult U2NativeMenuStateMachine::transition_to_page(uint32_t id, bool reset_stack) {
    const uint32_t from = current_page() ? current_page()->id : 0;
    const auto *target = find_page(id);
    if(!target) return {false, U2NativeMenuActionKind::None, from, from, ""};
    U2NativeMenuActionKind action;
    if(reset_stack) {
        std::vector<U2NativeMenuStackEntry> initial{{target, id, first_selectable(*target)}};
        m_stack = std::move(initial);
        m_transition_state = U2NativeMenuTransitionState::Reset;action = U2NativeMenuActionKind::TransitionReset;
    } else {
        const auto found = std::find_if(m_stack.begin(), m_stack.end(), [id](const auto &entry) { return entry.page_id == id; });
        if(found != m_stack.end()) {
            m_stack.erase(found + 1, m_stack.end());
            m_transition_state = U2NativeMenuTransitionState::Pop;action = U2NativeMenuActionKind::TransitionPop;
        } else {
            m_stack.push_back({target, id, first_selectable(*target)});
            m_transition_state = U2NativeMenuTransitionState::Push;action = U2NativeMenuActionKind::TransitionPush;
        }
    }
    m_last_status_note.clear();return {true, action, from, id, ""};
}
U2NativeMenuTransitionResult U2NativeMenuStateMachine::press_select() {
    const uint32_t id = current_page() ? current_page()->id : 0;
    const auto *ctrl = selected_control();
    if(!ctrl) return {false, U2NativeMenuActionKind::None, id, id, ""};
    U2NativeMenuTransitionResult result{true, U2NativeMenuActionKind::None, id, id, ""};
    if(ctrl->target_page_id) {
        if(find_page(ctrl->target_page_id)) return transition_to_page(ctrl->target_page_id, false);
        result.status_note = "Sous-menu non porte : " + (ctrl->target_page_name.empty() ? std::to_string(ctrl->target_page_id) : ctrl->target_page_name);
    } else {
        switch(ctrl->id) {
        case 0x2BBBB5C5u: return transition_to_page(m_save.has_profile ? 0x82D5C00Bu : 0x11FEFB39u, false);
        case 0xFA872AA5u:
            if(find_page(0xB793B4B2u)) return transition_to_page(0xB793B4B2u, false);
            [[fallthrough]];
        case 0xC79A65B5u:
            result.action = U2NativeMenuActionKind::DeferredGameplayOptions;
            result.status_note = "Action differee : Options de jeu";break;
        case 0xB861A261u:
            result.action = U2NativeMenuActionKind::DeferredNewGame;
            result.status_note = "Action differee : Nouvelle partie (chargement niveau)";break;
        case 0xF7A5B490u:
            result.action = U2NativeMenuActionKind::DeferredLoadGame;
            result.status_note = "Action differee : Charger une partie";break;
        case 0x161522F1u:
            result.action = U2NativeMenuActionKind::DeferredContinueOrPlay;
            result.status_note = "Action differee : Continuer / Jouer";break;
        case 0x131BA09Bu:
            result.action = U2NativeMenuActionKind::DeferredContinueOrPlay;
            result.status_note = "Action differee : Commencer chapitre";break;
        default: result.handled = false;break;
        }
    }
    m_last_status_note = result.status_note;return result;
}
U2NativeMenuTransitionResult U2NativeMenuStateMachine::press_back() {
    const uint32_t from = current_page() ? current_page()->id : 0;
    if(m_stack.size() <= 1) return {false, U2NativeMenuActionKind::None, from, from, ""};
    m_stack.pop_back();m_transition_state = U2NativeMenuTransitionState::Pop;m_last_status_note.clear();
    return {true, U2NativeMenuActionKind::TransitionPop, from, current_page()->id, ""};
}
U2NativeMenuTransitionResult U2NativeMenuStateMachine::reset_to_root() {
    return transition_to_page(m_menu ? m_menu->root_main.id : 0, true);
}
