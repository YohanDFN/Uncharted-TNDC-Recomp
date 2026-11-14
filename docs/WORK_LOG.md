# Journal cumulatif des suivis

Chaque remplacement de CURRENT_WORK ajoute son contenu intégral, daté en UTC
et identifié par SHA-256. Ne pas réécrire les entrées historiques.
Consulter par recherche ciblée. Le premier CURRENT_WORK est versionné ;
la première rotation en conservera automatiquement le contenu ici.


---

## 2026-11-12T21:59:40.142326+00:00

SHA-256: `5de0da14dc36f62294bc95d1d309ee3ea48b8b0c61b61b40d7a2486dadf7d040`

# Travail courant

- Cible : Uncharted 2, branche dev-native ; code validé abfe63c.
- Natif : 60 images, 44 premiers mips BC1 décodés ; 100 références de matériaux associées, dont 59 avec pixels.
- Validation du code : natif 456/456, hybride 462/462, six secondes sans crash.
- Preuves : [matériaux/images](NATIVE_MATERIAL_IMAGE_BINDINGS.md), [chargement](NATIVE_TEXTURE_TABLE_LOADING.md).
- Capture sampler : hybride 45 secondes, 462/462, STOPPED_AFTER_TIMEOUT. Deux samplers présents avec la signature des colonnes ; shader préparé f1ef5c58, 10 images, 2 samplers.
- Trace non commitée : preuves_avancement/2026-11-12/temple-samplers-capture.txt. Raw0 : 00fff00000000400 / 00fff00000000200 ; raw1 commun : 4000000008f00000.
- Champs backend : Wrap XYZ, AnisoLinear min/mag, mip Linear ; anisotropie différente. Co-présence prouvée, association à l'instruction précise encore ouverte.
- Instrumentation retirée, rebuild propre réussi ; test après retrait et rapport sampler à terminer. Logs : user/diag_temple_sampler_*.
- Prochaine expérience : identifier l'utilisation des samplers dans le shader, puis interpolation UV et échantillonnage testés avant application au viewer.
- Préserver : sous-module gpu/third_party/fsr-vulkan modifié, .agents/, scratch/diag_baseline_3d5dded/.
- Travail actuel : instructions compactes et archivage du suivi ; aucun changement de comportement du jeu.

<!-- End of archived handoff -->


---

## 2026-11-12T22:21:26.765953+00:00

SHA-256: `ef9880df48be36cc7a4276baacd9a7bcf7946ca0bc531a8c99d30bcdf1bfbdbd`

# Travail courant

- Cible : Uncharted 2, branche dev-native ; code validé abfe63c.
- Natif : 60 images, 44 premiers mips BC1 décodés ; 100 références de matériaux associées, dont 59 avec pixels.
- Validation du code : natif 456/456, hybride 462/462, six secondes sans crash.
- Preuves : [matériaux/images](NATIVE_MATERIAL_IMAGE_BINDINGS.md), [chargement](NATIVE_TEXTURE_TABLE_LOADING.md).
- Capture sampler : hybride 45 secondes, 462/462, STOPPED_AFTER_TIMEOUT. Deux samplers présents avec la signature des colonnes ; shader préparé f1ef5c58, 10 images, 2 samplers.
- Trace non commitée : preuves_avancement/2026-11-12/temple-samplers-capture.txt. Raw0 : 00fff00000000400 / 00fff00000000200 ; raw1 commun : 4000000008f00000.
- Champs backend : Wrap XYZ, AnisoLinear min/mag, mip Linear ; anisotropie différente. Co-présence prouvée, association à l'instruction précise encore ouverte.
- Instrumentation retirée, rebuild propre réussi ; test après retrait et rapport sampler à terminer. Logs : user/diag_temple_sampler_*.
- Prochaine expérience : identifier l'utilisation des samplers dans le shader, puis interpolation UV et échantillonnage testés avant application au viewer.
- Préserver : sous-module gpu/third_party/fsr-vulkan modifié, .agents/, scratch/diag_baseline_3d5dded/.
- Travail actuel : instructions compactes et archivage du suivi ; aucun changement de comportement du jeu.

