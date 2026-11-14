# État courant

- **Branche** : `dev-native` (sur `23bba9a` + Chantier A : habillage fidèle `MenuDrawer` et icônes `symbol3.fnt`).
- **Cible** : Habillage fidèle du menu principal natif (`MenuDrawer` + `symbol3.fnt`) sans exécution de code PS4.
- **État validé** :
  - `font_preview.{h,cpp}` décode `u2data/data/fonts/ps4/symbol3.fnt` (`packed=0`, 32 glyphes RGBA32 512×512 origine bas-gauche `tga[17]=0x08`) et substitue les balises `|ENTER|`, `|X|`, `|C|`, `|BACK|`, `|S|`, `|T|` (`Font::DrawText` `0x00CCB270–0x00CCB450`).
  - `menu_preview.{h,cpp}` décode `subtitle_string_name`/`id` (`+0x20/+0x28`, `MENU_LEGAL_COPYRIGHT` `0xC313D6F2`), `page_flags` (`+0x34`, bit `0x1` masquant les invites sur `PAGE_ROOT_MAIN`), et `layout_x`/`layout_y` (`+0x40/+0x44`).
  - `src/u2_standalone/{bitmap_font_preview,title_preview,engine_context}.h` rend `PAGE_ROOT_MAIN` épuré (logo + `"Appuyer sur [Croix bleue] pour commencer"` + mention légale enveloppée, sans boîte ni barre) et les sous-pages flottantes avec sélection dorée et invites `|X| SÉLECTIONNER` / `|C| RETOUR` en bas à droite (`BB_NATIVE_CAPTURE_PAGE=ROOT|PROFILE`).
  - Natif `scratch/run_native_test.ps1 6` (**643/643** dont Étape 21, code `0`), hybride `scratch/run_test.ps1 6` (`SelfTest 886`, `full-body 831`, `EngineTest 649/649`, `STOPPED_AFTER_TIMEOUT`), captures réelles `04-menu-fidele-ecran-titre-symbol3.png` et `05-menu-fidele-page-profil-symbol3.png` (1920×1080) inspectées et embarquées 7/7 dans `preuves_avancement.html`.
- **Preuves** : `docs/NATIVE_TITLE_SCREEN_PREVIEW.md`, `docs/PREUVES_AVANCEMENT.md`, `docs/preuves_avancement/2026-11-13/{provenance.json,preuves_avancement.html,04-menu-fidele-ecran-titre-symbol3.png,05-menu-fidele-page-profil-symbol3.png}`.
- **Incertitude** : placement pixel par pixel des textes/ombres sur SDL, pipeline shaders/foliage/éclairage du temple 3D, persistance des sous-pages d'options (`PAGE_CONTROLS`, `PAGE_VIDEO`, `PAGE_AUDIO`) et chargement de niveau depuis les actions différées (`Continuer / Jouer`, `Nouvelle partie`, `Charger partie`, `Commencer chapitre`).
- **Prochaine expérience** : porter les contrôles/valeurs d'options (`PAGE_INGAME_OPTIONS`) ou relier l'action différée `Continuer / Jouer` au chargement de niveau natif.
- **Travail non enregistré** : éléments environnementaux préservés uniquement (`.agents/`, sous-module `gpu/third_party/fsr-vulkan`, `scratch/diag_baseline_3d5dded/`, `docs/preuves_avancement/2026-11-12/temple-samplers-capture.txt`).
