/* Uncharted 2: Audio, Music, Speech & Sound Bank Cache Functions */
#include "runtime_u2_common.h"

static int64_t ABI detour_DcSyncAudioMetaData(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
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
    u2_set_status(status, 0);
    if (!g_u2_image_base || argc < 1 || !argv) return 0;
    uint32_t level = (uint32_t)argv[0];
    uint8_t *audio_mgr = g_u2_image_base + 0x01111750ull;
    *(uint32_t *)(audio_mgr + 0x594) = level;
    return 1;
}

static int64_t ABI detour_DcSuppressIdleSpeech(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A01360: (suppress-idle-speech duration-sec) */
    u2_set_status(status, 0);
    if (!g_u2_image_base || argc < 1 || !argv) return 0;
    float dur = u2_reg_to_f32(argv[0]);
    float scale = *(const float *)(g_u2_image_base + 0x00E672F4ull);
    int64_t frames = (int64_t)(dur * scale);
    if (frames < 1) frames = 1;
    int64_t *p_cur = (int64_t *)(g_u2_image_base + 0x0147E5C8ull);
    if (frames > *p_cur) *p_cur = frames;
    return 0;
}

static int64_t ABI detour_DcSay(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A01A80: (say speaker line-id) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    typedef int64_t (ABI *SayFn)(int32_t argc, uint64_t *argv, int32_t channel);
    SayFn fn = (SayFn)(uintptr_t)(g_u2_image_base + 0x009F3220ull);
    return fn ? fn(argc, argv, -1) : 0;
}

static int64_t ABI detour_DcCacheSoundBank(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FD540: (cache-sound-bank bank-name) */
    u2_set_status(status, 0);
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
    u2_set_status(status, 0);
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
    u2_set_status(status, 0);
    if (!g_u2_image_base || argc < 1 || !argv) return 0;
    const void *speech_name = (const void *)(uintptr_t)argv[0];
    if (!speech_name) return 0;
    typedef void (ABI *CacheFn)(void *, const void *, int32_t, int32_t);
    CacheFn fn = (CacheFn)(uintptr_t)(g_u2_image_base + 0x003C7FA0ull);
    void *audio_mgr = (void *)(g_u2_image_base + 0x01111750ull);
    if (fn) fn(audio_mgr, speech_name, 5, 0);
    return 0;
}

static int64_t ABI detour_DcSetMixerOverride(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x009FD3B0: (set-mixer-override id) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    if (argc > 0 && argv) {
        uint32_t id = (uint32_t)argv[0];
        typedef void (ABI *StringIdToStringFn)(uint32_t id, const void *unused);
        StringIdToStringFn str_fn = (StringIdToStringFn)(uintptr_t)(g_u2_image_base + 0x00CE33A0ull);
        if (str_fn) str_fn(id, NULL);
        uint8_t *audio_mgr = g_u2_image_base + 0x01111750ull;
        if (audio_mgr && audio_mgr[8] != 0) {
            *(uint32_t *)(audio_mgr + 0x4E0) = id;
        }
    }
    return 0;
}

