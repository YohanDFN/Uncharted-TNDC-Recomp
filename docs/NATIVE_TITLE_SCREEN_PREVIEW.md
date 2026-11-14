# Premier écran-titre natif — 13 novembre 2026

Hypothèse : le premier écran observé dans l'original peut être reproduit
visuellement sur le temple natif à partir de son logo et de son masque réels,
sans prétendre porter les contrôles ou les transitions du menu.
Critère : logo correctement orienté et transparent, invitation et mentions
reconnaissables, framebuffer capturé, tests sans régression.

Baseline : `ad88d7b`, temple avec carte de pierre 1024×1024, natif 484/484,
hybride 490/490. Le menu prototype existe mais ne sert pas de référence.

## Observation et références

La [capture hybride conservée du 11 novembre](preuves_avancement/2026-11-13/reference-titre-hybride-2026-11-11.png)
montre le logo Uncharted 2: Among Thieves, « Appuyer sur X pour commencer »
et les mentions légales. Sa provenance et sa validation visuelle sont décrites
dans [HYBRID_BOOT_RESTORE.md](HYBRID_BOOT_RESTORE.md). C'est l'écran-titre,
pas la liste d'options manuelle du prototype standalone.

Cette référence encore valide est réutilisée et copiée sans retouche. Les
nouvelles tentatives PrintWindow et dump GPU de cette session ont fourni des
images noires ou aucun framebuffer exploitable ; elles ne prouvent pas une
nouvelle capture de l'écran original et ne remplacent pas cette référence.
Leur cause n'est pas établie ; journaux et fichiers restent dans
`user/diag_menu_*`. La capture native finale, elle, a réussi et a été inspectée.

## Ressources originales

Dans `menu.pak`, le root et TEXTURE_TABLE sont retrouvés par les entrées du
header et la liste des logins déjà établies. Les deux ressources sélectionnées
ont les noms cooked complets suivants :

- `art/general/menu/logo2.tga/3DA08A9F6983BA12.ndb`, BC1 UNORM, format 71.
- `art/general/menu/logo2-alpha.tga/4C70F66839038231.ndb`, BC4 UNORM, format 80.

Les chemins complets, UID et offsets observés figurent dans la
[provenance des assets](preuves_avancement/2026-11-13/title-assets-provenance.json).
Chaque chaîne fait **702 464 octets**, 2048×512, 12 mips, une couche,
flags 01000000h, tag 3. Le tag seul ne prouve pas le tuilage.

Une exécution originale de 60 secondes avec `BB_IMAGE_LOG=1` donne deux images
GPU 2048×512, BC1 et BC4 UNORM, **Thin1DThin**, 12 niveaux, une couche et un
sample. Lecture bornée via OpenProcess QUERY_INFORMATION/VM_READ du seul
bb-probe du workspace, PID 28524 : région engagée et lisible, étendue entière
contenue, nombre d'octets lus contrôlé, aucune écriture guest.

Les deux chaînes correspondent intégralement aux octets du PAK :

| Plan | SHA-256 |
| --- | --- |
| Couleur | `0173da24ef461eac499a0cc4f031d96fc02a8369b344f7c88fa08d4c306e6cd9` |
| Masque | `7abbff48948699ad737428cf33ea0d93bff8f57a0a2bb36a1f08893b31f60f8d` |

[Comparaisons originales](preuves_avancement/2026-11-13/title-gpu-matches.json),
[couleur capturée](preuves_avancement/2026-11-13/title-color-gpu.bin) et
[masque capturé](preuves_avancement/2026-11-13/title-alpha-gpu.bin).
Plusieurs variantes localisées ont exactement les mêmes octets ; cette
comparaison ne permet pas d'identifier un nom de variante unique.
La paire générique utilisée en natif correspond aux deux payloads observés.
Elle ne constitue pas un port du shader UI original ou de la localisation.

Reproduction hors processus :

