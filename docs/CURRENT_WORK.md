# Suivi courant — Port PC Uncharted 2

- **Branche active** : `dev-native`
- **Dernier jalon validé** : Chantier C1 — Généralisation de la résolution `g_tColorMap` et de la mise à niveau `chunk-21-tstrm` à tous les sous-maillages BC1 de `menu-temple` dans `src/u2_standalone/temple_preview.h` (**12/13** maillages texturés, **11** mis à niveau jusqu'à `1024×1024`).
- **État validé** :
  - Natif `scratch/run_native_test.ps1 6` : `EngineTest` **672/672**, code de sortie **0**, `textured meshes=12 depth-passing-samples=5514708`.
  - Hybride `scratch/run_test.ps1 6` (`bb-probe.exe`) : `SelfTest` **886**, `EngineTest` full-body **831**, assertions C++23 **678/678**, `STOPPED_AFTER_TIMEOUT`.
  - Preuve visuelle `docs/preuves_avancement/2026-11-14/native-menu-temple-c1.png` (SHA-256 `72653247...`) inspectée et archivée dans `docs/preuves_avancement/2026-11-14/preuves_avancement.html` (ordre croissant A2→C1), `provenance.json` et `docs/PREUVES_AVANCEMENT.md`.
- **Incertitude / limite restante** :
  - Le 13e maillage (bannières verticales sur la droite) utilise une texture non BC1 (`compressed-only`) non décodée par `u2_native_DecodeThinBc1Preview`, et l'éclairage reste un Gouraud de prévisualisation sans normal maps ni post-traitement.
- **Prochaine expérience** :
  - Décoder le format de texture du dernier maillage de `menu-temple` (bannières, ex. BC3/alpha) ou passer au Chantier B2 (persistance/actions `Continuer / Jouer` du menu).
- **Travail non enregistré** :
  - Aucun (hors fichiers locaux préservés : `.agents/`, `scratch/diag_baseline_3d5dded/`, `docs/preuves_avancement/2026-11-12/temple-samplers-capture.txt`, sous-module `gpu/third_party/fsr-vulkan`).