- Suivi : archivage testé (deux rotations, conservation exacte CRLF/LF, SHA-256, échec d'archive, entrée vide et verrou concurrent). Instructions optimisées appliquées.

<!-- End of archived handoff -->


---

## 2026-11-12T22:29:35.684934+00:00

SHA-256: `cc92eb64db9284ed6f99c0fd35f12f53e34c06b87a575c913b3b70e75e3bc3a9`

# Travail courant

- Cible : Uncharted 2, branche dev-native ; première carte de couleur appliquée au viewer natif. Base 338d654, build natif SHA-256 fb162bb13dc1f7f16f4208c61b43853b860cfe0fe1b2f48d532efe5e4e0bddde.
- État validé : sous-mesh 21 (matériau colonnes), carte BC1 64×64 réelle, UV corrigés en perspective, répétition et bilinéaire ; éclairage de prévisualisation en espace linéaire. Les autres meshes gardent leurs couleurs.
- Preuves originales : image comparée sur 5120 octets, UV sur 6648 octets ; shaders actifs f1ef5c58 / 50411a0b, carte primaire image1 / sampler0. Aucun diagnostic GPU conservé dans le code.
- Tests : build et déploiement réussis, natif 469/469 et sortie 0 après 6 s ; hybride 30 s, SelfTest 886, full-body 831, moteur 475/475, STOPPED_AFTER_TIMEOUT, sans crash ni nouvelle assertion.
- Rapport : [échantillonnage et limites](NATIVE_TEMPLE_COLOR_SAMPLING.md). [Capture réelle](preuves_avancement/2026-11-12/06-temple-pierre-texturee.png), galerie autonome et provenance mises à jour. Capture inspectée ; contrôle d'affichage HTML arrêté à la demande de l'utilisateur.
- Limites : premier mip seulement ; pas de LOD, anisotropie, mélange de cartes, matériau ou éclairage original. Packages Resolved, logins encore en attente. Zéro code PS4 exécuté en natif.
- Prochaine expérience : vérifier pixels résidents et trajet UV/carte/sampler du grand visage ; appliquer sa carte primaire si décodable, sinon établir le format manquant avant de le porter.
- Préserver : sous-module gpu/third_party/fsr-vulkan modifié, .agents/, scratch/diag_baseline_3d5dded/, trace antérieure temple-samplers-capture.txt non commitée.
- Journaux : user/diag_columns_final_{build,native,hybrid}.log, user/diag_columns_probe3.log ; captures brutes et cache shader conservés. Tentatives intermédiaires interrompues consignées dans le rapport.

<!-- End of archived handoff -->


---

## 2026-11-12T22:32:36.094032+00:00

SHA-256: `2104e3a36482324974c675ea73081b676f833bebc451a1f4921b0414df349bae`

# Travail courant

- Cible : Uncharted 2, branche dev-native ; première carte de couleur appliquée au viewer natif. Base 338d654, build natif SHA-256 fb162bb13dc1f7f16f4208c61b43853b860cfe0fe1b2f48d532efe5e4e0bddde.
- État validé : sous-mesh 21 (matériau colonnes), carte BC1 64×64 réelle, UV corrigés en perspective, répétition et bilinéaire ; éclairage de prévisualisation en espace linéaire. Les autres meshes gardent leurs couleurs.
- Preuves originales : image comparée sur 5120 octets, UV sur 6648 octets ; shaders actifs f1ef5c58 / 50411a0b, carte primaire image1 / sampler0. Aucun diagnostic GPU conservé dans le code.
- Tests : build et déploiement réussis, natif 469/469 et sortie 0 après 6 s ; hybride 30 s, SelfTest 886, full-body 831, moteur 475/475, STOPPED_AFTER_TIMEOUT, sans crash ni nouvelle assertion.
- Rapport : [échantillonnage et limites](NATIVE_TEMPLE_COLOR_SAMPLING.md). [Capture réelle](preuves_avancement/2026-11-12/06-temple-pierre-texturee.png), galerie autonome et provenance mises à jour. Capture inspectée ; contrôle d'affichage HTML arrêté à la demande de l'utilisateur.
- Limites : premier mip seulement ; pas de LOD, anisotropie, mélange de cartes, matériau ou éclairage original. Packages Resolved, logins encore en attente. Zéro code PS4 exécuté en natif.
- Direction planifiée : [attribution automatique par famille de matériaux](UNCHARTED_NATIVE_PLAN.md#attribution-automatique-materiaux), sans sélection manuelle des meshes ; jalon documentaire, pas encore implémenté.
- Prochaine expérience : inventorier les variantes du temple et les pixels disponibles, puis vérifier le trajet UV/carte/sampler du grand visage pour établir une famille réutilisable. Traiter les mips détaillés séparément de l'attribution.
- Préserver : sous-module gpu/third_party/fsr-vulkan modifié, .agents/, scratch/diag_baseline_3d5dded/, trace antérieure temple-samplers-capture.txt non commitée.
- Journaux : user/diag_columns_final_{build,native,hybrid}.log, user/diag_columns_probe3.log ; captures brutes et cache shader conservés. Tentatives intermédiaires interrompues consignées dans le rapport.

<!-- End of archived handoff -->


---

## 2026-11-12T22:49:09.943451+00:00

SHA-256: `f3fdf52e58bc43dfec424f91d62fadeac379fafb568ab181d1abb3b85b15c225`

# Travail courant

- Cible : Uncharted 2, branche dev-native ; première carte de couleur appliquée au viewer natif. Base 338d654, build natif SHA-256 fb162bb13dc1f7f16f4208c61b43853b860cfe0fe1b2f48d532efe5e4e0bddde.
- État validé : sous-mesh 21 (matériau colonnes), carte BC1 64×64 réelle, UV corrigés en perspective, répétition et bilinéaire ; éclairage de prévisualisation en espace linéaire. Les autres meshes gardent leurs couleurs.
- Preuves originales : image comparée sur 5120 octets, UV sur 6648 octets ; shaders actifs f1ef5c58 / 50411a0b, carte primaire image1 / sampler0. Aucun diagnostic GPU conservé dans le code.
- Tests : build et déploiement réussis, natif 469/469 et sortie 0 après 6 s ; hybride 30 s, SelfTest 886, full-body 831, moteur 475/475, STOPPED_AFTER_TIMEOUT, sans crash ni nouvelle assertion.
- Rapport : [échantillonnage et limites](NATIVE_TEMPLE_COLOR_SAMPLING.md). [Capture réelle](preuves_avancement/2026-11-12/06-temple-pierre-texturee.png), galerie autonome et provenance mises à jour. Capture inspectée ; contrôle d'affichage HTML arrêté à la demande de l'utilisateur.
- Limites : premier mip seulement ; pas de LOD, anisotropie, mélange de cartes, matériau ou éclairage original. Packages Resolved, logins encore en attente. Zéro code PS4 exécuté en natif.
- Direction planifiée : [stratégie globale par chaînes fonctionnelles réutilisables](UNCHARTED_NATIVE_PLAN.md#strategie-globale-port-natif), contrats vérifiés, familles et bilan de couverture. Première chaîne : package → ressources → scène statique → matériaux → rendu ; attribution automatique des matériaux incluse. Stratégie documentaire, pas encore implémentée.
- Prochaine expérience : inventorier les dépendances et contrats manquants de cette chaîne sur menu-temple, puis identifier un second décor disponible et son scénario original. Les variantes du temple et le trajet UV/carte/sampler du grand visage alimentent la généralisation des matériaux ; les mips détaillés restent un axe distinct.
- Préserver : sous-module gpu/third_party/fsr-vulkan modifié, .agents/, scratch/diag_baseline_3d5dded/, trace antérieure temple-samplers-capture.txt non commitée.
- Journaux : user/diag_columns_final_{build,native,hybrid}.log, user/diag_columns_probe3.log ; captures brutes et cache shader conservés. Tentatives intermédiaires interrompues consignées dans le rapport.

<!-- End of archived handoff -->


---

## 2026-11-12T23:11:07.214099+00:00

SHA-256: `48c2d265e18e20d1c99878a32d74e6c3dd1b6d4c2137ce9e1ba0f3d515fe6762`

# Travail courant

- Cible : Uncharted 2, branche dev-native ; itérations unitaires demandées, généralisation reportée. Carte de pierre 1024×1024 appliquée au sous-mesh 21. Base 30aff7a, SHA-256 natif 0ab848d5656ffb75a9c51de0e517d5448ccbeebafa8e7ac08f25b5ad67d00a50.
- État validé : ressource cooked exacte du matériau colonnes retrouvée dans chunk-21-tstrm, lecture d'une page et de sa seule chaîne BC1 ; UV et éclairage du viewer conservés. Le snapshot résident reste en 64×64 et les autres meshes gardent leurs couleurs.
- Preuves : chaîne 701440 octets identique à l'image GPU originale 1024×1024 Thin1DThin, SHA-256 b13de58bb674b994997db6f48f2ede2afa2075ba81b1058be40c0af40f96d0d8 ; mip4 de 2048 octets identique au premier mip résident 64×64.
- Tests : build et déploiement réussis ; natif 484/484, full-body 831, sortie 0 après 6 s ; hybride 30 s, SelfTest 886, full-body 831, 490/490, STOPPED_AFTER_TIMEOUT, sans crash, Guest fault ou nouvelle assertion. Original avant modification : 45 s, 475/475.
- Rapport : [carte détaillée et limites](NATIVE_TEMPLE_DETAILED_COLOR.md). [Capture réelle](preuves_avancement/2026-11-12/07-temple-pierre-1024.png) inspectée, galerie et provenance mises à jour. Aucun contrôle d'affichage HTML.
- Limites : chargement ponctuel du viewer, pas de politique de streaming, LOD ou anisotropie ; premier mip en bilinéaire, matériau original incomplet, aperçu statique CPU. Packages Resolved et logins encore en attente. Aucun nouveau détour, aucun code PS4 exécuté en natif.
- Prochaine expérience unitaire : vérifier la carte de couleur du grand visage, ses pixels disponibles et le trajet UV/sampler original ; l'appliquer à ce mesh seulement après preuve.
- Stratégies globales et familles de matériaux conservées dans le plan pour plus tard ; elles ne sont pas déclarées implémentées.
- Préserver : sous-module gpu/third_party/fsr-vulkan modifié, .agents/, scratch/diag_baseline_3d5dded/, trace antérieure temple-samplers-capture.txt non commitée.
- Journaux : user/diag_column_mips_build_validated.log, user/diag_column_mips_native_final.log, user/diag_column_mips_hybrid_final.log, user/diag_column_mips_original.log. Deux assertions initiales réfutent l'égalité entre nom de table relatif et nom cooked complet ; correction et test dédiés conservés dans le rapport.

<!-- End of archived handoff -->


---

## 2026-11-14T10:17:08.068618+00:00

SHA-256: `8e20c3e8543d8cb11a6abc5b7166f90f0c7319ba472db104dad6d40146e98e08`

# Travail courant

- Cible : Uncharted 2, branche dev-native ; itérations unitaires. Premier écran-titre visuel sur le temple, base ad88d7b ; SHA-256 natif 40c35ed58507b4de52e4bf8eb7a6add1d616578bc686f5fca50d4711ea194b22.
- État validé : logo original 2048×512 couleur BC1 + masque BC4 depuis menu.pak, invitation française et mentions relevées, affichage statique. Pierre 1024×1024 conservée ; autres meshes encore en couleurs de prévisualisation.
- Preuves : deux chaînes de 702464 octets identiques aux images GPU originales Thin1DThin. Référence visuelle hybride conservée du 11 novembre ; nouvelles tentatives de recapture originale noires/inexploitables, non utilisées comme preuve d'image.
- Tests : build/déploiement réussis ; natif 493/493, full-body 831, sortie 0 après 6 s ; hybride 30 s, SelfTest 886, full-body 831, 499/499, STOPPED_AFTER_TIMEOUT, sans crash, Guest fault ou nouvelle assertion. F1 masque/réaffiche le titre, testé 12 s et sortie 0.
- Rapport : [écran-titre et limites](NATIVE_TITLE_SCREEN_PREVIEW.md). [Capture native réelle](preuves_avancement/2026-11-13/01-ecran-titre-natif.png), inspectée ; galerie autonome et provenance enregistrées sans contrôle d'affichage HTML.
- Limites : police SDL et disposition provisoires, logo affiché avec inversion verticale de prévisualisation. Invitation statique : aucune transition/menu jouable. Pas de nouveau détour, de code PS4 exécuté en natif ou de promotion des packages à Ready.
- Prochaine expérience : relever dans bb-probe l'écran après X, ses contrôles et textes ; afficher cette liste vérifiée avant de porter la transition et les actions une à une. Ne pas réutiliser la liste manuelle du prototype comme preuve.
- Stratégies globales et familles conservées dans le plan pour plus tard ; streaming, LOD, éclairage et matériaux originaux restent incomplets.
- Préserver : sous-module gpu/third_party/fsr-vulkan modifié, .agents/, scratch/diag_baseline_3d5dded/, trace antérieure temple-samplers-capture.txt non commitée.
- Journaux : user/diag_title_validated_{build,native,hybrid}.log, user/diag_title_toggle_native.log, user/diag_menu_title_original.log. Premier rejet dû au budget VRAM du lecteur résident ; correction ciblée sans élargir son budget. Diagnostics temporaires retirés.

<!-- End of archived handoff -->


---

## 2026-11-14T11:24:49.937339+00:00

SHA-256: `aacf035f930809491b8249621b25ac88622e5eba7e193d10a8568c39aedb27a6`

# Travail courant

- Cible : Uncharted 2, branche `dev-native` ; itérations unitaires. Vérification binaire et intégration de la police réelle du menu (`main.fnt`), base `bdda25c` ; SHA-256 `out/uncharted2-native.exe` : `03baee316902ef316fabce183341ed75a9f4063f0bc8208129d0f95eb0dd7b24`.
- État validé : `main.fnt` (`Adelon-Serial DB`, **183** glyphes, **592** paires de crénage, atlas `main_00.tga` 256×128 BGRA32 packé B/G/R/A) rend l'invitation française, les mentions UTF-8 (`™`, `©`) et la barre d'état dans `title_preview.h` sur le décor temple + logo original 2048×512.
- Preuves : `GameInitInternal` (`0x003F68C0`) remplit `g_apFonts` (`0x0144E640`) avec `main.fnt` en slot 1 (`0x003F6D0C`) ; `MenuDrawer` (`0x005BC2D4`) résout `fontIndex = -1` à `1` via `0x01450D18` / `0x00CDC2A0` / `0x005BC516` / `0x00CC9AF0` (codes ≤ 14 sur `symbol3.fnt` via `0x005DBF18`). `game3.fnt` (`0x003FE000`) n'appartient qu'à la commande debug inutilisée `reloadfont` (`0x003F57D7`) et `game3_00.tga` est absent.
- Tests : `git diff --check` et build MSYS2 (`-Wall -Wextra -Werror`) réussis. Natif 6 s : **501/501**, full-body **831**, sortie **0** ; F1 (`hidden`/`visible`), sortie **0** ; hybride (`user/last_run.log`) : SelfTest **886**, full-body **831**, **507/507**, 889 détours, 0 crash ni Guest fault jusqu'à fermeture manuelle au-delà de 30 s.
- Rapport : [écran-titre et preuves `main.fnt`](NATIVE_TITLE_SCREEN_PREVIEW.md). [Capture native réelle](preuves_avancement/2026-11-13/title-screen-main-font-capture.png) (1920×1080, SHA-256 `d307c00b...`), inspectée ; [galerie HTML autonome](preuves_avancement/2026-11-13/preuves_avancement.html) (3 PNG vérifiés par script) et provenance à jour.
- Limites : disposition, tailles de ligne (`42/32/18`), bouton croix géométrique (`symbol3.fnt` non porté), ombre de texte, inversion verticale du logo et atténuation du décor restent des conventions du viewer. Invitation statique sans transition ni pipeline UI original.
- Prochaine expérience : relever dans `bb-probe` l'écran après l'appui sur X, ses contrôles et ses textes ; afficher cette liste vérifiée avant de porter la transition et les actions une à une.
- Préserver : sous-module `gpu/third_party/fsr-vulkan` modifié, `.agents/`, `scratch/diag_baseline_3d5dded/`, trace antérieure `temple-samplers-capture.txt` non commitée.

<!-- End of archived handoff -->


---

## 2026-11-14T11:54:46.453573+00:00

SHA-256: `9d90748c43ed7d18bae723bc0df5426b02de82e01446d68a323d79661d24262a`

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

<!-- End of archived handoff -->


---

## 2026-11-14T12:32:45.640617+00:00

SHA-256: `a1928b4b24f2b95b5817e752bbf876fed61c92a1e8343104c1019286148888c8`

# État courant

- **Branche** : `dev-native` (sur `01ddd0a` + machine à états interactive `MenuHost`).
- **Cible** : Menu principal natif dynamique conforme à `MenuHost` (`0x005CB990`) en C++23 sans exécution de code PS4.
- **État validé** :
  - `menu_preview.{h,cpp}` décode `target_page_name` (`+0x48`) / `target_page_id` (`+0x50`) dans `dc1/ps4/menu.bin`, charge `PAGE_INGAME_OPTIONS` (`0xB793B4B2`), `PAGE_BONUSES` (`0x22D667C0`), `PAGE_CHAPTERS` (`0xBE2C8B4B`) et `PAGE_MULTIMEDALS` (`0xACE1A9D5`), lit l'état profil/campagne hôte dans `SAVEDATA02P/USR-DATA` et `SAVEDATA020/USR-DATA` (`0xCD..0xD6` inclus), et exécute `U2NativeMenuStateMachine` (pile de pages, curseur par page, saut des séparateurs et `type != 2`, report explicite des actions gameplay et feuilles non portées).
  - `title_preview.h` / `engine_context.h` ouvrent sur `PAGE_ROOT_MAIN` (`0x37656388`) avec navigation clavier (`Haut`/`Bas`/`W`/`S`, `Entrée`/`X`/`Espace` lorsque visible, `Retour arrière`/`Échap` si `depth > 1`, bascule `F1`) et souris, plus capture différée `BB_NATIVE_CAPTURE` (`>= 1800 ms`).
  - Natif `scratch/run_native_test.ps1 6` (`606/606` dont Étape 20, sortie `0`) ; séquence interactive automatisée Win32 + capture live `docs/preuves_avancement/2026-11-13/03-menu-interactif-pile-pages.png` (1920×1080, inspectée visuellement et embarquée 5/5 dans `preuves_avancement.html`, sortie `0`) ; hybride `scratch/run_test.ps1 6` (`bb-probe.exe`, `user/last_run.log` : SelfTest `886`, full-body `831`, `612/612`, zéro crash ni Guest fault jusqu'à `STOPPED_AFTER_TIMEOUT`).
- **Preuves** : `docs/NATIVE_TITLE_SCREEN_PREVIEW.md`, `docs/PREUVES_AVANCEMENT.md`, `docs/preuves_avancement/2026-11-13/{provenance.json,preuves_avancement.html,03-menu-interactif-pile-pages.png}`.
- **Incertitude** : chargement effectif des niveaux/sauvegardes depuis les boutons gameplay différés (`Nouvelle partie`, `Continuer / Jouer`, `Charger partie`, `Commencer chapitre`) et persistance/UI des sous-pages d'options (`PAGE_CONTROLS`, `PAGE_VIDEO`, `PAGE_AUDIO`) et des listes dynamiques (`LIST_MEDALS`).
- **Prochaine expérience** : relier soit les sous-pages d'options (`PAGE_INGAME_OPTIONS`), soit l'action différée `Continuer / Jouer` au pipeline de chargement de niveau natif.
- **Travail non enregistré** : éléments environnementaux préservés uniquement (`.agents/`, sous-module `gpu/third_party/fsr-vulkan`, `scratch/diag_baseline_3d5dded/`, `docs/preuves_avancement/2026-11-12/temple-samplers-capture.txt`).

<!-- End of archived handoff -->
