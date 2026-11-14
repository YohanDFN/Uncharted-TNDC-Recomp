# Suivi courant — Port PC Uncharted 2

- **Branche active** : `dev-native`
- **Dernier jalon validé** : Chantier C2 — Décodage `ThinBGRA8` (`87` / `91`) dans `texture_preview.{h,cpp}` et `package_manager.cpp`, composition du canal `-trans.` sur l'alpha de `g_tColorMap` et découpe alpha-cutout (`alpha < 128`) dans `preview_depth.h` / `temple_preview.h` (**13/13** maillages `menu-temple` texturés, dont les lianes `Submesh26`).
- **État validé** :
  - Natif `scratch/run_native_test.ps1 6` : `EngineTest` **673/673**, code de sortie **0**, `textured meshes=13 depth-passing-samples=5803707`.
  - Hybride `scratch/run_test.ps1 6` (`bb-probe.exe`) : `SelfTest` **886**, `EngineTest` full-body **831**, assertions C++23 **679/679**, `STOPPED_AFTER_TIMEOUT`.
  - Preuve visuelle `docs/preuves_avancement/2026-11-14/native-menu-temple-c2.png` (SHA-256 `ae7edc6e...`) inspectée et archivée dans `docs/preuves_avancement/2026-11-14/preuves_avancement.html` (ordre croissant A2→C2), `provenance.json` et `docs/PREUVES_AVANCEMENT.md`.
- **Incertitude / limite restante** :
  - Le rendu 3D `menu-temple` reste un aperçu CPU statique (pas de normal maps, lightmaps, ombres portées ni caméra animée/post-traitement), et les actions `Continuer / Jouer` / persistance des options du menu restent différées.
- **Prochaine expérience** :
  - Chantier C3 (éclairage/lightmaps/normal maps ou rendu GPU Vulkan de `menu-temple`) ou Chantier B2 (persistance des options et branchement de `Continuer / Jouer` vers le chargement de niveau natif).
- **Travail non enregistré** :
  - Aucun (hors fichiers locaux préservés : `.agents/`, `scratch/diag_baseline_3d5dded/`, `docs/preuves_avancement/2026-11-12/temple-samplers-capture.txt`, sous-module `gpu/third_party/fsr-vulkan`).
