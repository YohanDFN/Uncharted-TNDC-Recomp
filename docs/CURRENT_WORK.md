# État courant

- **Branche** : `dev-native` (sur `deda6fc` + aperçu post-X `menu.bin` + `3.common`).
- **Cible** : Menu d'écran-titre et transition post-X natifs en C++23 sans exécution de code PS4.
- **État validé** :
  - `menu_preview.{h,cpp}` décode `dc1/ps4/menu.bin` et `core.psarc:text1/ps4/3.common` (entrée 46, `0x410000`, `87 097` octets non compressés `[0, 21561]`) en streaming borné (`PAGE_ROOT_MAIN` `0x37656388`, `PAGE_PROFILE` `0x82D5C00B` `DÉMARRER` 7 contrôles avec séparateur + `Continuer / Jouer` `16/0`, `PAGE_NEWPROFILE` `0x11FEFB39` 4 contrôles).
  - `title_preview.h` affiche les deux listes vérifiées sur le décor du temple avec `main.fnt` (`F1` bascule `hidden`/`visible`).
  - Natif `scratch/run_native_test.ps1 6` (`552/552` dont Étape 19, sortie `0`), capture 1920×1080 `docs/preuves_avancement/2026-11-13/02-menu-apres-x-verifie.png` inspectée et embarquée dans `preuves_avancement.html` (4/4 PNG vérifiés), hybride `bb-probe` (`user/last_run.log` : SelfTest `886`, full-body `831`, `558/558`, `77 824` copies scalaires GPU, sortie propre sur fermeture manuelle après 30 s).
- **Preuves** : `docs/NATIVE_TITLE_SCREEN_PREVIEW.md`, `docs/PREUVES_AVANCEMENT.md`, `docs/preuves_avancement/2026-11-13/{provenance.json,preuves_avancement.html,02-menu-apres-x-verifie.png}`.
- **Incertitude** : évaluation dynamique en live de l'état profil/sauvegarde (`0x005CB78D`–`0x005CB851`) et datasource `16/0` (`0x005ACAB0`) pour basculer entre `"Jouer"` et `"Continuer"`.
- **Prochaine expérience** : câbler la transition interactive `X` / `Entrée` depuis `PAGE_ROOT_MAIN` vers la page active (`PAGE_PROFILE` ou `PAGE_NEWPROFILE`), sélection au curseur et retour `O` / `Échap`.
- **Travail non enregistré** : aucun fichier de l'itération en attente après commit ; éléments environnementaux préservés (`.agents/`, sous-module `gpu/third_party/fsr-vulkan`, `scratch/diag_baseline_3d5dded/`, `docs/preuves_avancement/2026-11-12/temple-samplers-capture.txt`).
