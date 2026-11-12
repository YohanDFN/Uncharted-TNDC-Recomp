# Carte de couleur de pierre 1024×1024 — 12 novembre 2026

Sous-système : lecture ciblée d'un dictionnaire de textures et viewer natif.
Hypothèse : le 64×64 de `menu-temple` est un mip résident d'une carte plus
détaillée accessible dans les données de streaming. Critère : retrouver cette
carte, la comparer à l'original puis afficher ses détails sur le même mesh.
Baseline réutilisée : code `9071a65`, 469 assertions natives / 475 hybrides,
et [capture 64×64](preuves_avancement/2026-11-12/06-temple-pierre-texturee.png).
Les commits suivants jusqu'à `30aff7a` sont documentaires.

## Ressource et comparaison originale

Le catalogue `u2data/build/main/texturedict1/ps4/TextureDictionaries.csv`,
ligne 1820, contient une entrée dont le chemin cooked correspond exactement
au paramètre g_tColorMap déjà validé du matériau 7. Il annonce `chunk-21`,
DXT1, 1024×1024, 11 mips et 699 064 octets logiques BC1.
Ce catalogue sélectionne l'archive à examiner ; il ne prouve pas son layout.

`chunk-21-tstrm.pak` possède une page non compressée de 660 352 octets à
fichier+DF60h. Le root est donné par le header ; son unique login référence
TEXTURE_DICTIONARY. Les arrays UID/noms/descripteurs se trouvent à metadata
+28h/+30h/+38h, et le compte à +20h. L'entrée 1242 donne le descripteur à
page+56DB0h, UID **685264F6h**, identique à l'image résidente.
Les offsets ici sont des résultats de l'inspection, pas des constantes du lecteur.

Le nom de table est relatif (`art/levels/...`) tandis que le descripteur
conserve le chemin cooked complet (`d:/dev/...`). La sélection native compare
le **nom cooked complet du descripteur**, sans retirer arbitrairement un préfixe.
La première implémentation confondait ces espaces de noms : deux assertions
réelles échouaient et le viewer gardait le 64×64. Cette hypothèse est réfutée
et un test spécifique couvre la différence des deux noms.

Le descripteur donne format 72, flags 010F0000h, une couche, tag 2,
11 mips, 1024×1024, offset VRAM 417B3200h et **701 440 octets stockés**.
La différence avec le CSV est expliquée par les dimensions de blocs arrondies
sur 8 dans chaque mip. Les quatre premiers niveaux occupent 524 288,
131 072, 32 768 et 8 192 octets. Le mip 4 commence donc à 696 320.
Le bloc VRAM débute à AF2E0h, fin de page contrôlée contre la taille header+20h
et la taille du fichier. Le login original `CBD9D0..CBD9F6` parcourt les
descripteurs de la liste +18h et ajoute une base au champ +8h ; le viewer
ne modifie pas ces structures et ne reproduit pas leur login moteur.

Une exécution originale de 45 secondes avec `BB_IMAGE_LOG=1` retrouve une
image **1024×1024 BC1 sRGB Thin1DThin**, 11 niveaux, une couche et un sample.
Une lecture Windows en accès QUERY_INFORMATION/VM_READ du seul bb-probe
du workspace compare 44 images BC1 candidates de 701 440 octets. Chaque
lecture vérifie une région engagée, lisible, sans garde, contenant toute
l'étendue, puis le nombre d'octets effectivement lus. Aucun octet guest écrit.
Une image correspond sur **toute sa chaîne**, SHA-256 :
`b13de58bb674b994997db6f48f2ede2afa2075ba81b1058be40c0af40f96d0d8`.
Le PID est conservé dans le JSON de capture ; l'adresse correspondante est
11636AC000h. Ni PID ni adresse ne sont utilisés par le code natif.

