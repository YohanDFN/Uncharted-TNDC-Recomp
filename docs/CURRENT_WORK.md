# Suivi courant

- **Cible actuelle** : Chantier A3 validé (natif + hybride `bb-probe.exe`) — constantes exactes `MenuDrawer::DrawBox` (`SDL_RenderGeometry` noir pur avec fondus `24.0f`/`40.0f` et alpha `0.65f`), titre blanc pur `1.25×`, copyright `0.65×` ancré à `642.0f` et ombres `3.0f`/`4.0f` alpha `1.0f`.
- **État validé** :
  - `src/u2_engine/ndlib/render/ngen/menu_preview.{h,cpp}`, `src/u2_standalone/title_preview.h` et `src/u2_engine/u2_engine_tests.cpp` compilés (`out/uncharted2-native.exe` SHA-256 `a0533eb5...`).
  - Natif `scratch/run_native_test.ps1 6` : **653/653** assertions `EngineTest` passées, sortie `0`.
  - Hybride `scratch/run_test.ps1 6` (`bb-probe.exe`) : SelfTest **886**, full-body **831**, EngineTest **659/659**, `STOPPED_AFTER_TIMEOUT` sans crash ni Guest fault.
  - Captures visuelles réelles inspectées (`native-menu-root-a3.png` et `native-menu-profile-a3.png`), galerie HTML autonome et `provenance.json` mis à jour.
- **Preuves clés** :
  - `MenuDrawer::DrawBox` (`0x005B8710`, `0x00E61D18–0x00E61D54`) : noir pur `(0,0,0)` avec fondus `24.0f` / `40.0f`, alpha `0.65f`, sans bordure rectangle.
  - `MenuPage::Draw` (`0x005A4B3F`, `0x005A4250`) : titre blanc `1.25×` (`0x8736FC45`), copyright `0.65×` ancré à `642.0f`.
  - `0x0059DA46` / `0x0057A9B7` : ombres `3.0f` / `4.0f`, alpha `1.0f`.
- **Incertitude / Limite** :
  - Les sous-pages dynamiques non portées (`PAGE_CONTROLS`, `LIST_MEDALS`, etc.) et la palette exacte par entrée de `menu.bin` (`MenuStyle`) restent à étendre si besoin.
- **Prochaine expérience** :
  - Prochaine étape au choix : rendu du temple (matériaux/shaders) ou transitions animées/sous-pages dynamiques du menu.
- **Travail non enregistré** :
  - `.agents/`, `scratch/diag_baseline_3d5dded/`, `docs/preuves_avancement/2026-11-12/temple-samplers-capture.txt`, sous-module `gpu/third_party/fsr-vulkan` (à préserver, ne jamais committer).
