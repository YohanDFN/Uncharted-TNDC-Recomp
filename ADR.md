# Architecture Decision Records (ADR) - Uncharted PC Port & Recompilation

> **Directive obligatoire pour les agents & développeurs** : Ce fichier consigne l'historique architectural, les leçons apprises et les règles strictes d'intégration du projet. Il doit être **lu et appliqué scrupuleusement avant toute modification de code**.

---

## Sommaire des Décisions

* [ADR-001 : Restauration de la Baseline Fonctionnelle (`e80b482`) & Archivage des Lots Massifs](#adr-001--restauration-de-la-baseline-fonctionnelle-e80b482--archivage-des-lots-massifs)
* [ADR-002 : Protocole d'Intégration et Validation Unitaire des Détours C Natifs](#adr-002--protocole-dintégration-et-validation-unitaire-des-détours-c-natifs)
* [ADR-003 : Conventions d'Appel SysV ABI (`ABI`), Toolchain MSYS2 Clang64 & Isolation Hôte/Guest](#adr-003--conventions-dappel-sysv-abi-abi-toolchain-msys2-clang64--isolation-hôteguest)

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
