# Etat du travail courant

- **Cible active** : Menu titre natif et rendu du temple (`dev-native`) — **Chantier A4 terminé** (`MenuStyle`, palette `0x005796B0`, courbes d'easing `0x005C3C00` et transitions fluides `0.2s`).
- **Etat valide** :
  - `PowerShell -ExecutionPolicy Bypass -File scratch/build_msys2.ps1` : OK (`bb-probe.exe` et `uncharted2-native.exe`).
  - `PowerShell -ExecutionPolicy Bypass -File scratch/run_native_test.ps1 6` : **667/667** assertions C++23 (`EngineTest`), code de sortie **0**.
  - `PowerShell -ExecutionPolicy Bypass -File scratch/run_test.ps1 6` (`bb-probe.exe`) : SelfTest **886**, full-body **831**, EngineTest **673/673**, `STOPPED_AFTER_TIMEOUT`, aucun crash ni Guest fault.
  - Preuves visuelles `BB_NATIVE_CAPTURE` (`ROOT` & `PROFILE`) inspectées et archivées dans [preuves_avancement.html](file:///c:/Users/Yohan/Desktop/uncharted_pc-0.3.3/docs/preuves_avancement/2026-11-14/preuves_avancement.html) et [PREUVES_AVANCEMENT.md](file:///c:/Users/Yohan/Desktop/uncharted_pc-0.3.3/docs/PREUVES_AVANCEMENT.md).
- **Incertitude / limite** :
  - Les polices d'interface et la mise en page du menu utilisent encore le rendu SDL 2D (`title_preview.h`) plutôt que le pipeline de rendu 3D/UI complet ; les sous-menus dynamiques (`PAGE_CONTROLS`, listes dynamiques) et le chargement de niveau après sélection restent différés.
- **Prochaine experience** :
  - Poursuivre le port natif sur le rendu du temple (`menu-temple` : shaders/samplers/textures complètes) ou sur le câblage d'une action différée (options / chapitres) selon la priorité utilisateur.
- **Travail non enregistre** :
  - Fichiers non suivis préservés : `.agents/`, `scratch/diag_baseline_3d5dded/`, `docs/preuves_avancement/2026-11-12/temple-samplers-capture.txt`, sous-module `gpu/third_party/fsr-vulkan`.
