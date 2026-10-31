/* Uncharted 2: Audio, Music, Speech & Sound Bank Cache Functions */
#include "runtime_u2_common.h"

static int64_t ABI detour_DcSyncAudioMetaData(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FD560: (sync-audio-meta-data) */
    (void)argc; (void)argv; (void)status;
    uint8_t *audio = g_u2_image_base + 0x01111750ull;
    if (*(const uint64_t *)(audio + 0x8) != 0) {
        typedef void (ABI *fn_t)(void *);
        fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x003B7F50ull);
        fn(audio);
    }
    return 0;
}

static int64_t ABI detour_DcUseAudioRainLevel(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A009F0: (use-audio-rain-level level) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc < 1 || !argv) return 0;
    uint32_t level = (uint32_t)argv[0];
    uint8_t *audio_mgr = g_u2_image_base + 0x01111750ull;
    *(uint32_t *)(audio_mgr + 0x594) = level;
    return 1;
}

static int64_t ABI detour_DcSuppressIdleSpeech(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A01360: (suppress-idle-speech duration-sec) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc < 1 || !argv) return 0;
    float dur = u2_reg_to_f32(argv[0]);
    float scale = *(const float *)(g_u2_image_base + 0x00E672F4ull);
    int64_t frames = (int64_t)(dur * scale);
    if (frames < 1) frames = 1;
    int64_t *p_cur = (int64_t *)(g_u2_image_base + 0x0147E5C8ull);
    if (frames > *p_cur) *p_cur = frames;
    return 0;
}

static int64_t ABI detour_DcCacheSoundBank(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FD540: (cache-sound-bank bank-name) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc < 1 || !argv) return 0;
    const void *bank_name = (const void *)(uintptr_t)argv[0];
    if (!bank_name) return 0;
    typedef void (ABI *CacheFn)(void *, const void *, int32_t, int32_t);
    CacheFn fn = (CacheFn)(uintptr_t)(g_u2_image_base + 0x003C7FA0ull);
    void *audio_mgr = (void *)(g_u2_image_base + 0x01111750ull);
    if (fn) fn(audio_mgr, bank_name, 2, 0);
    return 0;
}

static int64_t ABI detour_DcCacheSoundStream(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FD510: (cache-sound-stream stream-name) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc < 1 || !argv) return 0;
    const void *stream_name = (const void *)(uintptr_t)argv[0];
    if (!stream_name) return 0;
    typedef void (ABI *CacheFn)(void *, const void *, int32_t, int32_t);
    CacheFn fn = (CacheFn)(uintptr_t)(g_u2_image_base + 0x003C7FA0ull);
    void *audio_mgr = (void *)(g_u2_image_base + 0x01111750ull);
    if (fn) fn(audio_mgr, stream_name, 3, 0);
    return 0;
}

static int64_t ABI detour_DcCacheSpeechStream(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FD4E0: (cache-speech-stream speech-name) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc < 1 || !argv) return 0;
    const void *speech_name = (const void *)(uintptr_t)argv[0];
    if (!speech_name) return 0;
    typedef void (ABI *CacheFn)(void *, const void *, int32_t, int32_t);
    CacheFn fn = (CacheFn)(uintptr_t)(g_u2_image_base + 0x003C7FA0ull);
    void *audio_mgr = (void *)(g_u2_image_base + 0x01111750ull);
    if (fn) fn(audio_mgr, speech_name, 5, 0);
    return 0;
}

static int64_t ABI detour_DcCacheMusicPlaylist(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FD460: (cache-music-playlist playlist-name) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc < 1 || !argv) return 0;
    uint32_t sym = (uint32_t)argv[0];
    if (sym == 0) return 0;
    typedef void * (ABI *EvalFn)(uint32_t);
    EvalFn eval_sym = (EvalFn)(uintptr_t)(g_u2_image_base + 0x00A15300ull);
    const uint8_t *obj = eval_sym ? (const uint8_t *)eval_sym(sym) : NULL;
    if (!obj) return 0;
    const uint8_t *pl = *(const uint8_t **)obj;
    if (!pl) return 0;
    int32_t count = *(const int32_t *)(pl + 0xA8);
    if (count <= 0) return 0;
    const uint8_t *items = *(const uint8_t **)(pl + 0xB0);
    if (!items) return 0;
    typedef void (ABI *CacheFn)(void *, const void *, int32_t, int32_t);
    CacheFn fn = (CacheFn)(uintptr_t)(g_u2_image_base + 0x003C7FA0ull);
    void *audio_mgr = (void *)(g_u2_image_base + 0x01111750ull);
    if (!fn) return 0;
    for (int32_t i = 0; i < count; ++i) {
        const void *name = *(const void **)(items + (size_t)i * 16);
        if (name) fn(audio_mgr, name, 4, 0);
    }
    return 0;
}

/* Table of detours provided by this module */
static const U2NativeDetour s_audio_detours[] = {
    { "DcSyncAudioMetaData", 0x009FD560, (uintptr_t)detour_DcSyncAudioMetaData },
    { "DcUseAudioRainLevel", 0x00A009F0, (uintptr_t)detour_DcUseAudioRainLevel },
    { "DcSuppressIdleSpeech", 0x00A01360, (uintptr_t)detour_DcSuppressIdleSpeech },
    { "DcCacheSoundBank", 0x009FD540, (uintptr_t)detour_DcCacheSoundBank },
    { "DcCacheSoundStream", 0x009FD510, (uintptr_t)detour_DcCacheSoundStream },
    { "DcCacheSpeechStream", 0x009FD4E0, (uintptr_t)detour_DcCacheSpeechStream },
    { "DcCacheMusicPlaylist", 0x009FD460, (uintptr_t)detour_DcCacheMusicPlaylist },
};

size_t u2_get_audio_detours(const U2NativeDetour **out) {
    if (out) *out = s_audio_detours;
    return sizeof(s_audio_detours) / sizeof(s_audio_detours[0]);
}
