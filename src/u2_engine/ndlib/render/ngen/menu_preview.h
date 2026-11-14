#pragma once
#include <cstddef>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

struct U2NativeMenuControlPreview {
    uint32_t id{0}, type{0}, subtype{0}, string_id{0}, datasource_type{0}, datasource_index{0};
    std::string name, string_name, label;
    uint32_t target_page_id{0};
    std::string target_page_name;
    bool is_selectable() const { return type == 2 && !is_spacer() && !label.empty(); }
    bool is_spacer() const {
        return id == 0 && type == 1 && subtype == 0 && string_id == 0 &&
               datasource_type == 0 && datasource_index == 0;
    }
};
struct U2NativeMenuPagePreview {
    uint32_t id{0}, title_string_id{0};
    uint32_t subtitle_string_id{0}, page_flags{0};
    int32_t layout_x{0}, layout_y{0};
    std::string name, title_string_name, title;
    std::string subtitle_string_name, subtitle;
    std::vector<U2NativeMenuControlPreview> controls;
    bool hides_standard_prompts() const { return (page_flags & 0x1u) != 0u; }
    bool shows_title_logo() const { return id == 0x37656388u; }
    bool draws_page_box() const { return !shows_title_logo() && (page_flags & 0x81000u) == 0u; }
};
struct U2NativeMenuRect {
    float x{0}, y{0}, w{0}, h{0};
};
struct U2NativeMenuPageLayout {
    float virtual_width{1280.0f}, virtual_height{720.0f};
    bool show_logo{false}, show_box{false}, has_title{false}, show_prompts{false};
    U2NativeMenuRect logo_rect, page_box_rect, title_rect, content_rect;
    float row_height{0}, row_step{0}, font_height{0}, selected_font_height{0};
    float shadow_offset{0}, prompts_y{0}, prompts_right_x{0}, copyright_y{0};
    float box_fade_top{24.0f}, box_fade_bottom{40.0f}, box_alpha{0.65f};
    float title_scale{1.25f}, copyright_bottom_y{642.0f}, copyright_scale{0.65f}, control_shadow_offset{4.0f};
};
// Host layout only: verified virtual frame/anchors, viewer text sizing and note reserve.
bool u2_native_ComputeMenuPageLayout(const U2NativeMenuPagePreview &page, U2NativeMenuPageLayout *out);
struct U2NativePostStartMenuPreview {
    U2NativeMenuPagePreview root_main, profile, new_profile;
    U2NativeMenuPagePreview ingame_options, bonuses, chapters, multimedals;
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

struct U2NativeSavedataStatus {
    bool has_profile{false};
    bool has_campaign_save{false};
    uint32_t profile_slot_sum{0};
    uint32_t campaign_slot_sum{0};
};
// Invalid buffers preserve out_sum; valid inactive slots report a zero sum.
bool u2_native_HasActiveSavedataSlot(const uint8_t *data, size_t size, uint32_t *out_sum = nullptr);
bool u2_native_DetectSavedataStatus(U2NativeSavedataStatus *out);

enum class U2NativeMenuTransitionState : uint32_t { Idle = 0, Push = 7, Pop = 8, Reset = 12 };
enum class U2NativeMenuActionKind : uint32_t {
    None = 0, TransitionPush, TransitionPop, TransitionReset,
    DeferredNewGame, DeferredLoadGame, DeferredContinueOrPlay, DeferredGameplayOptions,
};
struct U2NativeMenuStackEntry {
    const U2NativeMenuPagePreview *page{nullptr};
    uint32_t page_id{0};
    size_t cursor{0};
};
struct U2NativeMenuTransitionResult {
    bool handled{false};
    U2NativeMenuActionKind action{U2NativeMenuActionKind::None};
    uint32_t from_page_id{0};
    uint32_t to_page_id{0};
    std::string status_note;
};
// menu must outlive this host state machine and remain at the same address.
class U2NativeMenuStateMachine {
public:
    bool init(const U2NativePostStartMenuPreview *menu, const U2NativeSavedataStatus &save);
    const U2NativeMenuPagePreview *find_page(uint32_t page_id) const;
    const U2NativeMenuPagePreview *current_page() const;
    size_t stack_depth() const { return m_stack.size(); }
    bool can_go_back() const { return m_stack.size() > 1; }
    const std::vector<U2NativeMenuStackEntry> &stack() const { return m_stack; }
    size_t current_cursor() const { return m_stack.empty() ? 0 : m_stack.back().cursor; }
    U2NativeMenuTransitionState transition_state() const { return m_transition_state; }
    const U2NativeSavedataStatus &savedata_status() const { return m_save; }
    const std::string &last_status_note() const { return m_last_status_note; }
    const std::string &status_message() const { return m_last_status_note; }
    std::string display_control_label(const U2NativeMenuControlPreview &ctrl) const;
    bool set_cursor(size_t index);
    bool move_cursor(int delta);
    const U2NativeMenuControlPreview *selected_control() const;
    U2NativeMenuTransitionResult transition_to_page(uint32_t target_page_id, bool reset_stack);
    U2NativeMenuTransitionResult press_select();
    U2NativeMenuTransitionResult press_back();
    U2NativeMenuTransitionResult reset_to_root();
private:
    const U2NativePostStartMenuPreview *m_menu{nullptr};
    U2NativeSavedataStatus m_save;
    std::vector<U2NativeMenuStackEntry> m_stack;
    U2NativeMenuTransitionState m_transition_state{U2NativeMenuTransitionState::Idle};
    std::string m_last_status_note;
};
