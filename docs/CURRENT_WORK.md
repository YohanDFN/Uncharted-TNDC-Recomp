# Suivi courant — Port PC Uncharted 2

- **Branche active** : `dev-native`
- **Dernier jalon validé** : Chantier B1 — Décodage complet des 110 pages de `menu.bin`, navigation vers les sous-pages (`PAGE_GAMEPLAY`, `PAGE_CONTROLS`, `PAGE_VIDEO`, `PAGE_AUDIO`, `Bonus`, etc.), contrôles `type=0/3/5/6` (en-têtes, toggles, sliders, combos) pilotés au clavier (`Gauche`/`Droite`) et défilement vertical sur 10 lignes dans `DrawBox`.
- **État validé** :
  - Natif `scratch/run_native_test.ps1 6` : `EngineTest` **672/672**, code de sortie **0**.
  - Hybride `scratch/run_test.ps1 6` (`bb-probe.exe`) : `SelfTest` **886**, `EngineTest` full-body **831**, assertions C++23 **678/678**, `STOPPED_AFTER_TIMEOUT`.
  - Preuve visuelle `docs/preuves_avancement/2026-11-14/native-menu-controls-b1.png` (SHA-256 `6beb0ccd...`) inspectée et archivée dans `docs/preuves_avancement/2026-11-14/preuves_avancement.html` (ordre croissant A2→B1), `provenance.json` et `docs/PREUVES_AVANCEMENT.md`.
- **Incertitude / limite restante** :
  - Les valeurs d'options (`type=3/5/6`) sont maintenues en mémoire hôte pour la session de preview (pas encore persistées dans la sauvegarde ni appliquées au mixer audio/rendu 3D), et `Continuer / Jouer` reste différé.
- **Prochaine expérience** :
  - Chantier B2 (persistance/application réelle des options et branchement de `Continuer / Jouer` vers le pipeline de chargement de niveau natif) ou enrichissement 3D `menu-temple`.
- **Travail non enregistré** :
  - Aucun (hors fichiers locaux préservés : `.agents/`, `scratch/diag_baseline_3d5dded/`, `docs/preuves_avancement/2026-11-12/temple-samplers-capture.txt`, sous-module `gpu/third_party/fsr-vulkan`).
