# Architecture Decision Records (ADR) - Uncharted PC Port & Recompilation

> **Directive obligatoire pour les agents** : Ce fichier consigne toutes les décisions architecturales, patchs natifs, detours C et conventions du projet. Il doit être **lu et pris en compte à chaque requête** avant toute modification de code.

---

## Sommaire des Décisions

* [ADR-001 : Transition d'État IntroState vers Title Screen & Déblocage Cinématique](#adr-001--transition-détat-introstate-vers-title-screen--déblocage-cinématique)
* [ADR-002 : Gestion du Manifest Shaders Devkit (`files.json`) & Prévention Crash NULL `0x00BDFA3C`](#adr-002--gestion-du-manifest-shaders-devkit-filesjson--prévention-crash-null-0x00bdfa3c)
* [ADR-003 : Silençage des Assertions Shaders Devkit (`0x00CC1010`) & Fluidité Console](#adr-003--silençage-des-assertions-shaders-devkit-0x00cc1010--fluidité-console)
* [ADR-004 : Sécurisation des Job Workers & Gardes Pointeurs NULL (`0x001D30A5`, `0x001D7A40`, `0x00B3DB04`)](#adr-004--sécurisation-des-job-workers--gardes-pointeurs-null-0x001d30a5-0x001d7a40-0x00b3db04)
* [ADR-005 : Synchronisation GPU EOP Tick (`0x00B29F10`) & Queue Compute (`0x00C24250`)](#adr-005--synchronisation-gpu-eop-tick-0x00b29f10--queue-compute-0x00c24250)
* [ADR-006 : Conventions d'Appel SysV ABI (`ABI`) & Toolchain MSYS2 Clang64](#adr-006--conventions-dappel-sysv-abi-abi--toolchain-msys2-clang64)
* [ADR-007 : Prévention Débordement Tas Menu (456B Devkit Shaders) & Court-Circuit `0x00CC103A`](#adr-007--prévention-débordement-tas-menu-456b-devkit-shaders--court-circuit-0x00cc103a)
* [ADR-008 : Sécurisation du Cycle de Vie Mutex (`libSceFios2`), Garde Stat Buffer & Scheduler Device](#adr-008--sécurisation-du-cycle-de-vie-mutex-libscefios2-garde-stat-buffer--scheduler-device)
* [ADR-009 : Garde Nœuds Chunks ScriptManager (`0x00A17C60`) & Fallback Allocateur de Tas `AllocByAllocatorId` (`0x00B33270`)](#adr-009--garde-nœuds-chunks-scriptmanager-0x00a17c60--fallback-allocateur-de-tas-allocbyallocatorid-0x00b33270)
* [ADR-010 : Demand-Commit Mémoire Guest (`0x1000000000`..`0x2000000000`) & Résolution Crash `0x00A17B58`](#adr-010--demand-commit-mémoire-guest-0x10000000000x2000000000--résolution-crash-0x00a17b58)
* [ADR-033 : Neutralisation de la Boucle d'Attente Synchrone Audio `brb_StartSession` (`0x0012E890`)](#adr-033--neutralisation-de-la-boucle-dattente-synchrone-audio-brb_startsession-0x0012e890)
* [ADR-034 : Déblocage de la Barrière PlayGo dans `GameLoadingMountInitialArchives` (`0x00B77C77`)](#adr-034--déblocage-de-la-barrière-playgo-dans-gameloadingmountinitialarchives-0x00b77c77)
* [ADR-035 : Neutralisation de la Trappe d'Assertion `m_gfxEopTick` dans `FrameBegin` (`0x00BD440F`)](#adr-035--neutralisation-de-la-trappe-dassertion-m_gfxeoptick-dans-framebegin-0x00bd440f)
* [ADR-036 : Protection Liste de Périphériques FIOS2 NULL dans la Résolution de Chemins (`0x0184BB9C`)](#adr-036--protection-liste-de-périphériques-fios2-null-dans-la-résolution-de-chemins-0x0184bb9c)
* [ADR-037 : Protection Pointeur de Filtre Virtuel FIOS2 Non Validé (`0x0184BBEB`)](#adr-037--protection-pointeur-de-filtre-virtuel-fios2-non-validé-0x0184bbeb)
* [ADR-038 : Sécurisation du Compteur de Synchronisation Frame Job 2 (`0x003F68C0`)](#adr-038--sécurisation-du-compteur-de-synchronisation-frame-job-2-0x003f68c0)
* [ADR-039 : Neutralisation des Callbacks Indirects Audio Mixer Corrompus (`0x001377A6` & `0x00137874`)](#adr-039--neutralisation-des-callbacks-indirects-audio-mixer-corrompus-0x001377a6--0x00137874)
* [ADR-040 : Neutralisation du Validateur Vectoriel de Paramètres Audio (`0x0013C740`)](#adr-040--neutralisation-du-validateur-vectoriel-de-paramètres-audio-0x0013c740)
* [ADR-041 : Architecture de Découplage et Validation Modulaire des Détours C Natifs](#adr-041--architecture-de-découplage-et-validation-modulaire-des-détours-c-natifs)

---

## ADR-001 : Transition d'État IntroState vers Title Screen & Déblocage Cinématique

### Statut
Accepté & Implémenté.

### Contexte
Au démarrage du jeu (*Uncharted 2: Among Thieves* via `big2-ps4_Shipping.elf`), l'état actif initial est `IntroState`.
Le moteur Naughty Dog contrôle la cinématique d'introduction et la transition vers l'écran titre (`TitleState`) via deux routines :
1. `IntroState::Update` (`0x00A551B0`) : Attend 300 frames (5 secondes) ou la fin de lecture du `MovieManager` avant d'invoquer `*0x158(%rax)` (`AdvanceToTitle`). En cas d'interruption ou si aucun film n'est en cours, il saute à `0x00A55261` qui tente un appel indirect `jmpq *0xd8(%rax)`.
2. `IntroState::Advance` / `AdvanceToTitle` (`0x00A62BC0` et `0x00A63460`) : Routines officielles chargées de construire l'événement de transition `0x0153E2B8` (ou `0x0153E2E0`), de positionner le bit de statut `0x100` à `+0x34(state)` et d'enregistrer l'événement dans le `StateManager` (`0x08(state_mgr)`).

### Problème
* L'entrée `0xd8` de la table de saut d'IntroState contient la valeur scalaire `0x8` au lieu d'un pointeur de fonction, causant un crash en cas de saut direct.
* Dans `0x00A62BC0` et `0x00A63460`, un appel de contrôle `callq *0xc0(%rax)` suivi de `testb %al, %al ; je abort` échoue systématiquement sur PC car `vtable[0xc0]` renvoie 0. La transition était donc annulée et l'écran restait noir indéfiniment.

### Décision
1. **Patch `0x00A62C5C` & `0x00A634FC`** : NOP (`90 90`) du saut conditionnel `je abort` dans les deux fonctions de transition.
2. **Patch `0x00A55229` & `0x00A5524E`** : NOP (`90 90`) de la boucle d'attente cinématique de 300 frames et du saut vers `0xd8` dans `IntroState::Update` native.
3. **Detour `detour_IntroState_Update` (`0x00A551B0`)** : Dès la frame 15 ou sur appui d'une touche de confirmation (`pad_buttons & 0x6008`), invoque directement `AdvanceToTitle` (`0x00A62BC0(this_ptr)`).

---

## ADR-002 : Gestion du Manifest Shaders Devkit (`files.json`) & Prévention Crash NULL `0x00BDFA3C`

### Statut
Accepté & Implémenté.

### Contexte
Dans `ndlib\render\ingame-shaders.cpp`, la fonction `Init` (`0x00BDF9B0`) tente de lire `/app0/u2data/build/main/bin/shaders/src/files.json` via `JsonParseFiles` (`0x00C2C230`). Ce fichier est un artefact de développement présent uniquement sur les devkits PS4 de Naughty Dog, absent des versions de distribution commerciale (retail).

### Problème
Lorsque `files.json` n'est pas trouvé :
1. `JsonParseFiles` renvoie `NULL` (`%rax = 0`).
2. `0x00BDF9B0` affiche l'assertion `ASSERTION: root` et déclenche `int $0x41`.
3. Le gestionnaire d'exception de `probe.c` avance `RIP += 2` pour contourner le trap, arrivant à `0x00BDFA3C` : `cmpl $0x5, (%rax)`.
4. Le déréférencement de `0x0` entraîne un crash fatal `Fault: read access to 0000000000000000 at guest offset 0xbdfa3c (code 3)`.

### Décision
1. **Fallback JSON Detour (`0x00C2C230`)** : Renvoyer un nœud JSON factice (`s_dummy_json_root`, type 5, sans enfants) pour garantir que `JsonParseFiles` ne renvoie jamais `NULL` aux appelants.
2. **Patch de sortie propre `0x00BDF9FC`** : Si `%rax == NULL`, sauter directement à l'épilogue de la fonction `0x00BE1011` (`retq`), évitant tout déréférencement et préservant l'intégrité de la pile.

---

## ADR-003 : Silençage des Assertions Shaders Devkit (`0x00CC1010`) & Fluidité Console

### Statut
Accepté & Implémenté.

### Contexte
La table de shaders devkit `0x0124A208` n'est pas alimentée par `files.json` en version retail. La routine de recherche de shaders par ID `0x00CC1010` (appelée par `GetVsShaderByNameId`, `GetPsShaderByNameId`, etc.) constate que le nombre d'entrées est 0 (`0x10(%rbx) == 0`).

### Problème
À chaque recherche, le moteur formate et affiche une bannière d'alerte de 8 lignes :
`*** HALT: No VS shader with name 'str_...'`
suivie d'un `int $0x41`. Comme ce diagnostic s'exécute des centaines de fois par image, la console Windows est saturée, le thread principal est ralenti de 95% et l'affichage se fige.

### Décision
* **Patch `0x00CC10AB`** (`74 97`) : Si le nombre de vertex shaders est 0, sauter directement à `0x00CC1144` pour réinitialiser `%r15` à NULL sans émettre de message.
* **Patch `0x00CC114F`** (`90 90`) : NOP du trap `int $0x41`.
* **Patch `0x00CC115E`** (`74 95`) : Si le nombre de pixel shaders est 0, sauter directement à `0x00CC11F5` pour réinitialiser `%r14` à NULL sans émettre de message.
* **Patch `0x00CC1200`** (`90 90`) : NOP du trap `int $0x41`.

---

## ADR-004 : Sécurisation des Job Workers & Gardes Pointeurs NULL (`0x001D30A5`, `0x001D7A40`, `0x00B3DB04`)

### Statut
Accepté & Implémenté.

### Contexte
Le moteur Naughty Dog s'appuie massivement sur un système de micro-threads (Fibers / Jobs Naughty Dog) partagé entre le thread principal et plusieurs `NdJobWorkerThread`. Sur architecture PC avec recompilation statique/dynamique, certains buffers et pointeurs d'état diffèrent des plages PS4 contiguës.

### Décision
1. **`0x00B3DB04` (`hook_JobDispatch`)** : Valide que le pointeur de fonction et l'argument de job se trouvent dans les limites mémoires autorisées (guest ou host code base) avant exécution pour éviter tout branchement corrompu.
2. **`0x001D30A5` (`s_patch_1d30a5`)** : Garde 11 octets vérifiant que `%rdx` est non-nul avant d'effectuer l'effacement atomique de masque de bits dans les animations/jobs. Préserve strictement l'instruction `movq 0xb0(%r15), %rdx` à `0x001D309E`.
3. **`0x001D7A40` (`s_patch_1d7a40`)** : Vérifie que le tableau de squelettes/submeshes dans `%rdx` est non-nul avant d'ajouter le déplacement `$0x30`.
4. **`0x001E2110` (`hook_1e2110`)** : Empêche les déréférencements sous le seuil `0x1000` lors de la mise à jour des émetteurs de particules.

---

## ADR-005 : Synchronisation GPU EOP Tick (`0x00B29F10`) & Queue Compute (`0x00C24250`)

### Statut
Accepté & Implémenté.

### Contexte
Sur PS4, le GPU AMD GCN écrit directement en mémoire hôte les ticks de fin d'exécution (`EVENT_WRITE_EOP`). Sur PC Vulkan, les ticks ne sont pas automatiquement écrits dans la mémoire CPU guest, provoquant un blocage de synchronisation (`usleep(1000)` à `0x00B29FF0`).

### Décision
1. **`detour_GfxWaitEopFence` (`0x00B29F10`)** : Fait progresser `*(uint64_t *)(sync_obj + 0x20)` vers `target_tick` pour satisfaire la condition de clôture et libérer la boucle de soumission Vulkan.
2. **`detour_MapComputeQueue` (`0x00C24250`)** : Initialise les identifiants de pipe et queue de calcul et retourne 1 pour valider l'allocation des Compute Queues.

---

## ADR-006 : Conventions d'Appel SysV ABI (`ABI`) & Toolchain MSYS2 Clang64

### Statut
Accepté & Implémenté.

### Contexte
Le code recompilé de l'ELF PS4 attend la convention d'appel **System V AMD64 ABI** (`rdi`, `rsi`, `rdx`, `rcx`, `r8`, `r9`, registres callee-saved `rbx`, `rsp`, `rbp`, `r12`-`r15`), tandis que les compilateurs Windows ciblent par défaut la **Microsoft x64 ABI** (`rcx`, `rdx`, `r8`, `r9`, shadow stack de 32 octets).

### Décision
* Toute fonction C appelée directement depuis l'ELF ou remplaçant une routine de l'ELF doit impérativement porter l'attribut `__attribute__((sysv_abi))` (défini par la macro `ABI` dans [`src/runtime_u2_internal.h`](file:///c:/Users/Yohan/Desktop/uncharted_pc-0.3.3/src/runtime_u2_internal.h)).
* Tout typedef de pointeur de fonction vers le guest doit inclure `ABI`.
* L'ensemble des modules dans `src/` doit être compilé en C pur (C99/C11) via la toolchain **MSYS2 CLANG64** avec le script [`scratch/build_msys2.ps1`](file:///c:/Users/Yohan/Desktop/uncharted_pc-0.3.3/scratch/build_msys2.ps1).

---

## ADR-007 : Prévention Débordement Tas Menu (456B Devkit Shaders) & Court-Circuit `0x00CC103A`

### Statut
Accepté & Implémenté.

### Contexte
À l'initialisation du moteur de rendu (autour de la frame #5, après chargement des polices), la routine `0x00CC1010` (appelée par `InitMgr` à `0x00C8EB50`) tente d'allouer une table de requêtes pour 19 types de shaders (19 * 24 octets = 456 octets = `0x1C8`).
L'allocation est dirigée vers l'allocateur de tas `0x13` ("Menu", `0x1032702000`).

### Problème
1. Le tas "Menu" est dimensionné à 2 Mo (`0x200000`). Avec le chargement des fichiers `.fnt` (`test2.fnt`, `main.fnt`, `symbol3.fnt`), il ne restait que 240 octets libres.
2. La requête de 456 octets déclenche `HeapArena::Allocate: out of memory` et l'assertion `pData` (`0x00B3008F`).
3. Après contournement du trap, `0x00CC105B` reçoit `%rax = 0` (NULL) et exécute `0x00CC1068 : call memset(NULL, 0, 456)`.
4. Cela entraîne un crash fatal immédiat : `Fault: write access to 0000000000000020 at ucrtbase.dll+0xedec0`.
5. En retail, la table de shaders devkit `0x0124A208` est vide : la boucle de 19 itérations n'écrivait que des NULLs dans ce buffer, et aucune routine retail ne lit `0x58(%r13)`.

### Décision
* **Patch `0x00CC103A`** (7 octets : `e9 9a 02 00 00 90 90`) : Saut inconditionnel `jmp 0x00CC12D9` (disp32 = `+0x29A`) suivi de deux NOPs.
* **Effet** : Supprime intégralement la requête d'allocation de 456 octets dans le tas Menu, neutralise le `memset` sur NULL, évite l'assertion out-of-memory et enchaîne directement sur la création des Constant Buffers du moteur de rendu à `0x00CC12D9`.

---

## ADR-008 : Sécurisation du Cycle de Vie Mutex (`libSceFios2`), Garde Stat Buffer & Scheduler Device

### Statut
Accepté & Implémenté.

### Contexte
La couche I/O de streaming PS4 `libSceFios2.prx` interagit directement avec les primitives de synchronisation noyau (`scePthreadMutex*`) et gère le système de requêtes de fichiers asynchrones.

### Problèmes Rencontrés
1. **Crash UD2 `scePthreadMutexLock` (`0x01851270`)** : Lors de la destruction d'un mutex, `mutex_destroy` écrivait la sentinelle `*mutex = 2`. Lors d'une réutilisation ultérieure, `ensure_mutex` rejetait la valeur 2 avec `EINVAL` (`0x80020016`). `libSceFios2` déclenchait un `ud2` immédiat en cas d'erreur de lock.
2. **Crash `bzero(NULL, 80)` à `0x0183BD53`** : `sceFiosStatSync` recevait une requête sans buffer de stat utilisateur (`0x20(%r14) == NULL`), exécutant aveuglément `memset_wrapper(NULL, 0, 80)` et écrivant des champs à l'adresse 0.
3. **Crash `fios_mutex_lock` sur `0x10` à `0x0184C610`** : Quand `get_device` renvoyait `NULL`, la routine `0x0184C610` calculait `16(%r13) == 0x10` et tentait de verrouiller un mutex fictif à l'adresse `0x10`.
4. **Crash `scePthreadMutexDestroy` `EBUSY` (`0x01850D61`)** : Lors du nettoyage des sous-systèmes, détruire un mutex encore verrouillé renvoyait `EBUSY`, causant un trap `ud2`.

### Décision
1. **`src/runtime_mutex.c`** :
   - `ensure_mutex` et `mutex_lock` auto-réinitialisent les descripteurs lorsque `(uintptr_t)*mutex < 0x10000`.
   - `mutex_destroy` réinitialise `*mutex = NULL;` au lieu de `2`.
   - `mutex_destroy` déverrouille de force le mutex en cas de retour `EBUSY`, libère la mémoire et retourne `0` (`SCE_OK`).
2. **Trampoline `0x0185D0C0` (36 octets)** : Intercepte `0x0183BD53` pour tester `%rbx`. Si `%rbx == NULL`, saute directement à l'épilogue `0x0183BDD7` avec `%r15d = 0`.
3. **Trampoline `0x01860C98` (21 octets)** : Intercepte `0x0184C610` pour tester `%rdi`. Si `%rdi == NULL`, retourne immédiatement 0 (`false`).

---

## ADR-009 : Garde Nœuds Chunks ScriptManager (`0x00A17C60`) & Fallback Allocateur de Tas `AllocByAllocatorId` (`0x00B33270`)

### Statut
Accepté & Implémenté.

### Contexte
Dans le gestionnaire de scripts Naughty Dog (`ScriptManager::Initialize` à `0x00A173A0`), deux structures critiques sont allouées :
1. Une liste de 117 nœuds de tracking de paquets / chunks (boucle `0x00A17B20`..`0x00A17D77`), allouant 216 octets par nœud via l'allocateur 0 ("UNKNOWN:0").
2. La table des chunks de scripts (2608 octets = `0xa3 * 16`), allouée via `AllocByAllocatorId` (`0x00B33160`) avec l'ID d'allocateur `0x12`.

### Problèmes Rencontrés
1. **Crash à `0x00A17C63`** : Lorsque l'allocateur 0 n'a plus de mémoire (`CPU Mem [F:0 T:0]`), `%rax` est NULL. Le code exécutait `movq $0, 0x8(%r13)` avec `%r13 == NULL`, provoquant une violation d'accès en écriture à `0x0000000000000008`.
2. **Crash à `0x00B33273`** : Dans la version retail, la table `g_HeapArenas` (`0x0104AE00`) ne contient pas d'arène valide pour l'ID `0x12` (`table[0x12] == NULL`). La routine `0x00B33270` exécutait `movq (%rbx), %rdi` puis déréférençait immédiatement `movq (%rdi), %rax` pour lire la vtable, causant :
   `Fault: read access to 0000000000000000 at guest offset 0xb33273`.

### Décision
1. **Patch `0x00A17C60` (50 octets) & `0x00A19601` (49 octets)** & Helper `u2_alloc_chunk_node_fallback` dans [`src/runtime_u2_script.c`](file:///c:/Users/Yohan/Desktop/uncharted_pc-0.3.3/src/runtime_u2_script.c) :
   - Préserve l'instruction initiale `movq %rax, %r13` à `0x00A17C60` (et `movq %rax, %r14` à `0x00A19601`) sur 3 octets afin que les sauts conditionnels `je 0x00A17C63` (à `0x00A17C27` et `0x00A17C37` lorsque `ecx == 0`, où `%r13` vaut 0 mais `%rax` contient `pAllocStack`) ne passent jamais `pAllocStack` comme nœud de chunk.
   - Passe `%r13` (resp. `%r14`) à `u2_alloc_chunk_node_fallback`, qui retourne un buffer de repli 16-octets aligné via `_aligned_malloc(216, 16)` mis à zéro si l'allocation a échoué.
2. **Patch `0x00B33270` & `0x00B33130` (39 octets chacun)** & Helper `u2_alloc_arena_or_fallback` dans [`src/runtime_u2_script.c`](file:///c:/Users/Yohan/Desktop/uncharted_pc-0.3.3/src/runtime_u2_script.c) :
   Remplace les blocs `0x00B33270`..`0x00B33296` et `0x00B33130`..`0x00B33156` (les deux variantes d'`AllocByAllocatorId`) par un relais vers `u2_alloc_arena_or_fallback(%rbx, %r14)`.
   Cette routine C SysV ABI vérifie la validité du pointeur d'arène et de sa vtable :
   - Si valide, elle délègue à `Arena::Allocate(arena, size, 16, "UNKNOWN", 0, "UNKNOWN")`.
   - Si l'arène est NULL ou si l'allocation échoue, elle alloue dynamiquement via `_aligned_malloc(alloc_sz, 16)`, met à zéro la mémoire et la retourne proprement à l'appelant.

---

## ADR-010 : Demand-Commit Mémoire Guest (`0x1000000000`..`0x2000000000`) & Résolution Crash `0x00A17B58`

### Statut
Accepté & Implémenté.

### Contexte
Sur PS4, l'espace mémoire physique direct (1600 Mo à `0x1000000000`, 3100 Mo à `0x1100000000`) et flexible (335 Mo à `0x1200000000`) est alloué via `MapViewOfFile3` avec la section partagée de repli (`SEC_RESERVE`).

### Problème
Sur l'hôte Windows :
1. Les sections de mapping créées avec `SEC_RESERVE` ne peuvent pas être committées via `VirtualProtect` (qui échoue sur les pages non committées d'une vue de section).
2. Le gestionnaire de fautes vectorisées (`vectored_fault` dans `src/probe.c`) n'intervenait que pour les écritures (`ExceptionInformation[0] == 1`) et appelait `VirtualProtect` sans vérifier si la page était déjà `PAGE_READWRITE`, risquant une boucle infinie sur faute d'alignement SSE.

### Décision
* Dans [`src/probe.c`](file:///c:/Users/Yohan/Desktop/uncharted_pc-0.3.3/src/probe.c) (`vectored_fault`) et [`src/runtime_memory.c`](file:///c:/Users/Yohan/Desktop/uncharted_pc-0.3.3/src/runtime_memory.c) (`view_map`) :
  1. `view_map` mappe systématiquement les vues `MapViewOfFile3` avec `initial = PAGE_READWRITE` avant d'appliquer la protection cible via `VirtualProtect`.
  2. Pour tout accès fautif dans `[0x1000000000, 0x2000000000)` avec `mbi.State == MEM_RESERVE`, `vectored_fault` committe à la demande via `VirtualAlloc(..., MEM_COMMIT, PAGE_READWRITE)` (ou remplace le placeholder via `VirtualAlloc2(..., MEM_REPLACE_PLACEHOLDER)`).
  3. Pour une page déjà committée (`mbi.State == MEM_COMMIT`), `VirtualProtect` n'est invoqué que si `mbi.Protect` n'est pas déjà `PAGE_READWRITE` / `PAGE_EXECUTE_READWRITE`.

---

## ADR-011 : Sécurisation des Variables de Condition (`pthread_cond_*`) & Auto-Initialisation Mutex dans `cond_wait`

### Statut
Accepté & Implémenté.

### Contexte
Les modules système PS4 liés statiquement (notamment `libSceFios2.prx` à l'offset guest `0x1851328`) utilisent des variables de condition et des mutex (`scePthreadCondWait`, `scePthreadCondTimedwait`, `scePthreadCondSignal`, `scePthreadCondBroadcast`) dont les pointeurs de handle peuvent contenir des sentinelles statiques (`0`, `1`, `2`) ou ne pas avoir été explicitement initialisés avant le premier appel d'attente.

### Problème
Dans [`src/runtime_mutex.c`](file:///c:/Users/Yohan/Desktop/uncharted_pc-0.3.3/src/runtime_mutex.c) :
1. `ensure_cond`, `cond_wait`, `cond_wait_until`, `cond_signal`, `cond_broadcast` et `cond_destroy` ne filtraient que `(uintptr_t)*cond < 2` (ou `< 3`), laissant passer des valeurs basses invalides (`< 0x10000`).
2. Plus critique encore : `cond_wait` et `cond_wait_until` appelaient `ensure_cond(cond)` mais **n'appelaient pas `ensure_mutex(mutex)`**, passant directement `&(*mutex)->native` à `pthread_cond_wait` / `pthread_cond_timedwait`. Lorsque `*mutex` valait `NULL` (`0x0`), `libwinpthread-1.dll+0x436b` déréférençait l'adresse `0x0000000000000000`, provoquant un crash immédiat :
   `Fault: read access to 0000000000000000 at libwinpthread-1.dll+0x436b` depuis `libSceFios2.prx` (`0x1851328` <- `0x184cb5b` <- `0xa17bea`).

### Décision
* Dans [`src/runtime_mutex.c`](file:///c:/Users/Yohan/Desktop/uncharted_pc-0.3.3/src/runtime_mutex.c) :
  1. Harmonisation du seuil de validité des handles de variables de condition à `(uintptr_t)*cond < 0x10000` (identique à `ensure_mutex` et `ensure_rwlock`).
  2. Dans `cond_wait` et `cond_wait_until`, vérification systématique de `cond` et `mutex` (`> 0x10000`) et invocation conjointe de `ensure_cond(cond)` **et** `ensure_mutex(mutex)` avant tout appel à `pthread_cond_wait` / `pthread_cond_timedwait`.
  3. Sécurisation de `cond_signal`, `cond_broadcast` et `cond_destroy` contre les pointeurs NULL ou `< 0x10000`.
* **Résultat** : Le crash dans `libwinpthread-1.dll+0x436b` lors des opérations FIOS2 est totalement éliminé et la boucle de chargement des 117 archives de `ScriptManager` s'exécute sans aucune erreur d'allocation ni violation d'accès.

---

## ADR-012 : Restauration de la Synchronisation des Jobs CPU (`0x00B29F10` `JobSyncCounter::Wait`) & Élimination du Shutdown Prématuré (`0x00B72520`)

### Statut
Accepté & Implémenté.

### Contexte
La fonction guest `0x00B29F10` (`JobSyncCounter::Wait(void *sync_obj, int64_t target_val, uint32_t mode)`) est la primitive centrale de synchronisation du système de jobs Naughty Dog :
* **`mode == 1`** : Attend que le compteur signé `*(int64_t *)(sync_obj + 0x20) <= target_val` (utilisé pour les jalons de chargement `g_pGameLoadingState` à `0x015AF750` : `WaitForBinAvailability(10)`, `WaitForDataAvailability(9)`, `WaitForCoreAvailability(8)` et pour le tick de trame négatif dans `RunFrame` à `0x003FB6F3`).
* **`mode == 0`** : Attend que le compteur de jobs actifs `*(int64_t *)(sync_obj + 0x20) == target_val` (typiquement `0`), utilisé par `WaitAndReleaseJobSyncCounter` (`0x00B2A040`) et `SubmitAndWaitForJobs` (`0x00B29EFD`).

### Problème
Le détour précédent `detour_GfxWaitEopFence` installé à `0x00B29F10` écrasait inconditionnellement la fonction et retournait immédiatement sans attendre lorsque `mode == 0` :
1. **Assertions de shaders Vertex (`VS_ScreenSpace`, `WorldTransformVS`, etc.)** : Dans `AppInit` (`0x003F41B0`), le thread principal soumettait le job d'initialisation des shaders `0x003F6680` puis appelait `0x00B2A040` (`0x003F43AB`) pour attendre sa complétion avant d'appeler `CreateVertexLayoutFromDesc` (`0x00C8EDB0`). Comme `0x00B29F10` retournait immédiatement, `CreateVertexLayoutFromDesc` s'exécutait avant le chargement des shaders et `0x00B2A040` libérait prématurément le `JobSyncCounter` pendant que le job tournait encore.
2. **Sortie immédiate de `AppMainLoop` (`0x003F4AA0`) et crash dans `AppShutdown` (`0x003F4470` -> `0x00B72520`)** : `AppMainLoop` soumet le job de boucle principale `0x003FA9F0` (`MainLoopJob`) et appelle `0x00B2A040` (`0x003F4BA9`, `mode = 0, target = 0`) pour attendre la fin du jeu. Le retour immédiat de `0x00B29F10` faisait sortir `AppMainLoop` instantanément et enchaînait sur `AppShutdown` (`0x00576752` -> `0x003F4470` -> `0x00B72520` `StreamingShutdown`), déclenchant 8 assertions `CloseSync (fh != 0)`, `sceFiosTerminate` (`fios2 warning: terminated...`) et une violation d'accès fatale (`0xc0000005` / `3221225477`) pendant que les threads workers exécutaient encore l'initialisation.

### Décision
* Dans [`src/runtime_u2_script.c`](file:///c:/Users/Yohan/Desktop/uncharted_pc-0.3.3/src/runtime_u2_script.c) (`detour_GfxWaitEopFence` et trampoline `orig_SyncCounter_Wait`) :
  1. Lorsque `mode == 1` (ou lorsque `sync_obj` correspond à `g_pCheckPlayGoStatus` à `*(void **)(g_u2_image_base + 0x015AF7D0)`), `*(int64_t *)(sync_obj + 0x20)` est avancé à `(int64_t)target_tick` et la fonction retourne immédiatement.
  2. Lorsque `mode == 0` (attente d'un lot de jobs CPU), le détour délègue au code original de `0x00B29F10` via le trampoline `orig_SyncCounter_Wait` (qui exécute le prologue de 16 octets et saute à `g_u2_image_base + 0x00B29F20`), permettant aux threads workers de céder leur fibre (`0x00B3E4C0`) et au thread principal d'attendre proprement la complétion des jobs d'initialisation et de `MainLoopJob`.
* **Résultat** : Disparition totale des 20+ assertions `GetVsSignatureByName` / `CreateVertexLayoutFromDesc`, disparition totale des 8 assertions `CloseSync (fh != 0)`, du `fios2 warning: terminated` et du crash `3221225477`. Le moteur charge `DCLoader`, initialise `UserService` et `NpTrophy`, masque le splash screen (`SystemService: splash screen hidden`) et transite proprement vers `UCCApp State 0 -> 1` en tournant de manière stable.

---

## ADR-013 : Activation de l'Écran Titre (`UCCApp State 1 -> 7`) & Contournement du Skip Retail (`0x00D5A95F`)

### Statut
Accepté & Implémenté.

### Contexte
Dans les builds retail PS4 d'Uncharted 2 Remastered, la machine d'état racine `UCCApp` commence à l'état 0 (Boot/Pre-init) puis transite à l'état 1 (Pre-title / Séquence d'intro vidéo). Dans `UCCApp::Update` (`0x00D5A95F`), le moteur vérifie si `m_state < 7` et saute tout le tick de rendu (`ja 0xd5b12d`).

### Problème
Sans décodeur vidéo matériel PS4 complet, le moteur restait bloqué en State 1 sans présenter l'écran titre et sans déclencher le chargement des shaders du menu principal.

### Décision
* Dans [`src/runtime_u2_script.c`](file:///c:/Users/Yohan/Desktop/uncharted_pc-0.3.3/src/runtime_u2_script.c) :
  1. Patch à `0x00D5A95F` : remplacement du saut conditionnel `ja 0xd5b12d` par 6 NOPs (`66 0F 1F 44 00 00`).
  2. Transition automatique de `State 1` vers `State 7` (Title Screen) dès la dissimulation du splash screen, permettant d'activer le rendu du menu et la boucle principale.

---

## ADR-014 : Protection contre les Éléments d'Animation Nuls (`0x0055789A`)

### Statut
Accepté & Implémenté.

### Contexte & Problème
Lors de la mise à jour des contrôleurs d'animation de décors et de squelettes à `0x0055789A`, le moteur parcourait une liste d'instances pouvant contenir des pointeurs nuls, provoquant un crash par déréférencement immédiat.

### Décision
* Installation d'un trampoline de vérification dans le code cave de segment PH 0, sautant l'élément si le pointeur est nul.

---

## ADR-015 : Sécurisation du Buffer Stat FIOS2 (`0x0183BD53` libSceFios2.prx)

### Statut
Accepté & Implémenté.

### Contexte & Problème
Lors d'une requête de présence de fichier sans métadonnées (`stat buffer == NULL`), `libSceFios2.prx` appelait aveuglément `memset(NULL, 0, 80)` suivi d'écritures directes dans `*(NULL)`.

### Décision
* Trampoline de 36 octets à `0x0185D0C0` : si `rbx == NULL`, assigne `r15d = 0` et saute directement à l'épilogue de la fonction (`0x0183BDD7`).

---

## ADR-016 : Sécurisation du Pointeur Périphérique FIOS2 (`0x0184C610` libSceFios2.prx)

### Statut
Accepté & Implémenté.

### Contexte & Problème
Lors de l'ordonnancement des requêtes d'E/S dans `libSceFios2.prx`, la structure périphérique pouvait être nulle, déclenchant une violation d'accès sur l'indexation de sa vtable.

### Décision
* Trampoline de 21 octets à `0x01860C98` : si `rdi == NULL`, retourne 0 immédiatement.

---

## ADR-017 : Initialisation Stub des Particules (`0x0050E150` Renderer::InitParticles)

### Statut
Accepté & Implémenté.

### Contexte & Problème
La routine de rendu des particules `0x0050E150` tentait d'allouer des buffers d'instances GPU avant la compilation complète des pipelines graphiques, provoquant une boucle d'assertions et un crash d'allocation.

### Décision
* Patch de 4 octets à `0x0050E150` : `31 c0 c3 90` (`xorl %eax, %eax ; retq ; nop`).

---

## ADR-018 : Protection du Déréférencement Pointeur Décor (`0x0055524B`)

### Statut
Accepté & Implémenté.

### Contexte & Problème
À `0x0055524B`, l'instruction `movl (%rdi), %eax` était exécutée sur un pointeur d'instance NULL lors de l'énumération des nœuds de scène.

### Décision
* Trampoline de 30 octets à `0x00DE7E00` : vérifie `rdi != NULL` ; si NULL, saute à `0x0055527E`.

---

## ADR-019 : Protection des Buffers de Transformation Matrice Audio (`0x0013D850`)

### Statut
Accepté & Implémenté.

### Contexte & Problème
La routine de transformation et mixage matriciel audio à `0x0013D850` reçoit `rdi = dst`, `rsi = src`, `edx = count`. Lorsque les canaux audio ne sont pas alloués, les appelants passent `rdi = NULL` et/ou `rsi = NULL`. Les deux boucles AVX (`0x0013D878` et `0x0013D960`) déréférençaient ces registres (`vmulps (%rdi,%rcx,4)` et `vaddps (%rsi,%rcx,4)`), provoquant des crashs à `0x0013D8C8` et `0x0013D978`.

### Décision
* Hook à l'entrée de fonction `0x0013D850` avec un trampoline de 28 octets à `0x00DE7E20` :
  `testq %rdi, %rdi ; jz ret ; testq %rsi, %rsi ; jz ret ; testl %edx, %edx ; jz ret ; vxorps ... ; vucomiss ... ; jmp 0x0013D858 ; retq`
* **Résultat** : Protection unifiée de toutes les boucles internes de mixage audio contre les accès nuls.

---

## ADR-020 : Protection Arène Sous-Allocateur Audio (`0x0013D180`)

### Statut
Accepté & Implémenté.

### Contexte & Problème
À `0x0013D186`, le sous-allocateur d'arène audio lisait `0x28(%rdi)` sans vérifier `rdi`, provoquant une violation d'accès en lecture à `0x28`.

### Décision
* Trampoline de 22 octets à `0x00DE7E40` : si `rdi == NULL`, retourne NULL (`xorl %eax, %eax ; retq`).

---

## ADR-021 : Initialisation Sécurisée Descripteur Orbis Object (`0x00C0EE01`)

### Statut
Accepté & Implémenté.

### Contexte & Problème
À `0x00C0EE11`, la fonction lisait `0x30(%rbp)` et copiait aveuglément 0x38 octets de descripteur. Lorsque le pointeur source valait NULL, cela déclenchait une violation d'accès à l'offset `0x30`.

### Décision
* Trampoline de 33 octets à `0x00FE2C00` : vérifie `rax != NULL && *rax != NULL`. Si NULL, bifurque directement vers le chemin d'initialisation de repli à `0x00C0EE9A`.

---

## ADR-022 : Filtrage Pointeur Nœud Mixeur Audio Invalide (`0x001E5180`)

### Statut
Accepté & Implémenté.

### Contexte & Problème
À `0x001E51E0`, lecture de `0x10(%rdi)`. Les appelants (ex: `0x001CFF77`) calculaient `rdi = *(r15 + 0x20) - (-0x80)`. Quand le buffer de voix est NULL, ce calcul produit `rdi = 0x80`, provoquant une violation d'accès en lecture à `0x90` (`0x80 + 0x10`).

### Décision
* Trampoline de 24 octets à `0x00FE2C30` : `cmpq $0x1000, %rdi ; ja continue ; retq`.

---

## ADR-023 : Sécurisation du Memset Audio AVX (`0x0013D270`)

### Statut
Accepté & Implémenté.

### Contexte & Problème
La routine de mise à zéro AVX `0x0013D270` recevait un pointeur de destination `rdi == NULL`, provoquant une violation d'accès en écriture à l'adresse 0 (`0x0013D2A0`).

### Décision
* Trampoline de 19 octets à `0x00FE2C50` : `testq %rdi, %rdi ; jnz continue ; retq`.

---

## ADR-024 : Protection des Voix Audio Non Allouées (`0x001D4330`)

### Statut
Accepté & Implémenté.

### Contexte & Problème
À `0x001D4330`, déréférencement aveugle du buffer de voix `*(rdi + 0x20)` sans validation préalable.

### Décision
* Trampoline de 42 octets à `0x00FE2C70` : vérifie `rdi != NULL && *(rdi + 0x20) != NULL` avant de poursuivre l'exécution.

---

## ADR-025 : Protection Nœud Matrice Audio Sibling (`0x001E1DD0`)

### Statut
Accepté & Implémenté.

### Contexte & Problème
À `0x001E1E32`, déréférencement de `0x10(%rdi)` sur un pointeur calculé à `0x80`.

### Décision
* Trampoline de 32 octets à `0x00FE2CB0` : `cmpq $0x1000, %rdi ; ja continue ; retq`.

---

## ADR-026 : Saut de Transformation Voix Nulle (`0x0013B093`)

### Statut
Accepté & Implémenté.

### Contexte & Problème
À `0x0013B097`, lecture de `0x4(*(r14 + 0x38))` qui provoquait une violation d'accès en lecture à `0x4` quand le pointeur interne était nul.

### Décision
* Trampoline de 35 octets à `0x00FE2CD0` : vérifie `*(r14 + 0x38)` ; si NULL, sauvegarde `r15` dans `-0x148(%rbp)` et saute directement à `0x0013B201`.

---

## ADR-027 : Protection du Nettoyage de Buffer Audio (`0x0013B910`)

### Statut
Accepté & Implémenté.

### Contexte & Problème
À `0x0013B914`, lecture de `0x10(*(rdi + 0x38))` provoquant une violation d'accès à `0x10`.

### Décision
* Trampoline de 42 octets à `0x00FE2D00` : vérifie `rdi != NULL && *(rdi + 0x38) != NULL` avant d'exécuter le clear de buffer.

---

## ADR-028 : Protection Pré-Mixeur de Voix Audio (`0x001D3864`)

### Statut
Accepté & Implémenté.

### Contexte & Problème
À `0x001D3864`, le pré-mixeur déréférençait `rbx = *(rdi + 0x20)` sans tester si le buffer était nul.

### Décision
* Trampoline de 25 octets à `0x00FE2D30` : si `rbx == NULL`, saute directement à l'épilogue de nettoyage à `0x001D3954`.

---

## ADR-029 : Protection Filtre Audio (`0x001EB0C0`)

### Statut
Accepté & Implémenté.

### Contexte & Problème
À `0x001EB0D9`, lecture de `0x154(%rdi)` avec `rdi = 0xc0` (arithmétique d'offset sur pointeur NULL).

### Décision
* Trampoline de 25 octets à `0x00FE2D60` : `cmpq $0x1000, %rdi ; ja continue ; retq`.

---

## ADR-030 : Protection Filtre Audio Sibling (`0x001E0F30`)

### Statut
Accepté & Implémenté.

### Contexte & Problème
Identique à ADR-029 sur la routine jumelle `0x001E0F30`.

### Décision
* Trampoline de 25 octets à `0x00FE2D90` : `cmpq $0x1000, %rdi ; ja continue ; retq`.

---

## ADR-031 : Protection de Copie de Descripteur Render Target (`0x00C0047A`)

### Statut
Accepté & Implémenté.

### Contexte & Problème
À `0x00C0047D`, le moteur copiait 0x38 octets depuis `*(rbx)`. Quand `*(rbx) == NULL`, l'instruction `movl 0x30(%rax), %ecx` échouait avec une violation d'accès à `0x30`.

### Décision
* Trampoline de 96 octets à `0x00FE2DC0` : si `*(rbx) == NULL`, initialise à zéro les 0x38 octets de `rsi` via `vxorps` et saute à `0x00C004B1`.

---

## ADR-032 : Protection Nœud de Voix Audio Invalide (`0x00133700`)

### Statut
Accepté & Implémenté.

### Contexte & Problème
À `0x00133704`, l'instruction `movq 0x10(%rdi), %rax` déréférençait `rdi`. Lors de l'initialisation du pipeline audio, des appelants passaient `rdi = 0x80`, provoquant une violation d'accès en lecture à `0x90`.

### Décision
* Trampoline de 23 octets à `0x00FE2E30` :
  `cmpq $0x1000, %rdi ; ja .valid ; retq ; .valid: pushq %rbp ; pushq %r14 ; pushq %rbx ; movq 0x10(%rdi), %rax ; jmp 0x00133708`
  et hook à `0x00133700` (`jmp 0x00FE2E30 ; 3x nop`).
* **Résultat** : Le pipeline audio (`brbDriver`, WASAPI audio ports 0, 9, 17, ATRAC9 Ajm context) s'initialise entièrement sans crash et l'exécutable tourne de manière continue et stable.

---

## ADR-033 : Neutralisation de la Boucle d'Attente Synchrone Audio `brb_StartSession` (`0x0012E890`)

### Statut
Accepté & Implémenté.

### Contexte & Problème
À `0x0012E88D`, la fonction `brb_StartSession` vérifiait `cmpl 0x10(%rcx), %eax` suivi de `je 0x12e8ec` (`74 5A`). En cas de non-concordance immédiate des statuts d'état audio, la fonction bouclait jusqu'à 1000 fois avec des `sceKernelUsleep(10000)`, gelant le thread de chargement pendant 10 secondes entières avant de continuer.

### Décision
* Remplacement de `je 0x12e8ec` par un saut inconditionnel `jmp 0x12e8ec` (`EB 5A`) à `0x0012E890`.
* **Résultat** : L'accusé de réception audio est immédiat (`brb_StartSession(): OK`), économisant 10 secondes de blocage au démarrage sans perturber le driver WASAPI.

---

## ADR-034 : Déblocage de la Barrière PlayGo dans `GameLoadingMountInitialArchives` (`0x00B77C77`)

### Statut
Accepté & Implémenté.

### Contexte & Problème
À `0x00B77C6B`, `GameLoadingMountInitialArchives` interrogeait le compteur d'avancement PlayGo `*(int64_t *)(*(0x15af7d0) + 0x20)`. Si 0, l'instruction `je 0xb783e0` (`0F 84 63 07 00 00`) forçait une attente passive infinie avec des `Sleep(50ms)` en attendant que les chunks PS4 soient téléchargés en arrière-plan. Sur PC, l'intégralité des données du jeu est déjà présente sur disque local.

### Décision
* Remplacement du saut conditionnel de 6 octets à `0x00B77C77` par un NOP multi-octets (`66 0F 1F 44 00 00`).
* **Résultat** : Le moteur monte immédiatement les archives initiales et accède à la boucle de chargement des ressources.

---

## ADR-035 : Neutralisation de la Trappe d'Assertion `m_gfxEopTick` dans `FrameBegin` (`0x00BD440F`)

### Statut
Accepté & Implémenté.

### Contexte & Problème
Dans `FrameBegin` (`ndlib/render/fg-draw-mgr.cpp`), le moteur s'assurait que la trame en recyclage (3 frames en arrière) avait bien reçu son tick GPU : `cmpq $0, 0xa28(%rax,%rcx) ; jne 0xbd444c` (`75 3B`). Si 0, l'assertion échouait et exécutait une interruption `int $0x41` (trappe debug à `0x00BD444A`). Sur PC lors de la première compilation des shaders Vulkan, le pipeline peut accuser une latence temporaire.

### Décision
* Remplacement de `jne 0xbd444c` par `jmp 0xbd444c` (`EB 3B`) à `0x00BD440F`.
* **Résultat** : La progression des frames n'est plus interrompue par la latence initiale de compilation des shaders.

---

## ADR-036 : Protection Liste de Périphériques FIOS2 NULL dans la Résolution de Chemins (`0x0184BB9C`)

### Statut
Accepté & Implémenté.

### Contexte & Problème
Lors de la résolution de chemins pour des répertoires optionnels ou non existants (ex. dossiers DLC `%s/dc%u`), la liste des périphériques montés à `*(0x6c3c8)` dans `libSceFios2.prx` est vide (`rbx == NULL`). À `0x0184BB9C`, la fonction sautait sans vérification dans la boucle de correspondance (`jmp 0x184bca0`), laquelle déréférençait immédiatement `0xc0(%rbx)` à `0x0184BC8D`, provoquant une violation d'accès en lecture à `0x00000000000000c0` (thread 10428).

### Décision
* Remplacement du saut inconditionnel à `0x0184BB9C` par un saut vers la cave `0x0186DDF0` (`e9 4f 22 02 00`).
* Trampoline de 14 octets dans la cave `0x0186DDF0` :
  ```x86asm
  testq %rbx, %rbx
  je 0x0184BD0A       # Saut direct vers le repli sur périphérique par défaut
  jmp 0x0184BCA0      # Poursuite normale de la boucle
  ```
* **Résultat** : Élimination totale du crash `0x00000000000000c0`, la résolution FIOS2 retombe proprement sur le périphérique par défaut sans interrompre les threads de chargement.

---

## ADR-037 : Protection Pointeur de Filtre Virtuel FIOS2 Non Validé (`0x0184BBEB`)

### Statut
Accepté & Implémenté.

### Contexte & Problème
À `0x0184BBEB` dans `libSceFios2.prx`, la boucle de filtrage de périphérique exécute `callq *0x4118(%r12)`. Pour des chemins overlay ou synthétiques (ex: `/app0/u2data/data/fonts/ps4/*.fnt`), `*(r12 + 0x4118)` contenait un pointeur non mappé / non engagé en mémoire (`0x2f30b9a8`), déclenchant une violation d'accès immédiate sur le thread de chargement des polices.

### Décision
* Remplacement de l'instruction d'appel de 8 octets à `0x0184BBEB` par `jmp 0x0186DE10 ; 3x nop` (`e9 20 22 02 00 90 90 90`).
* Trampoline de 38 octets dans la cave `0x0186DE10` :
  ```x86asm
  movq 0x4118(%r12), %rax
  movq %rax, %r10
  subq *(0x0186DE50), %r10    # Soustrait la base d'image hôte (stockée à l'initialisation)
  cmpq $0x2000000, %r10       # Vérifie si le pointeur appartient au segment exécutable
  jae .skip                   # Hors limites : filtre contourné
  callq *%rax
  jmp .done
.skip:
  movb $1, %al                # Simule un filtre concordant
.done:
  jmp 0x0184BBF3              # Reprise sur testb %al, %al sans corrompre les instructions
  ```
* **Résultat** : Les vérifications des polices et overlays franchissent le filtre sans plantage et accèdent directement à l'étape suivante de compilation des shaders.

---

## ADR-038 : Sécurisation du Compteur de Synchronisation Frame Job 2 (`0x003F68C0`)

### Statut
Accepté & Implémenté.

### Contexte & Problème
Dans le moteur Naughty Dog, `detour_Job2` intercepte la barrière de synchronisation de frame `Job2` (`0x003F68C0`). Le sync counter global (`0x1200105000` / `0x1200105180`) est utilisé par plusieurs jobs asynchrones. Auparavant, une synchronisation agressive décrémentait ou forçait le compteur pour tous les appelants, créant des désynchronisations prématurées entre `UCCApp` et les passes de rendu.

### Décision
* Restriction stricte du déclenchement du forçage de synchronisation dans `detour_Job2` : l'ajustement ne s'applique que lorsque `caller_offset == 0x003F43BE` (boucle principale de la frame), préservant ainsi les compteurs des autres jobs parallèles.
* **Résultat** : La séquence de synchronisation de la frame `AppMainLoop` (Sync #1 à #12) se déroule dans l'ordre attendu sans blocage ni décalage de trame.

---

## ADR-039 : Neutralisation des Callbacks Indirects Audio Mixer Corrompus (`0x001377A6` & `0x00137874`)

### Statut
Accepté & Implémenté.

### Contexte & Problème
Dans la boucle de mixage audio DSP (`0x00137430`), exécutée en continu sur le thread `brbDriver`, deux appels indirects utilisent des pointeurs de rappel stockés dans la structure DSP de la voix (`-0x30(%rbp)`) :
1. `0x001377A6` : `callq *0x250(%rbx)`
2. `0x00137874` : `callq *0x258(%rax)`

Lors de l'initialisation du pipeline audio WASAPI sous Windows, `*0x250(%rbx)` pointe vers la zone de données constantes `0x00FE2534` qui contient `0xCC` (`int 3`), provoquant une interruption immédiate `EXCEPTION_BREAKPOINT` (`0x80000003`) non gérée qui terminait silencieusement le processus.
De même, `*0x258(%rax)` déclenchait une cascade de mixage DSP prématurée sur des canaux de voix encore incomplets.

### Décision
1. **Trampoline sécurisé à `0x00FE2E60` pour `0x001377A6`** :
   * Valide que `%rbx != NULL` et que `*0x250(%rbx)` se trouve dans la plage exécutable valide `[0x10000, 0x00FE0000)`.
   * Si l'adresse est invalide (ex: pointeur vers cave `0x00FE2534`), l'appel est ignoré en toute sécurité et l'exécution reprend à `0x001377AC`.
2. **Contournement sécurisé à `0x00137874`** :
   * Remplacement de l'instruction de 6 octets `callq *0x258(%rax)` par 6 NOPs (`90 90 90 90 90 90`).
   * La restauration de `%rdx` (`movq %rbx, %rdx` à `0x0013787A`) est préservée intacte.
* **Résultat** : Le crash `0x80000003` sur `brbDriver` à `0x00FE2534` est éliminé. Le thread audio s'exécute de façon fluide à travers l'ensemble des voies de mixage.

---

## ADR-040 : Neutralisation du Validateur Vectoriel de Paramètres Audio (`0x0013C740`)

### Statut
Accepté & Implémenté.

### Contexte & Problème
La routine `0x0013C740` effectue une vérification d'assertion/clamp sur le vecteur flottant à 4 composantes `%xmm0` (bornes `%xmm1` et `%xmm2`), et invoque la routine de log `0x12D570` en cas de dépassement. Elle restaure le vecteur d'origine depuis `-0xD0(%rbp)` sans modifier l'état du moteur. Lors de l'itération des canaux voix audio, l'unwinding de pile et le dépassement de l'index de boucle provoquaient une sortie de pile et une violation d'accès en lecture à `0x0013C798`.

### Décision
* Remplacement du premier octet de `0x0013C740` par `retq` (`0xC3`).
* **Résultat** : `%xmm0` et tous les registres callee-saved sont préservés, éliminant tout débordement de pile lors du traitement des voix.

---

## ADR-041 : Architecture de Découplage et Validation Modulaire des Détours C Natifs

### Statut
Accepté & Documenté.

### Contexte & Problème
Plus de 930 fonctions décompilées en C natif sont injectées lors de l'initialisation (`src/runtime_u2_script.c`). Auparavant, le commutateur global `BB_U2_DISABLE_DETOURS=1` désactivait indifféremment les detours applicatifs ET l'infrastructure vitale (Gfx sync, Job workers, State machine, patchs mémoire ADR). Cela empêchait d'exécuter l'ELF PS4 d'origine dans des conditions saines et d'isoler unitaire les régressions introduites par les réécritures en C.

### Décision
* Découplage strict entre la **Couche 1 : Infrastructure Vitale** (permanente et inconditionnelle) et la **Couche 2 : Détours Applicatifs Modulaires** (`player`, `npc`, `scene`, `event`, `audio`).
* Mise en place d'un protocole de validation à 4 paliers (Palier 0 : Baseline ELF pur -> Palier 1 : Par sous-système -> Palier 2 : Dichotomie chirurgicale -> Palier 3 : Traçage télémétrique).
* Spécification complète et protocole détaillés dans [`docs/DETOUR_VALIDATION.md`](docs/DETOUR_VALIDATION.md).

