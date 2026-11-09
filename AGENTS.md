# Directives de Développement & Protocole Rapide (Agent Antigravity)

Ce document résume le flux de travail optimisé pour aller droit au but sans perte de temps.

---

## 1. Règle d'or : Zéro Décompilation Massive à l'Aveugle
* **La baseline fonctionnelle de référence est `e80b482`**.
* Ne JAMAIS injecter de lots de fonctions sans tester immédiatement le jeu.
* L'ancien état (Lots 1 à 25) est archivé sur la branche distante `dev_archive_lots`. Il sert de référence de consultation.
* Chaque fonction C réécrite doit être testée et validée unitairement avec `run_test.ps1 6` avant tout commit.

---

## 2. Commandes d'Action Rapide

### Compilation (MSYS2 CLANG64)
```powershell
powershell -ExecutionPolicy Bypass -File scratch/build_msys2.ps1
```

### Test Runtime Immédiat (6 secondes)
> Nécessite `BypassSandbox: true` pour accéder aux fichiers du jeu dans `Desktop\Uncharted\CUSA02343`.
```powershell
powershell -ExecutionPolicy Bypass -File scratch/run_test.ps1 6
```

### Désassemblage d'une Routine ELF PS4
```powershell
node scratch/u2_disasm.js <offset_hex> <longueur_octets>
# Exemple: node scratch/u2_disasm.js 0x00A127D0 40
```

### Rapport de Statut des Fonctions DC
```powershell
node scratch/status_report.js
```

---

## 3. Protocole d'Intégration d'un Nouveau Détour
1. **Consulter l'archive ou désassembler** : Examiner l'implémentation originale dans `scratch/u2_disasm.js` ou dans `dev_archive_lots`.
2. **Implémenter dans le bon module (Séparation stricte Runtime Script C vs Moteur C++23)** :
   * **Sous-systèmes Moteur C++23 (`src/u2_engine/*.cpp`)** : Toute classe ou méthode C++ du moteur Naughty Dog (`CatmullRomSpline`, `SplineTracker`, `CameraZoom`, `RailVehicle`, `Turret`, `TankBoss`, etc.) doit être écrite proprement en **C++23** dans `src/u2_engine/` et enregistrée dans `src/u2_engine/u2_engine_registry.cpp` (module `"engine"`), **jamais** mélangée dans `src/runtime_u2_*.c`.
   * **Bindings Script DC (`src/runtime_u2_*.c`)** : Réservés exclusivement aux fonctions de script `Dc*` :
     * `src/runtime_u2_player.c` : Fonctions `Dc*` joueur, armes, mouvements.
     * `src/runtime_u2_npc.c` : Fonctions `Dc*` IA, ennemis, patrouilles.
     * `src/runtime_u2_scene.c` : Fonctions `Dc*` acteurs de scène, caméras, physique, décors.
     * `src/runtime_u2_event.c` : Fonctions `Dc*` événements, déclencheurs, cinématiques.
     * `src/runtime_u2_audio.c` : Fonctions `Dc*` audio, dialogues, effets sonores.
3. **Respecter l'ABI** : `ABI` (`__attribute__((sysv_abi))`) obligatoire sur tout point d'entrée appelé depuis l'ELF.
4. **Tests Unitaires Fonctionnels & Full-Body Sandbox (`src/u2_engine/u2_engine_tests.cpp`)** :
   * Toute nouvelle méthode C++ dans `src/u2_engine/` **DOIT** avoir ses assertions fonctionnelles dans `u2_run_engine_unit_tests()` (`src/u2_engine/u2_engine_tests.cpp`), appelée via le trampoline ELF (`image + offset`).
   * L'ensemble des 830+ détours `Dc*` et méthodes C++ est validé automatiquement en mode **Full-Body Sandbox** (`g_u2_image_base` pointant sur une image sandbox exécutable de 22 Mo avec `.rodata` réel et stubs SysV `xor eax, eax; xorps xmm0, xmm0; ret` sur `.text`) à chaque exécution de `run_test.ps1` (`BB_U2_SELFTEST=1`).
5. **Compiler** : `scratch/build_msys2.ps1`.
6. **Tester en direct** : `scratch/run_test.ps1 6` (Doit afficher `[U2 SelfTest] PASSED`, `[U2 EngineTest] PASSED` et `STOPPED_AFTER_TIMEOUT` sans crash).
7. **Committer** : Commit atomique ciblé sur `dev`.
