# Suivi courant

- **Cible actuelle** : Chantier A2 validé (natif + hybride `bb-probe.exe`) — géométrie 2D `1280×720` de `MenuPage` (`u2_native_ComputeMenuPageLayout`), masquage du logo hors `PAGE_ROOT_MAIN`, panneau centré `MenuDrawer::DrawBox` et invites `|X|` / `|C|` ancrées sous le panneau.
- **État validé** :
  - `src/u2_engine/ndlib/render/ngen/menu_preview.{h,cpp}`, `src/u2_standalone/title_preview.h` et `src/u2_engine/u2_engine_tests.cpp` compilés via `scratch/build_msys2.ps1` (`out/uncharted2-native.exe` SHA-256 `fb1f6feb...`).
  - Natif `scratch/run_native_test.ps1 6` : **650/650** assertions `EngineTest` passées, sortie `0`.
  - Hybride `scratch/run_test.ps1 6` (`bb-probe.exe`) : SelfTest **886**, full-body **831**, EngineTest **656/656**, `STOPPED_AFTER_TIMEOUT` sans crash ni Guest fault.
  - Captures visuelles réelles inspectées (`docs/preuves_avancement/2026-11-14/native-menu-root.png` et `native-menu-profile.png`), galerie HTML autonome et `provenance.json` générés, `docs/PREUVES_AVANCEMENT.md` mis à jour.
- **Preuves clés** :
  - `MenuHost::Draw` (`0x005D130D–0x005D1370`) : repère virtuel `1280×720`.
  - `MenuDrawer::BuildIcon` (`0x005BC6B6`) & `MenuPage::DrawSinglePage` (`0x005A611E`) : logo `1024×256` en `(128, 25)` uniquement sur `PAGE_ROOT_MAIN` (`0x37656388`), sauté sur `PAGE_PROFILE` et sous-pages.
  - `MenuPage::ComputeLayout` (`0x005A2670`), `MenuDrawer::DrawBox` (`0x005B8710`) & `MenuPage::DrawPrompts` (`0x005A50F2`).
- **Incertitude / Limite** :
  - Les couleurs/opacités exactes du panneau `DrawBox`, des ombres et des sous-pages dynamiques non portées restent des approximations du viewer.
- **Prochaine expérience** :
  - Étape suivante du menu fidèle ou rendu temple (`textures/matériaux`/`shaders` ou `U2NativeMenu` transitions/animations).
- **Travail non enregistré** :
  - `.agents/`, `scratch/diag_baseline_3d5dded/`, `docs/preuves_avancement/2026-11-12/temple-samplers-capture.txt`, sous-module `gpu/third_party/fsr-vulkan` (à préserver, ne jamais committer).
