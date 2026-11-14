#pragma once
#include <cstddef>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

struct U2NativeMenuControlPreview {
    uint32_t id{0}, type{0}, subtype{0}, string_id{0}, datasource_type{0}, datasource_index{0};
    std::string name, string_name, label;
    bool is_spacer() const {
        return id == 0 && type == 1 && subtype == 0 && string_id == 0 &&
               datasource_type == 0 && datasource_index == 0;
    }
};
struct U2NativeMenuPagePreview {
    uint32_t id{0}, title_string_id{0};
    std::string name, title_string_name, title;
    std::vector<U2NativeMenuControlPreview> controls;
};
struct U2NativePostStartMenuPreview {
    U2NativeMenuPagePreview root_main, profile, new_profile;
    std::string select_label, back_label, title_menu_label, play_label, continue_label;
};

// Verified menu.bin and big-endian .common records; host-owned results only.
// Every false return preserves *out. No guest code or guest objects are used.
bool u2_native_ReadLocalizationTable(std::map<uint32_t, std::string> *out,
                                     const uint8_t *data, size_t size);
bool u2_native_ReadPostStartMenuPreview(U2NativePostStartMenuPreview *out,
                                      const uint8_t *menu_bin, size_t menu_size,
                                      const uint8_t *common_bin, size_t common_size);
bool u2_native_LoadPostStartMenuPreview(U2NativePostStartMenuPreview *out);
