# Architecture Decision Records (ADR) - Uncharted PC Port & Recompilation

> **Directive obligatoire pour les agents & développeurs** : Ce fichier consigne l'historique architectural, les leçons apprises et les règles strictes d'intégration du projet. Il doit être **lu et appliqué scrupuleusement avant toute modification de code**.

---

## Sommaire des Décisions

* [ADR-001 : Restauration de la Baseline Fonctionnelle (`e80b482`) & Archivage des Lots Massifs](#adr-001--restauration-de-la-baseline-fonctionnelle-e80b482--archivage-des-lots-massifs)
* [ADR-002 : Protocole d'Intégration et Validation Unitaire des Détours C Natifs](#adr-002--protocole-dintégration-et-validation-unitaire-des-détours-c-natifs)
* [ADR-003 : Conventions d'Appel SysV ABI (`ABI`), Toolchain MSYS2 Clang64 & Isolation Hôte/Guest](#adr-003--conventions-dappel-sysv-abi-abi-toolchain-msys2-clang64--isolation-hôteguest)
* [ADR-004 : Séparation Stricte Runtime Script C vs Moteur C++23 (`src/u2_engine/`) & Harnais Full-Body Sandbox](#adr-004--séparation-stricte-runtime-script-c-vs-moteur-c23-srcu2_engine--harnais-full-body-sandbox)
* [ADR-005 : Résolution de l'Écran Noir au Boot & Protection ABI `%rdx` dans `u2_set_status`](#adr-005--résolution-de-lécran-noir-au-boot--protection-abi-rdx-dans-u2_set_status)
* [ADR-006 : Branche `dev-native` & Exécutable Autonome 100 % C++23 (`uncharted2-native.exe`)](#adr-006--branche-dev-native--exécutable-autonome-100--c23-uncharted2-nativeexe)

---

## ADR-001 : Restauration de la Baseline Fonctionnelle (`e80b482`) & Archivage des Lots Massifs

### Statut
Accepté & Appliqué.

### Contexte & Rétrospective Historique
1. **La source de vérité initiale** : Le port HLE `Supermedo/uncharted_pc` (tag `v0.3.3`) exécute le code machine x86_64 de l'ELF PS4 (`big2-ps4_Shipping.elf`) directement sur PC via le CPU hôte et des shaders Vulkan. Dans cet état, le jeu s'exécute de façon parfaitement stable (vidéo, streaming d'assets, threading audio/vidéo).
2. **Le point de bascule destructeur (31 octobre 2026)** :
   * Entre 12h32 et 21h26, 25 lots successifs ont été injectés en masse, décompilant 843 fonctions du moteur (DC natives) d'un seul coup (+9 818 lignes de code C) sans aucun test unitaire intermédiaire.
   * Ces 843 fonctions contenaient des stubs incomplets, des retours `0` factices et des structures mal alignées.
   * Face aux plantages immédiats, une cascade de correctifs de panique a été ajoutée (forçage d'avancement d'`IntroState`, forçage de l'état `UCCApp state = 7` dès le boot dans `2a72c4e`), court-circuitant le cycle de vie normal du moteur et déclenchant des dizaines de crashs de pointeurs NULL sur des structures audio, de scène et de rendu pas encore allouées.
3. **Validation expérimentale** :
   * L'ancien état complet (Lots 1 à 25 + patchs compensatoires) a été sauvegardé dans la branche distante [`dev_archive_lots`](https://github.com/YohanDFN/Uncharted-TNDC-Recomp/tree/dev_archive_lots).
   * L'exécution testée au commit `e80b482` (dernier commit propre avant les 25 lots) s'est avérée **100% stable** : streaming de données 3D, threads `MovieDecodeThread`, `SaveWorker`, `DCLoader` actifs sans le moindre crash ni arrêt forcé.

### Décision
* Réinitialisation de la branche `dev` sur la base saine de `e80b482`.
* Proscription absolue de toute injection massive de code décompilé non testé.
* Chaque nouvelle fonction décompilée doit désormais être validée unitairement en exécution réelle (`scratch/run_test.ps1`) avant d'être commitée.

---

## ADR-002 : Protocole d'Intégration et Validation Unitaire des Détours C Natifs

### Statut
Accepté & Implémenté.

### Spécification
Tout ajout ou réécriture en C d'une fonction PS4 native doit respecter le protocole à 4 niveaux défini dans [`docs/DETOUR_VALIDATION.md`](docs/DETOUR_VALIDATION.md) :
1. **Désassemblage & Signature** : Vérification du bytecode original via `scratch/u2_disasm.js <offset> <size>`.
2. **Respect strict de l'ABI** : Utilisation du mot-clé `ABI` (`__attribute__((sysv_abi))`) pour toute fonction de rappel ou détour invoqué depuis le code guest PS4.
3. **Compilation MSYS2 Clang64** : Vérification sans warnings bloquants (`powershell -ExecutionPolicy Bypass -File scratch/build_msys2.ps1`).
4. **Validation Runtime Réelle** : Test systématique d'au moins 6 secondes (`powershell -ExecutionPolicy Bypass -File scratch/run_test.ps1 6`) avec vérification qu'aucun crash n'apparaît dans `user/last_run.log`.

---

## ADR-003 : Conventions d'Appel SysV ABI (`ABI`), Toolchain MSYS2 Clang64 & Isolation Hôte/Guest

### Statut
Accepté.

### Règles d'or du Runtime C
* **Calling convention** : Le code guest PS4 utilise l'AMD64 System V ABI (registres `rdi, rsi, rdx, rcx, r8, r9`). L'environnement Windows utilise la Microsoft x64 ABI (`rcx, rdx, r8, r9`). Toute fonction hôte appelée par le guest doit être déclarée `ABI` (ou `__attribute__((sysv_abi))`).
* **Toolchain** : MSYS2 CLANG64 sous Windows (`clang.exe` 19+, `ninja`, `cmake`).
* **Stubs temporaires** : Si une fonction décompilée n'est pas encore pleinement comprise, **ne pas la détourner** : laisser le code machine PS4 d'origine de l'ELF s'exécuter plutôt que d'injecter un stub renvoyant une valeur fausse.

---

## ADR-004 : Séparation Stricte Runtime Script C vs Moteur C++23 (`src/u2_engine/`) & Harnais Full-Body Sandbox

### Statut
Accepté & Appliqué.

### Contexte
Après l'achèvement de 100 % des 832 fonctions natives de script DC (`Dc*`) dans `src/runtime_u2_*.c`, le portage s'est étendu aux classes et sous-systèmes C++ du moteur Naughty Dog (`src/game/...`, `src/ndlib/...`). Mélanger ces classes C++ avec les bindings C du langage de script DC nuirait à la lisibilité et à la maintenabilité du code.

### Décision
1. **Architecture Modulaire C++23 (`src/u2_engine/`)** :
   * Toutes les classes et méthodes C++ du moteur (`CatmullRomSpline`, `SplineTracker`, `CameraZoom`, `RailVehicle`, `Process`, `ProcessMgr`, `ProcessList`, `SaveBuffer`, `Hud`, `GameTask`, `Settings`, `GameLoading`) sont implémentées en **C++23** dans `src/u2_engine/*.cpp` et exposées comme un 6ème module indépendant `"engine"` via `src/u2_engine/u2_engine_registry.cpp`.
   * Les fichiers `src/runtime_u2_{player,npc,scene,event,audio}.c` restent exclusivement dédiés aux fonctions de script `Dc*`.
2. **Double Validation Automatique au Démarrage (`BB_U2_SELFTEST=1`)** :
   * **Harnais Full-Body Sandbox (831 fonctions `Dc*`)** : `u2_run_engine_unit_tests()` (`src/u2_engine/u2_engine_tests.cpp`) alloue une image sandbox exécutable de 22 Mo (`PAGE_EXECUTE_READWRITE`) pré-remplie avec un stub SysV valide (`xor eax, eax; xorps xmm0, xmm0; ret`) sur `.text` et les tables `.rodata` réelles de l'ELF, puis exécute 100 % du corps de chaque détour `Dc*` via son trampoline ELF en 3 passes (`NULL`, arguments nuls, pointeurs valides).
   * **Tests Fonctionnels C++23 (via Trampolines ELF)** : Chaque méthode C++ de `src/u2_engine/` possède ses assertions fonctionnelles vérifiant les valeurs de retour (`%rax`, `%xmm0`) et les mutations de structures en passant par le saut `jmp64` installé dans l'image ELF.

---

## ADR-005 : Résolution de l'Écran Noir au Boot & Protection ABI `%rdx` dans `u2_set_status`

### Statut
Accepté & Appliqué (`36daaf2`, `80bffda`).

### Contexte & Cause Racine
Lors des tests d'amorçage complets (affichage du splash screen, montage des sauvegardes `SAVEDATA02P`/`SAVEDATA0CC` et transition vers le menu principal à 1920x1080), deux bogues critiques d'ABI ont été identifiés et corrigés :
1. **Corruption de la Free-List des Frames de Script (`ScriptManager::Run` `0x00A14F20`)** :
   * `0x00A14F20` reçoit **5 arguments** en registres SysV (`%rdi=self`, `%rsi=code`, `%rdx=arg2`, `%rcx=arg3`, `%r8=arg4`) et dépile la tête de la free-list `g_pScriptStackFrameFreeList` (`0x011639F0`) via le pointeur `next` situé à l'**offset `+0x00`** (`mov (%rcx), %rdx`), et non `frame->caller` (`+0x20`).
   * De plus, dans `ExecuteScriptCode` (`0x00A14120`), lorsque `status == 2` (yield de frame), la fonction doit retourner `*(uint64_t *)(frame->sp - 8)` (`0x00A146D7`) et non `0`.
2. **Crash à 16 secondes dans `Process::KillTree` (`0x009BE8AE`) causé par `u2_set_status(status, 0)`** :
   * Certaines fonctions `Dc*` à 0 argument comme `DcKillAllAttachables` (`0x009F2BF0`) sont appelées directement depuis le code C++ du moteur (`0x00A54E49`, juste après `NewSetting` `0x00A1CD50`) sans passer par la VM de script, laissant un pointeur `Process *` valide sur le tas (`0x000000105da23480`) dans le 3ème registre d'argument `%rdx` (`uint32_t *status`).
   * Comme l'ELF PS4 est mappé à `0x000000000b900000` (`< 4 Go`), écrire `*status = 0` sur 32 bits mettait à zéro l'intégralité du pointeur de vtable 64 bits de l'objet `Process`, provoquant un crash différé lors de l'appel virtuel `0xb8(%rax)` dans `Process::KillTree`.

### Décision
* **Règle ABI `u2_set_status` (`src/runtime_u2_common.h`)** : La VM de script pré-initialisant systématiquement `*status = 0` sur la pile avant chaque appel natif (`0x00A14627: movl $0x0, 0x1c(%rsp)` et `0x00A1578C: movl $0x0, 0x1c(%rsp)`), `u2_set_status(status, val)` ignore désormais toute écriture de `val == 0` et n'écrit dans `%rdx` que les codes non nuls (`1` = exit/abort, `2` = yield/wait).
* Une assertion de non-régression dans `u2_run_engine_unit_tests()` vérifie à chaque démarrage qu'aucun des 831 détours `Dc*` n'écrase `(%rdx)` avec `0`.

---

## ADR-006 : Branche `dev-native` & Exécutable Autonome 100 % C++23 (`uncharted2-native.exe`)

### Correction de frontière mémoire (11 novembre 2026)

Les huit entrées de la famille AllocStack hôte sont réservées au registre de
la cible compilée avec `U2_STANDALONE=1`. Dans `bb-probe`, allocation, pile et
libération restent ensemble dans l'ELF. Le C++ utilise exclusivement son
`thread_local` hôte et ne lit ni n'écrit le slot TLS invité ; sa libération
ne peut donc pas recevoir un bloc du pool ELF. Les tests installent leurs
trampolines mémoire uniquement dans la sandbox et vérifient la préservation
d'une pile invitée sentinelle. Voir [les preuves et limites](docs/HYBRID_REGRESSION_ALLOCSTACK.md).

### Statut
Accepté & Planifié sur `dev-native`.

### Décision
* Création de la branche dédiée `dev-native` clonée depuis `dev` (`3d5dded`) pour développer en parallèle un second exécutable autonome : **`uncharted2-native.exe`**.
* **`uncharted2-native.exe`** contient uniquement le code C++23 réécrit (`src/u2_engine/*.cpp`, `src/u2_standalone/*.cpp`), les 832 bindings `Dc*` et les couches PC (`bbgpu` Vulkan/SDL3/WASAPI/ATRAC9), sans exécuter le moindre octet de code machine PS4 (`eboot.elf`).
* Le plan d'architecture complet en 5 piliers et 3 étapes séquentielles (Boot -> Splash Screen -> Menu Principal) est documenté dans [`docs/UNCHARTED_NATIVE_PLAN.md`](docs/UNCHARTED_NATIVE_PLAN.md).