```powershell
C:/msys64/clang64/bin/python.exe scratch/verify_title_logo.py --game-dir C:/Users/Yohan/Desktop/Uncharted/CUSA02343 --output user/diag_title_assets_recheck.json
```

## Modification et conventions de prévisualisation

Le lecteur ciblé de PackageMgr contrôle les deux pages, logins et relocations
dans des copies hôtes possédées, puis les noms, UID, formats, dimensions,
couches, mips et étendues avant de lire seulement les deux chaînes. Aucun
offset de fichier constant, aucun pointeur dans une page libérée.
Les statuts du registre ne changent pas ; `menu` reste Resolved.

La banque VRAM du package dépasse le budget de 16 Mio du lecteur résident
existant. Une première tentative échouait donc proprement et gardait le décor.
Le nouveau lecteur contrôle la banque par ses pages et la taille déclarée,
sans l'allouer ; il ne lit que les deux images. Le budget du lecteur résident
commun n'a pas été élargi. Les diagnostics temporaires ont été retirés.

Le décodeur BC1 existant fournit les couleurs. Le décodeur BC4 UNORM ajouté
reproduit les deux palettes d'endpoints et le rangement Thin1D des blocs.
Le masque fournit l'alpha d'une image RGBA possédée, sans prémultiplication.
Les données décodées gardent leur orientation de stockage ; le viewer applique
une inversion verticale à l'affichage du logo pour retrouver la référence.
La première capture inversée a permis de vérifier cette différence et reste
dans `user/diag_title_final.bmp`.

Le logo est composé sur le temple existant. Le placement est mesuré
approximativement depuis la référence, dans l'espace 1920×1080 du viewer,
pas récupéré des contrôles UI originaux. Invitation française, mentions
légales UTF-8 (`™`, `©`) et barre d'état utilisent désormais la police bitmap
réelle **`main.fnt`** (`Adelon-Serial DB`, taille 42, `lineHeight=42`, `base=34`,
**183** glyphes, **592** paires de crénage) et son atlas `main_00.tga`
256×128 BGRA32 packé sur 4 plans (`chnl` 1/2/4/8), décodés et testés par
`src/u2_engine/ndlib/render/ngen/font_preview.{h,cpp}` et rendus via
`src/u2_standalone/bitmap_font_preview.h` / `title_preview.h`.

Preuve binaire (`out/big2/eboot.elf`) établissant `main.fnt` contre l'hypothèse
initiale :
- `GameInitInternal` (`0x003F68C0`, enregistré en `0x003F5833`) charge
  `g_apFonts` (`0x0144E640`) via `0x00CC1B90` (`"/app0/u2data/data/fonts/ps4/%s"`) :
  slot 0 `test2.fnt` (`0x003F6B97`, overlay FPS `0x003F900D`), slot 1
  **`main.fnt`** (`0x003F6D0C`), slot 2 `symbol3.fnt` (`0x003F6E81`, codes 1..32),
  slots 3..6 `japanese.fnt`, `korean.fnt`, `chinese.fnt`, `russian.fnt`.
- `0x003FE000` (qui référence `game3.fnt` en slot 1 à `0x003FE2EF`) n'a aucun
  appelant direct et n'est que le callback console `"reloadfont"` (`0x003F57D7`) ;
  de plus `game3_00.tga` est absent sur disque et des archives `menu.pak`/`bin.pak`.
- `MenuDrawer` initialise `fontIndex = -1` (`0x005BC2D4`). À l'affichage et
  à la mesure (`0x005BC516`, `0x005BE038`, `Font::DrawText` `0x00CC9A70` ->
  `0x00CC9AF0`), `-1` lit l'index de langue (`0x01450D18 + 0x604`) puis le slot
  `+0x0C` rempli à **`1`** (`main.fnt`) par `0x00CDC2A0` (`ENGLISH`, `FRENCH`,
  `ITALIAN`, etc.) ; les glyphes `<= 14` (`0x005DBF18`) basculent vers `symbol3.fnt`.

