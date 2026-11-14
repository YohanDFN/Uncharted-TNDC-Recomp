# Suivi courant — Port PC Uncharted 2

- **Branche active** : `dev-native`
- **Dernier jalon validé** : Chantier A5 — Décodage de `general/menu/edge-shadow.tga` + `edge-shadow-alpha.tga` (`menu.pak` `TEXTURE_TABLE`) et rendu des passes 2 et 3 de `MenuDrawer::DrawBox` (`0x005B8E43`, `0x005B8FD0`, teinte `0xF1AF1CF1`, UV `0.0140625f`) dans `src/u2_standalone/title_preview.h`.
- **État validé** :
  - Natif `scratch/run_native_test.ps1 6` : `EngineTest` **668/668**, code de sortie **0**.
  - Hybride `scratch/run_test.ps1 6` (`bb-probe.exe`) : `SelfTest` **886**, `EngineTest` full-body **831**, assertions C++23 **674/674**, `STOPPED_AFTER_TIMEOUT`.
  - Preuve visuelle `docs/preuves_avancement/2026-11-14/native-menu-profile-a5.png` (SHA-256 `0fd97bbb...`) inspectée et archivée dans `docs/preuves_avancement/2026-11-14/preuves_avancement.html` (ordre croissant A2→A5), `provenance.json` et `docs/PREUVES_AVANCEMENT.md`.
- **Incertitude / limite restante** :
  - L'éclairage et les matériaux complets de la scène 3D `menu-temple` restent partiels (preview CPU depth + colonne texturée) ; les interactions détaillées des sous-pages (ex. sélection de chapitre, options) restent à câbler.
- **Prochaine expérience** :
  - Poursuivre l'habillage et la navigation des sous-pages du menu natif (`PAGE_PROFILE` → sous-menus) ou enrichir le rendu `menu-temple` selon la priorité utilisateur.
- **Travail non enregistré** :
  - Aucun (hors fichiers locaux préservés : `.agents/`, `scratch/diag_baseline_3d5dded/`, `docs/preuves_avancement/2026-11-12/temple-samplers-capture.txt`, sous-module `gpu/third_party/fsr-vulkan`).
