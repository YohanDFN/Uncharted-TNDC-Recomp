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
