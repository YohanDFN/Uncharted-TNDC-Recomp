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
pas récupéré des contrôles UI originaux. Invitation française et mentions
relevées sont affichées avec une police SDL provisoire, des équivalents ASCII
pour les symboles légaux et un bouton X dessiné. Ombre de texte et atténuation
du décor sont des conventions de lisibilité. Pas de nouvelle liste d'options,
de particules ou de dague inventée ajoutée au décor.

L'invitation est statique : Entrée, clic et X ne lancent aucune transition.
Le pied de page signale explicitement cette limite. F1 masque/réaffiche
l'écran-titre et rend l'aperçu précédent accessible ; Espace conserve les
vues géométriques et Échap ferme le viewer. Les callbacks du menu prototype
ne sont pas invoqués dans ce chemin. Aucun nouveau détour ni code PS4 exécuté.

## Validation et résultat

- `git diff --check`, build et déploiement MSYS2 réussis.
- Natif six secondes : **493/493**, full-body **831**, code **0**,
  logo BC1+BC4 2048×512 réellement chargé, capture réussie et inspectée.
- Hybride trente secondes : SelfTest **886**, full-body **831**, **499/499**,
  **STOPPED_AFTER_TIMEOUT**, aucun crash, Guest fault ou nouvelle assertion.
- Test natif de F1, douze secondes : masquage puis réaffichage observés dans
  le journal, **493/493**, code **0**. Messages de touches limités à la fenêtre
  `uncharted2-native.exe` du workspace.
- Neuf assertions ajoutées : palettes BC4, detiling, bords partiels, entrées
  invalides et absence de mutation, composition RGB/alpha, échec atomique,
  vraie ressource et transparence, absence de promotion du package.

[Capture native finale](preuves_avancement/2026-11-13/01-ecran-titre-natif.png),
[journal natif](preuves_avancement/2026-11-13/validation-titre-native.txt),
[journal hybride](preuves_avancement/2026-11-13/validation-titre-hybrid.txt),
[test F1](preuves_avancement/2026-11-13/validation-titre-f1.txt).
Build basé sur `ad88d7b` avec cette itération, SHA-256 natif :
`40c35ed58507b4de52e4bf8eb7a6add1d616578bc686f5fca50d4711ea194b22`.
Framebuffer SDL 1920×1080 converti de BMP en PNG sans retouche ; aucune image
générée. Galerie et provenance du dossier du 13 novembre mises à jour sans
contrôle de l'affichage HTML, à la demande de l'utilisateur.

Limites : premier écran statique sur un décor encore partiellement texturé,
police et disposition provisoires, pipeline UI, animations, localisation
et transitions originales non portés. Ce jalon ne rend pas le jeu jouable.

Prochaine expérience : relever dans l'original l'écran après l'appui sur X,
ses contrôles et ses textes, puis afficher cette liste vérifiée avant de
porter la transition et les actions une par une.