Distinction faits prouvés vs conventions du viewer : l'atlas `main.fnt`, son
décompactage 4 plans et les 592 crénages appartiennent aux données originales,
avec 0 glyphe manquant sur les chaînes de l'écran-titre. En revanche, les tailles
de ligne (`42/32/18`), l'icône cercle + croix dessinée géométriquement (au lieu
de `symbol3.fnt`), l'ombre de texte et l'atténuation du décor restent des
conventions du viewer. Pas de nouvelle liste d'options, de particules ou de
dague inventée ajoutée au décor.

L'invitation est statique : Entrée, clic et X ne lancent aucune transition.
Le pied de page signale explicitement cette limite. F1 masque/réaffiche
l'écran-titre et rend l'aperçu précédent accessible ; Espace conserve les
vues géométriques et Échap ferme le viewer. Les callbacks du menu prototype
ne sont pas invoqués dans ce chemin. Aucun nouveau détour ni code PS4 exécuté.

## Validation et résultat

- `git diff --check`, build et déploiement MSYS2 réussis (`-Wall -Wextra -Werror`).
- Natif six secondes (`scratch/run_native_test.ps1 6`) : **501/501** assertions
  C++23, full-body **831**, code **0**,
  `[U2 Native Font] main.fnt glyphs=183 kernings=592 line-height=42 base=34 atlas=256x128 packed B/G/R/A (proven slot 1 menu/UI font)`.
- Capture framebuffer native (`BB_NATIVE_CAPTURE` -> BMP 4 s converti en PNG
  sans retouche, code **0**) : [capture main.fnt](preuves_avancement/2026-11-13/title-screen-main-font-capture.png)
  (1920×1080, 2 070 791 octets, SHA-256 PNG `d307c00bafaa72bb160e99a1418b191f592aa8f14f781f2213eb7120cd34846a`),
  inspectée visuellement (glyphes français, `™`, `©` et barre d'état nets).
- Test natif de `F1` (budget 8 s, messages `VK_F1` ciblés sur le HWND du PID) :
  **501/501**, `[U2 Native Title] hidden` puis `visible`, code **0**.
- Hybride (`scratch/run_test.ps1 30`, `user/last_run.log`) : SelfTest **886**,
  full-body **831**, **507/507** assertions C++23, **889** détours installés,
  exécution au-delà de 30 s (`180 224` copies scalaires GPU) sans crash,
  Guest fault ni nouvelle assertion, arrêté proprement à la fermeture manuelle
  de la fenêtre (`Window closed`).
- 8 assertions ajoutées dans `u2_engine_tests.cpp` (Étape 18) : parsing synthétique
  BMFont/TGA + décompactage B/G/R/A, layout avec crénage et facteur d'échelle,
  préservation atomique sur erreur et chargement réel de `main.fnt` (183 glyphes,
  592 crénages, 256×512).

[Capture native précédente](preuves_avancement/2026-11-13/01-ecran-titre-natif.png),
[capture main.fnt](preuves_avancement/2026-11-13/title-screen-main-font-capture.png),
[galerie HTML autonome](preuves_avancement/2026-11-13/preuves_avancement.html),
[provenance.json](preuves_avancement/2026-11-13/provenance.json).
Build basé sur `bdda25c` avec cette itération, SHA-256 `out/uncharted2-native.exe` :
`03baee316902ef316fabce183341ed75a9f4063f0bc8208129d0f95eb0dd7b24`.
Intégration base64 des 3 PNG dans l'HTML vérifiée par script Python.

Limites : premier écran statique sur un décor encore partiellement texturé,
disposition/icône X/ombre provisoires, `symbol3.fnt`, pipeline UI, animations,
localisation et transitions originales non portés. Ce jalon ne rend pas le jeu jouable.

Prochaine expérience : relever dans l'original l'écran après l'appui sur X,
ses contrôles et ses textes, puis afficher cette liste vérifiée avant de
porter la transition et les actions une par une.
