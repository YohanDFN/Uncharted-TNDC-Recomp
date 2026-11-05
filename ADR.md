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
1. **Patch `0x00A17C60` (50 octets)** & Helper `u2_alloc_chunk_node_fallback` dans [`src/runtime_u2_script.c`](file:///c:/Users/Yohan/Desktop/uncharted_pc-0.3.3/src/runtime_u2_script.c) :
   Lorsque l'allocateur 0 échoue à fournir 216 octets pour un nœud de chunk, attribue dynamiquement un buffer de repli 16-octets aligné via `_aligned_malloc(216, 16)` mis à zéro au lieu de laisser `%r13` à NULL.
   En cas d'échec absolu, saute strictement à `0x00A17D62` (qui recharge `%r13 = 0x1044800` et `%r14 = 0x1534140`), empêchant le déréférencement de `0x298` (`0 + r12 * 8`) à `0x00A17B20`.
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
Lors de l'initialisation du moteur de script (`ScriptManager`), le balayage de la liste chaînée des chunks (`0x00A17B58`) lit les métadonnées (`0x98(%rcx)`) d'un nœud alloué dans la plage directe haute (`0x105d8f42b8`).

### Problème
Sur l'hôte Windows :
1. Les sections de mapping créées avec `SEC_RESERVE` ne peuvent pas être protégées via `VirtualProtect` (qui échoue avec `ERROR_INVALID_PARAMETER` / 87 ou `ERROR_INVALID_ADDRESS` / 487 sur les vues de sections).
2. Lorsque `VirtualAlloc(..., MEM_COMMIT)` initial dans `view_map` ne valide qu'une partie de la plage sous forte charge mémoire, les pages réservées restantes (`State = MEM_RESERVE`, `Protect = PAGE_NOACCESS`) déclenchent un crash :
   `Fault: read access to 000000105d8f42b8 at guest offset 0xa17b58`.
3. Le gestionnaire de fautes vectorisées (`vectored_fault` dans `src/probe.c`) n'intervenait que pour les écritures (`ExceptionInformation[0] == 1`), causant l'arrêt immédiat sur les lectures de mémoire valide du guest.

### Décision
* Dans [`src/probe.c`](file:///c:/Users/Yohan/Desktop/uncharted_pc-0.3.3/src/probe.c) (`vectored_fault`) :
  Pour tout accès fautif (lecture ou écriture) dans la plage mémoire guest `[0x1000000000, 0x2000000000)` :
  1. Invocation de `VirtualAlloc(page_addr, 0x10000, MEM_COMMIT, PAGE_READWRITE)` : valide et committe à la demande le bloc de 64 Ko sur la vue de section.
  2. Si `VirtualAlloc` réussit, reprise immédiate de l'instruction (`EXCEPTION_CONTINUE_EXECUTION`).
  3. Repli sur `VirtualProtect` si la page appartenait à une réservation standard.
* **Résultat** : La boucle de packages de `ScriptManager` progresse au-delà de `0x00A17B58` jusqu'à `0x00A17BEA` et les requêtes streaming de FIOS2.
   - Si valide, elle délègue à `Arena::Allocate(arena, size, 16, "UNKNOWN", 0, "UNKNOWN")`.
   - Si l'arène est NULL ou si l'allocation échoue, elle alloue dynamiquement via `_aligned_malloc(alloc_sz, 16)`, met à zéro la mémoire et la retourne proprement à l'appelant.

---

## ADR-010 : Demand-Commit Mémoire Guest (`0x1000000000`..`0x2000000000`) & Résolution Crash `0x00A17B58`

### Statut
Accepté & Implémenté.

### Contexte
Sur PS4, l'espace mémoire physique direct (1600 Mo à `0x1000000000`, 3100 Mo à `0x1100000000`) et flexible (335 Mo à `0x1200000000`) est alloué via `MapViewOfFile3` avec la section partagée de repli (`SEC_RESERVE`).
Lors de l'initialisation du moteur de script (`ScriptManager`), le balayage de la liste chaînée des chunks (`0x00A17B58`) lit les métadonnées (`0x98(%rcx)`) d'un nœud alloué dans la plage directe haute (`0x105d8f42b8`).

### Problème
Sur l'hôte Windows :
1. Les sections de mapping créées avec `SEC_RESERVE` ne peuvent pas être protégées via `VirtualProtect` (qui échoue avec `ERROR_INVALID_PARAMETER` / 87 ou `ERROR_INVALID_ADDRESS` / 487 sur les vues de sections).
2. Lorsque `VirtualAlloc(..., MEM_COMMIT)` initial dans `view_map` ne valide qu'une partie de la plage sous forte charge mémoire, les pages réservées restantes (`State = MEM_RESERVE`, `Protect = PAGE_NOACCESS`) déclenchent un crash :
   `Fault: read access to 000000105d8f42b8 at guest offset 0xa17b58`.
3. Le gestionnaire de fautes vectorisées (`vectored_fault` dans `src/probe.c`) n'intervenait que pour les écritures (`ExceptionInformation[0] == 1`), causant l'arrêt immédiat sur les lectures de mémoire valide du guest.

### Décision
* Dans [`src/probe.c`](file:///c:/Users/Yohan/Desktop/uncharted_pc-0.3.3/src/probe.c) (`vectored_fault`) :
  Pour tout accès fautif (lecture ou écriture) dans la plage mémoire guest `[0x1000000000, 0x2000000000)` :
  1. Invocation de `VirtualAlloc(page_addr, 0x10000, MEM_COMMIT, PAGE_READWRITE)` : valide et committe à la demande le bloc de 64 Ko sur la vue de section.
  2. Si `VirtualAlloc` réussit, reprise immédiate de l'instruction (`EXCEPTION_CONTINUE_EXECUTION`).
  3. Repli sur `VirtualProtect` si la page appartenait à une réservation standard.
* **Résultat** : La boucle de packages de `ScriptManager` progresse au-delà de `0x00A17B58` jusqu'à `0x00A17BEA` et les requêtes streaming de FIOS2.
