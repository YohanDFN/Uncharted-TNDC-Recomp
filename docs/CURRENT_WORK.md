# Suivi courant — Port PC Uncharted 2

- **Cible & branche** : `dev-native` — Chantier C4 (`LIGHT_TABLE_1` : décodage des `874` light probes SH L2 de `menu-temple` et éclairage par pixel avec normal maps BC5).
- **État validé** :
  - Décodeur `u2_native_ReadLightTable` dans [level_resources.{h,cpp}](src/u2_engine/game/loading/level_resources.h) et [package_manager.{h,cpp}](src/u2_engine/ndlib/io/package_manager.cpp) (`LIGHT_TABLE_1 decoded=1 probes=874 dynamic=0`).
  - Éclairage SH L2 par pixel (`K=4` probes les plus proches en distance inverse, normales BC5, tone-mapping Reinhard) dans [temple_preview.h](src/u2_standalone/temple_preview.h) et [preview_depth.h](src/u2_standalone/preview_depth.h).
  - Build MSYS2 complet (`bb-probe.exe` + `uncharted2-native.exe`), `run_native_test.ps1 6` (sortie `0`, `EngineTest` `703/703`), `run_test.ps1 6` (`STOPPED_AFTER_TIMEOUT`, `SelfTest` `886`, full-body `831`, `EngineTest` `709/709`).
- **Preuves** :
  - Capture réelle `ROOT` (`1920×1080`) [native-menu-temple-c4.png](preuves_avancement/2026-11-14/native-menu-temple-c4.png) (SHA-256 `8d7557e0...`) inspectée et archivée dans [preuves_avancement.html](preuves_avancement/2026-11-14/preuves_avancement.html) (ordre croissant A2→C4), [provenance.json](preuves_avancement/2026-11-14/provenance.json) et [PREUVES_AVANCEMENT.md](PREUVES_AVANCEMENT.md).
- **Incertitude / limite restante** :
  - Le rendu 3D `menu-temple` reste une rastérisation CPU statique avec tone-mapping Reinhard de viewer (pas de lightmaps, ombres portées, caméra animée ni rendu GPU Vulkan / shaders PS4).
- **Prochaine expérience** :
  - Chantier C5 (migration du rendu `menu-temple` vers le pipeline GPU Vulkan pour le temps réel) ou Chantier B2 (persistance des options et branchement de `Continuer / Jouer` vers le chargement de niveau natif).
- **Travail non enregistré** :
  - Aucun (hors fichiers locaux préservés : `.agents/`, `scratch/diag_baseline_3d5dded/`, `docs/preuves_avancement/2026-11-12/temple-samplers-capture.txt`, sous-module `gpu/third_party/fsr-vulkan`).