static int64_t ABI detour_DcCacheMusicPlaylist(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FD460: (cache-music-playlist playlist-name) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcStartMovieAudio(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x009FE0D0: (start-movie-audio movie-sound-id [start-time]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    void **movie_mgr_slot = (void **)(g_u2_image_base + 0x010608E0ull);
    if (!movie_mgr_slot || !*movie_mgr_slot) return 0;
    void *movie_mgr = *movie_mgr_slot;
    uint64_t sound_id = (argc > 0 && argv) ? argv[0] : 0;
    float start_time = (argc > 1 && argv) ? (float)(int32_t)argv[1] : 0.0f;
    typedef void (ABI *StartMovieAudioFn)(void *mgr, uint64_t id, uint32_t flags, float time);
    StartMovieAudioFn fn = (StartMovieAudioFn)(uintptr_t)(g_u2_image_base + 0x00B822E0ull);
    if (fn) fn(movie_mgr, sound_id, 0, start_time);
    return 0;
}

static int64_t ABI detour_DcPlayMusicStinger(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FD3F0: (play-music-stinger stinger-sym) */
    u2_set_status(status, 0);
    if (!g_u2_image_base || argc <= 0 || !argv) return 0;
    uint32_t sym = (uint32_t)argv[0];
    uint8_t *music_mgr = g_u2_image_base + 0x01113C58ull;
    if (*(const uint8_t *)music_mgr != 0 && sym != 0xF114B39Du) {
        typedef void * (ABI *EvalFn)(uint32_t);
        EvalFn eval_sym = (EvalFn)(uintptr_t)(g_u2_image_base + 0x00A15300ull);
        void *obj = eval_sym ? eval_sym(sym) : NULL;
        if (obj && *(void **)obj) {
            typedef void (ABI *PlayStingerFn)(void *, void *);
            PlayStingerFn fn = (PlayStingerFn)(uintptr_t)(g_u2_image_base + 0x003BE420ull);
            if (fn) fn(music_mgr, *(void **)obj);
        }
    }
    return 0;
}

static int64_t ABI detour_DcStopSound(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A01160: (stop-sound sound-handle) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 0;
}

static int64_t ABI detour_DcRailOverrideSpeedForAudio(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AB9120: (rail-override-speed-for-audio [rail-entity] [speed]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 0;
}

/* Table of detours provided by this module */
static int64_t ABI detour_DcPlaySound(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A010C0: (play-sound sound-name [vol] [pitch]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    uint32_t sound_id = 0;
    if (argc > 0 && argv && argv[0]) {
        sound_id = (uint32_t)argv[0];
    } else {
        typedef uint32_t (ABI *GetSelfFn)(uint32_t, const void *, uint32_t);
        GetSelfFn get_self = (GetSelfFn)(uintptr_t)(*(uint8_t **)(g_u2_image_base + 0x010666C8ull));
        if (get_self) sound_id = get_self(0, 0, 1);
    }
    if (!sound_id) return 0;
    float vol = (argc >= 2) ? u2_reg_to_f32(argv[1]) : 1.0f;
    float pitch = (argc >= 3) ? u2_reg_to_f32(argv[2]) : 0.0f;
    typedef int32_t (ABI *PlayHelperFn)(uint32_t, void *, uint32_t, uint32_t, void *, float, float);
    PlayHelperFn helper = (PlayHelperFn)(uintptr_t)(g_u2_image_base + 0x00A00B30ull);
    return helper ? (int64_t)helper(sound_id, NULL, 0, 0, NULL, vol, pitch) : 0;
}

static int64_t ABI detour_DcPlaySoundAtJoint(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A00DA0: (play-sound-at-joint sound-name entity joint-id [vol] [pitch]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base || argc < 2 || !argv) return 0;
    uint32_t sound_id = (uint32_t)argv[0];
    uint32_t ent_id = (uint32_t)argv[1];
    uint32_t joint_id = (argc >= 3) ? (uint32_t)argv[2] : 0;
    float vol = (argc >= 4) ? u2_reg_to_f32(argv[3]) : 1.0f;
    float pitch = (argc >= 5) ? u2_reg_to_f32(argv[4]) : 0.0f;
    typedef uint8_t * (ABI *GetEntityFn)(uint64_t, uint32_t);
    GetEntityFn get_ent = (GetEntityFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
    uint8_t *ent = get_ent ? get_ent(0, ent_id) : NULL;
    typedef int32_t (ABI *PlayHelperFn)(uint32_t, void *, uint32_t, uint32_t, void *, float, float);
    PlayHelperFn helper = (PlayHelperFn)(uintptr_t)(g_u2_image_base + 0x00A00B30ull);
    return helper ? (int64_t)helper(sound_id, ent, joint_id, 0, NULL, vol, pitch) : 0;
}

static int64_t ABI detour_DcPlaySoundAtPosition(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A00A30: (play-sound-at-position sound-name pos-ptr [vol] [pitch]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base || argc < 2 || !argv || !argv[0] || !argv[1]) return 0;
    uint32_t sound_id = (uint32_t)argv[0];
    const float *pos = (const float *)(uintptr_t)argv[1];
    float vol = (argc >= 3) ? u2_reg_to_f32(argv[2]) : 1.0f;
    float pitch = (argc >= 4) ? u2_reg_to_f32(argv[3]) : 0.0f;
    float vec[4] = { pos[0], pos[1], pos[2], 1.0f };
    typedef int32_t (ABI *PlayHelperFn)(uint32_t, void *, uint32_t, uint32_t, void *, float, float);
    PlayHelperFn helper = (PlayHelperFn)(uintptr_t)(g_u2_image_base + 0x00A00B30ull);
    return helper ? (int64_t)helper(sound_id, NULL, 0, 0, vec, vol, pitch) : 0;
}

static int64_t ABI detour_DcPlaySoundAttached(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A00F30: (play-sound-attached sound-name entity [vol] [pitch]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base || argc < 2 || !argv) return 0;
    uint32_t sound_id = (uint32_t)argv[0];
    uint32_t ent_id = (uint32_t)argv[1];
    float vol = (argc >= 3) ? u2_reg_to_f32(argv[2]) : 1.0f;
    float pitch = (argc >= 4) ? u2_reg_to_f32(argv[3]) : 0.0f;
    typedef uint8_t * (ABI *GetEntityFn)(uint64_t, uint32_t);
    GetEntityFn get_ent = (GetEntityFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
    uint8_t *ent = get_ent ? get_ent(0, ent_id) : NULL;
    typedef int32_t (ABI *PlayHelperFn)(uint32_t, void *, uint32_t, uint32_t, void *, float, float);
    PlayHelperFn helper = (PlayHelperFn)(uintptr_t)(g_u2_image_base + 0x00A00B30ull);
    return helper ? (int64_t)helper(sound_id, ent, 0, 0, NULL, vol, pitch) : 0;
}

static const U2NativeDetour s_audio_detours[] = {
    { "DcSyncAudioMetaData", 0x009FD560, (uintptr_t)detour_DcSyncAudioMetaData },
    { "DcUseAudioRainLevel", 0x00A009F0, (uintptr_t)detour_DcUseAudioRainLevel },
    { "DcSuppressIdleSpeech", 0x00A01360, (uintptr_t)detour_DcSuppressIdleSpeech },
    { "DcSay", 0x00A01A80, (uintptr_t)detour_DcSay },
    { "DcCacheSoundBank", 0x009FD540, (uintptr_t)detour_DcCacheSoundBank },
    { "DcCacheSoundStream", 0x009FD510, (uintptr_t)detour_DcCacheSoundStream },
    { "DcCacheSpeechStream", 0x009FD4E0, (uintptr_t)detour_DcCacheSpeechStream },
    { "DcSetMixerOverride", 0x009FD3B0, (uintptr_t)detour_DcSetMixerOverride },
    { "DcCacheMusicPlaylist", 0x009FD460, (uintptr_t)detour_DcCacheMusicPlaylist },
    { "DcStartMovieAudio", 0x009FE0D0, (uintptr_t)detour_DcStartMovieAudio },
    { "DcPlayMusicStinger", 0x009FD3F0, (uintptr_t)detour_DcPlayMusicStinger },
    { "DcStopSound", 0x00A01160, (uintptr_t)detour_DcStopSound },
    { "DcRailOverrideSpeedForAudio", 0x00AB9120, (uintptr_t)detour_DcRailOverrideSpeedForAudio },
    { "DcPlaySound", 0x00A010C0, (uintptr_t)detour_DcPlaySound },
    { "DcPlaySoundAtJoint", 0x00A00DA0, (uintptr_t)detour_DcPlaySoundAtJoint },
    { "DcPlaySoundAtPosition", 0x00A00A30, (uintptr_t)detour_DcPlaySoundAtPosition },
    { "DcPlaySoundAttached", 0x00A00F30, (uintptr_t)detour_DcPlaySoundAttached },
};

size_t u2_get_audio_detours(const U2NativeDetour **out) {
    if (out) *out = s_audio_detours;
    return sizeof(s_audio_detours) / sizeof(s_audio_detours[0]);
}