- [Chaîne compressée capturée dans l'original](preuves_avancement/2026-11-12/column-1024-gpu.bin).
- [Comparaisons des images GPU candidates](preuves_avancement/2026-11-12/column-1024-gpu-matches.json).
- [Provenance, catalogue et descripteur](preuves_avancement/2026-11-12/column-1024-provenance.json).
- [Trace de création originale et validation](preuves_avancement/2026-11-12/column-1024-original-trace.txt).

Les **2 048 octets du mip 4** égalent le premier mip 64×64 déjà comparé au GPU,
SHA-256 `3edbd10c1fdb46a00a1a989dd672aed26ad70f81b71b574d399881e5dd3b0ad2`.
La totalité de la queue de 5 120 octets n'est pas identique ; aucune équivalence
de tous les petits mips n'est déduite de cette comparaison partielle.
L'identité de la chaîne détaillée repose sur la comparaison GPU complète.
L'instant exact et la politique de transition du streaming ne sont pas établis.

Reproduction hors processus, avec les données du jeu et la capture conservée :

```powershell
C:/msys64/clang64/bin/python.exe scratch/verify_column_detailed_texture.py --game-dir C:/Users/Yohan/Desktop/Uncharted/CUSA02343 --gpu-capture docs/preuves_avancement/2026-11-12/column-1024-gpu.bin --output user/diag_column_1024_recheck.json
```

## Implémentation bornée

`texture_preview.cpp` sélectionne un descripteur dans une page brute, par
offsets relatifs vérifiés. Il contrôle root, type, arrays, unicité du nom,
UID, format, dimensions, couches, layout, nombre et taille totale des mips,
ainsi que l'étendue VRAM. Il ne relocalise aucun pointeur guest.

`PackageMgr_LoadNativeDictionaryTexturePreview` ouvre le dictionnaire par
le VFS, lit sa page et la seule chaîne sélectionnée, puis utilise le décodeur
BC1 Thin1DThin déjà existant. Bornes : nom de dictionnaire de 96 caractères
alphanumériques/tiret/underscore, page de 4 Mio, chaîne de 16 Mio au maximum,
dimensions puissances de deux jusqu'à 1024 ; une seule page de type 0.
Les formats, dimensions et tags hors de ce périmètre sont rejetés. La preuve
de layout et l'application restent limitées à la carte de colonnes capturée ;
ces champs ne prouvent pas le layout de tous les dictionnaires. Le fichier est fermé par RAII,
les sorties ne sont publiées qu'après succès et les images sont possédées.

Le viewer conserve sa sélection unitaire du sous-mesh 21 et du paramètre
g_tColorMap. Il essaie `chunk-21-tstrm` selon le catalogue observé, vérifie
UID/format, dimensions 1024×1024, opacité et égalité du premier mip résident
avant de remplacer sa copie locale. En cas de rejet, le 64×64 valide reste
affiché. Aucun snapshot résident du registre, login ou statut n'est modifié.
Aucun nouveau détour, aucun code PS4 exécuté en natif.

## Résultat et validation

La [capture réelle 1920×1080](preuves_avancement/2026-11-12/07-temple-pierre-1024.png)
montre les moulures, motifs et grains de pierre nettement plus détaillés qu'au
jalon précédent, à cadrage et éclairage identiques. Les grandes surfaces unies
des autres meshes restent présentes. Capture SDL du framebuffer du build
validé, conversion BMP vers PNG sans retouche, inspection visuelle effectuée.
L'HTML autonome est mis à jour sans contrôle de son affichage, conformément
à la demande de l'utilisateur.

Build basé sur `30aff7a` avec les changements de cette itération ; SHA-256 natif :
`0ab848d5656ffb75a9c51de0e517d5448ccbeebafa8e7ac08f25b5ad67d00a50`.

Quinze assertions ajoutées : descripteur valide, noms de table distincts du
nom cooked, entrées absentes, casse, doublons, bornes des noms/descripteurs,
UID incohérent, dépassement VRAM, taille de mips incorrecte, layout inconnu,
référence hors page, lecture réelle 1024, égalité du mip 64, API invalide et
absence de mutation du snapshot résident. Le premier build a aussi signalé
une comparaison signée/non signée, corrigée avant validation.

- `git diff --check` : réussi.
- Build MSYS2 : compilation, liens et déploiement réussis ; binaires out et
  out/gpu identiques. Journal `user/diag_column_mips_build_validated.log`.
- Natif, six secondes : **484/484**, sandbox full-body **831**, code **0**,
  upgrade **1024×1024** et capture effectivement observés.
- Hybride, trente secondes : SelfTest **886**, full-body **831**, **490/490**,
  **STOPPED_AFTER_TIMEOUT**, aucun crash, Guest fault ou nouvelle assertion.
- Capture originale de 45 secondes avant modification : **475/475**, SelfTest
  et sandbox réussis, timeout attendu.

[Journal natif](preuves_avancement/2026-11-12/validation-pierre-1024-native.txt)
et [journal hybride](preuves_avancement/2026-11-12/validation-pierre-1024-hybrid.txt).
Les tentatives avec les deux assertions rejetées sont conservées dans
`user/diag_column_mips_native.log`. Aucun diagnostic temporaire ajouté au GPU.

Limites : chargement ponctuel pour le viewer, pas le port de la politique de
streaming. Seul le mip 1024 est échantillonné, en bilinéaire, sans LOD,
anisotropie, mélange de cartes ni matériau original complet. Le rendu reste
statique sur CPU ; de l'aliasing peut subsister sur les surfaces éloignées.
Le registre conserve ses 60 images résidentes et les packages restent Resolved.

Prochaine étape unitaire recommandée : vérifier la carte de couleur du grand
visage, ses pixels disponibles et le trajet UV/sampler original, puis l'appliquer
à ce mesh seulement. La généralisation reste planifiée pour plus tard.
