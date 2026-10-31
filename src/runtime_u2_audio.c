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

static int64_t ABI detour_DcJetskiClearEngineSoundOverride(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A01B20: (jetski-clear-engine-sound-override) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcJetskiOverrideEngineSound(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A01B30: (jetski-override-engine-sound [rpm] [load]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcJetskiStartEngineSound(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A01B40: (jetski-start-engine-sound) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcSetMixerOverride(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FD3B0: (set-mixer-override mixer-sym) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc <= 0 || !argv) return 0;
    uint8_t *audio = g_u2_image_base + 0x01111750ull;
    if (*(const uint8_t *)(audio + 0x8) != 0) {
        *(uint32_t *)(audio + 0x4E0) = (uint32_t)argv[0];
    }
    return 0;
}

static int64_t ABI detour_DcPlayMusicStinger(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FD3F0: (play-music-stinger stinger-sym) */
    if (status) *status = 0;
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

static int64_t ABI detour_DcStartMovieAudio(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FE0D0: (start-movie-audio movie-name [fade-time]) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc <= 0 || !argv) return 0;
    void *movie_mgr = *(void **)(g_u2_image_base + 0x010608E0ull);
    if (!movie_mgr) return 0;
    float fade_time = (argc >= 2) ? (float)(int32_t)argv[1] : 0.0f;
    typedef void (ABI *StartMovieAudioFn)(void *, uint64_t, float, uint32_t);
    StartMovieAudioFn fn = (StartMovieAudioFn)(uintptr_t)(g_u2_image_base + 0x00B822E0ull);
    if (fn) fn(movie_mgr, argv[0], fade_time, 0);
    return 0;
}

static int64_t ABI detour_DcPlayMovie(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FE100: (play-movie movie-name) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc <= 0 || !argv) return 0;
    void *movie_mgr = *(void **)(g_u2_image_base + 0x010608E0ull);
    if (!movie_mgr) return 0;
    uintptr_t *vtable = *(uintptr_t **)movie_mgr;
    if (vtable) {
        typedef uint8_t (ABI *IsPlayingFn)(void *);
        IsPlayingFn is_playing = (IsPlayingFn)vtable[7];
        if (is_playing && !is_playing(movie_mgr)) {
            typedef void (ABI *PlayMovieFn)(void *, uint64_t, uint32_t, float);
            PlayMovieFn play_fn = (PlayMovieFn)vtable[2];
            if (play_fn) play_fn(movie_mgr, argv[0], 0, 0.0f);
        }
    }
    return 0;
}

static int64_t ABI detour_DcPlaySound(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A010C0: (play-sound sound-name [vol] [pitch]) */
    if (status) *status = 0;
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

static int64_t ABI detour_DcPlaySoundAtPosition(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A00A30: (play-sound-at-position sound-name pos-ptr [vol] [pitch]) */
    if (status) *status = 0;
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

static int64_t ABI detour_DcPlaySoundAtJoint(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A00DA0: (play-sound-at-joint sound-name entity joint-id [vol] [pitch]) */
    if (status) *status = 0;
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

static int64_t ABI detour_DcPlaySoundAttached(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A00F30: (play-sound-attached sound-name entity [vol] [pitch]) */
    if (status) *status = 0;
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

static int64_t ABI detour_DcStopSound(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A01160: (stop-sound sound-handle [fade-sec]) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc <= 0 || !argv || !argv[0]) return 0;
    uint32_t sound_handle = (uint32_t)argv[0];
    uint8_t *audio_mgr = g_u2_image_base + 0x0115F1A0ull;
    uint8_t *head = *(uint8_t **)(audio_mgr + 0xCA0);
    uint8_t *cur = head;
    while (cur) {
        if ((*(const uint16_t *)(cur + 0x34) & 1) != 0) {
            if (*(const uint32_t *)(cur + 0x44) == sound_handle) {
                typedef void (ABI *StopFn)(void *);
                StopFn stop_fn = (StopFn)(uintptr_t)(g_u2_image_base + 0x009BE760ull);
                if (stop_fn) stop_fn(cur);
                break;
            }
        }
        typedef uint8_t * (ABI *NextFn)(void *, void *);
        NextFn next_fn = (NextFn)(uintptr_t)(g_u2_image_base + 0x009BCDF0ull);
        cur = next_fn ? next_fn(cur, head) : NULL;
    }
    return 0;
}

static int64_t ABI detour_DcShutUp(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A011E0: (shut-up [speaker] [fade-time]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t speaker_id = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    float fade_time = (argc >= 2 && argv) ? u2_reg_to_f32(argv[1]) : 0.0f;
    typedef uint8_t * (ABI *GetEntityFn)(uint64_t, uint32_t);
    GetEntityFn get_ent = (GetEntityFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
    uint8_t *speaker = speaker_id ? get_ent(0, speaker_id) : NULL;
    if (speaker) {
        typedef void (ABI *ShutUpFn)(uint32_t, void *, float);
        ShutUpFn fn = (ShutUpFn)(uintptr_t)(g_u2_image_base + 0x00A34240ull);
        if (fn) fn(1, speaker, fade_time);
    }
    return 0;
}

static int64_t ABI detour_DcIsSpeaking(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A013F0: (is-speaking [speaker]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t speaker_id = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    typedef uint8_t * (ABI *GetEntityFn)(uint64_t, uint32_t);
    GetEntityFn get_ent = (GetEntityFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
    uint8_t *speaker = speaker_id ? get_ent(0, speaker_id) : NULL;
    if (speaker) {
        uint32_t comp_flags = *(const uint32_t *)(speaker + 0x34);
        if ((comp_flags & 0x10000u) != 0) {
            uint8_t *speech_comp = *(uint8_t **)(speaker + 0x90);
            if (speech_comp && *(const uint32_t *)(speech_comp + 0x18) != 0) {
                return 1;
            }
        }
    }
    return 0;
}

static int64_t ABI detour_DcStopSpeaking(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A01570: (stop-speaking [speaker]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t speaker_id = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    typedef uint8_t * (ABI *GetEntityFn)(uint64_t, uint32_t);
    GetEntityFn get_ent = (GetEntityFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
    uint8_t *speaker = speaker_id ? get_ent(0, speaker_id) : NULL;
    if (speaker) {
        typedef void (ABI *StopSpeechFn)(uint32_t, void *);
        StopSpeechFn fn = (StopSpeechFn)(uintptr_t)(g_u2_image_base + 0x00A34240ull);
        if (fn) fn(1, speaker);
    }
    return 0;
}

static int64_t ABI detour_DcWaitSay(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A016D0: (wait-say speaker line-id) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc <= 0 || !argv) return 0;

    /* Clamps active train check bounds safely to prevent int $0x41 debug trap */
    uint8_t *mgr = g_u2_image_base + 0x01050320ull;
    int32_t count = *(const int32_t *)(mgr + 0x200);
    int32_t extra = -1;
    if (count >= 0 && count < 16) {
        uint8_t *slot = mgr + 8 + (size_t)count * 32;
        void *train_car = *(void **)slot;
        if (train_car) {
            uint8_t *p = *(uint8_t **)((uint8_t *)train_car + 0x10);
            if (p) extra = *(const int32_t *)(p + 8);
        }
    }

    typedef int64_t (ABI *SayFn)(int32_t, uint64_t *, int32_t);
    SayFn say_fn = (SayFn)(uintptr_t)(g_u2_image_base + 0x009F3220ull);
    int64_t res = say_fn ? say_fn(argc, argv, extra) : 0;
    if (status && res != 0) {
        *status = 2; /* pending wait status */
    }
    return res;
}

static int64_t ABI detour_DcSayPreRoll(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A019F0: (say-pre-roll speaker line-id) */
    if (status) *status = 0;
    if (argc < 2 || !argv || !argv[0] || !argv[1] || !g_u2_image_base) return 1;
    typedef uint8_t * (ABI *GetEntityFn)(uint64_t, uint32_t);
    GetEntityFn get_ent = (GetEntityFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
    uint8_t *speaker = get_ent ? get_ent(0, (uint32_t)argv[0]) : NULL;
    if (speaker) {
        uint32_t comp_flags = *(const uint32_t *)(speaker + 0x34);
        if ((comp_flags & 0x10000u) != 0) {
            typedef void (ABI *PreRollFn)(void *, uint32_t);
            PreRollFn fn = (PreRollFn)(uintptr_t)(g_u2_image_base + 0x003BE370ull);
            if (fn) fn(speaker, (uint32_t)argv[1]);
        }
    }
    return 1;
}

static int64_t ABI detour_DcSay(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A01A80: (say speaker line-id) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc <= 0 || !argv) return 0;
    typedef int64_t (ABI *SayFn)(int32_t, uint64_t *, int32_t);
    SayFn say_fn = (SayFn)(uintptr_t)(g_u2_image_base + 0x009F3220ull);
    return say_fn ? say_fn(argc, argv, -1) : 0;
}

static int64_t ABI detour_DcRailOverrideSpeedForAudio(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AB9120: (rail-override-speed-for-audio rail-id [speed]) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc < 1 || !argv) return 0;
    uint32_t rail_id = (uint32_t)argv[0];
    float speed = (argc >= 2) ? u2_reg_to_f32(argv[1]) : -1.0f;
    typedef uint8_t * (ABI *GetEntityFn)(uint64_t, uint32_t);
    GetEntityFn get_ent = (GetEntityFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
    uint8_t *rail_ent = get_ent ? get_ent(0, rail_id) : NULL;
    if (rail_ent) {
        if (speed < 0.0f) {
            typedef void (ABI *AudioEventFn)(uint32_t, void *);
            AudioEventFn fn = (AudioEventFn)(uintptr_t)(g_u2_image_base + 0x00A34240ull);
            if (fn) fn(1, rail_ent);
        } else {
            typedef float (ABI *SpeedHelperFn)(void *);
            SpeedHelperFn spd_fn = (SpeedHelperFn)(uintptr_t)(g_u2_image_base + 0x00004A70ull);
            float base = spd_fn ? spd_fn(rail_ent) : 1.0f;
            float final_spd = speed * base * 0.44704f;
            *(float *)(rail_ent + 0x120) = final_spd;
        }
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
    { "DcJetskiClearEngineSoundOverride", 0x00A01B20, (uintptr_t)detour_DcJetskiClearEngineSoundOverride },
    { "DcJetskiOverrideEngineSound", 0x00A01B30, (uintptr_t)detour_DcJetskiOverrideEngineSound },
    { "DcJetskiStartEngineSound", 0x00A01B40, (uintptr_t)detour_DcJetskiStartEngineSound },
    { "DcSetMixerOverride", 0x009FD3B0, (uintptr_t)detour_DcSetMixerOverride },
    { "DcPlayMusicStinger", 0x009FD3F0, (uintptr_t)detour_DcPlayMusicStinger },
    { "DcStartMovieAudio", 0x009FE0D0, (uintptr_t)detour_DcStartMovieAudio },
    { "DcPlayMovie", 0x009FE100, (uintptr_t)detour_DcPlayMovie },
    { "DcPlaySound", 0x00A010C0, (uintptr_t)detour_DcPlaySound },
    { "DcPlaySoundAtPosition", 0x00A00A30, (uintptr_t)detour_DcPlaySoundAtPosition },
    { "DcPlaySoundAtJoint", 0x00A00DA0, (uintptr_t)detour_DcPlaySoundAtJoint },
    { "DcPlaySoundAttached", 0x00A00F30, (uintptr_t)detour_DcPlaySoundAttached },
    { "DcStopSound", 0x00A01160, (uintptr_t)detour_DcStopSound },
    { "DcShutUp", 0x00A011E0, (uintptr_t)detour_DcShutUp },
    { "DcIsSpeaking", 0x00A013F0, (uintptr_t)detour_DcIsSpeaking },
    { "DcStopSpeaking", 0x00A01570, (uintptr_t)detour_DcStopSpeaking },
    { "DcWaitSay", 0x00A016D0, (uintptr_t)detour_DcWaitSay },
    { "DcSayPreRoll", 0x00A019F0, (uintptr_t)detour_DcSayPreRoll },
    { "DcSay", 0x00A01A80, (uintptr_t)detour_DcSay },
    { "DcRailOverrideSpeedForAudio", 0x00AB9120, (uintptr_t)detour_DcRailOverrideSpeedForAudio },
};

size_t u2_get_audio_detours(const U2NativeDetour **out) {
    if (out) *out = s_audio_detours;
    return sizeof(s_audio_detours) / sizeof(s_audio_detours[0]);
}
