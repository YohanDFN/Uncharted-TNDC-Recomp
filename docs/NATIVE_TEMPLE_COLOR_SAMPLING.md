# Première carte de couleur appliquée au temple — 12 novembre 2026

Sous-système : viewer natif, sans nouveau détour ni changement du pipeline hybride.

Hypothèse : le float2 de l'attribut 1 du sous-mesh 21 fournit directement les
coordonnées de sa carte de couleur ; l'image doit être liée au shader actif
et son sampler établi avant application. Critère : détails de pierre visibles
sur ce sous-mesh, profondeur conservée et tests sans régression.

## Preuves originales

La capture antérieure de deux samplers ne prouvait que leur co-présence.
Une instrumentation temporaire dans `Rasterizer::FindPreparedStage` examine
les descripteurs préparés dont les ressources et le programme correspondent
au stage courant. La signature initiale de huit octets et les dimensions 64×64
servent seulement à sélectionner deux dessins à capturer, jamais à conclure
à l'identité de l'image.

Après contrôle `IsValidGpuMapping` de chaque étendue lue, deux dessins au
maximum publient les hashes de shaders, bindings et samplers. Ils copient
5 120 octets des images candidates et le buffer UV de 6 648 octets
(stride 4, borne diagnostique 1 Mio). Les lectures sont immédiates, sans
écriture guest ni conservation de pointeur. Le buffer UV est une ressource
Guest : le nom SPIR-V `ssbo_7` correspond à l'ordinal 5 de cette liste,
pas à l'ordinal 6. La première tentative sur 6 lisait le descripteur spécial
NULL, taille zéro ; aucune lecture de cette adresse n'a été effectuée.

- [Trace des deux dessins](preuves_avancement/2026-11-12/column-bindings-capture.txt).
- [Comparaisons et instructions de shaders](preuves_avancement/2026-11-12/column-shader-bindings.json).
- [Buffer UV original capturé](preuves_avancement/2026-11-12/column-runtime-uv.bin).
- [Image compressée originale capturée](preuves_avancement/2026-11-12/column-runtime-image.bin).

Les deux stages préparés ont les hashes `f1ef5c58` (fragments) et `50411a0b`
(sommets). Les images 1 et 4 correspondent sur leurs **5 120 octets** à la
carte de couleur déjà comparée au PAK, SHA-256
`4b9f0e9526ade0aab5edd273636e69236efc7b24b46b9e7d06dda2109829923f`.
L'adresse varie entre processus ; aucun pointeur capturé n'entre dans le viewer.

Le shader traduit en cache, désassemblé avec `spirv-dis`, charge image1 et
sampler0 pour `OpImageSampleImplicitLod`. Ses coordonnées proviennent de
l'interpolation d'attr0.xy. Le shader de sommets lit un `half2` dans `ssbo_7`
et le transmet sans transformation dans attr0.xy et attr0.zw. Le buffer
consommé est identique aux 1 662 couples du sous-mesh 21 à page0+112000,
SHA-256 `e00fa64fbf6382e613fa9c2368918c3f7bfd4093190114fed455802a7baf7476`.
Il n'y a donc pas d'inversion de V ou d'échelle ajoutée dans ce trajet primaire.
Cela ne porte pas les autres échantillonnages ni le mélange du matériau.

Sampler0 : raw0 `00fff00000000400`, raw1 `4000000008f00000`.
Les champs de `gpu/shadps4/video_core/amdgpu/resource.h` établissent Wrap XYZ,
coordonnées normalisées, min/mag AnisoLinear, mip Linear, anisotropie maximale
4 et biais LOD zéro. Sampler1 a une anisotropie différente ; le viewer ne
choisit pas entre les deux au nom du matériau. La liaison primaire à sampler0
est établie par l'instruction du shader.

Reproduction de l'analyse après capture :

```powershell
C:/msys64/clang64/bin/python.exe scratch/verify_temple_column_bindings.py --pak C:/Users/Yohan/Desktop/Uncharted/CUSA02343/u2data/build/main/pak22/ps4/menu-temple.pak --output user/diag_column_bindings_recheck.json
```

Le script vérifie les captures `user/diag_column_*`, les streams déjà relevés
et les variantes SPIR-V du cache local. Les SHA-256 et extraits utiles restent
dans le JSON versionné. L'instrumentation GPU a été retirée avant le build final.

## Modification et résultat visible

`preview_depth.h` transporte les UV dans le clipping homogène puis interpole
u/w, v/w et 1/w. Le z/w conserve son interpolation linéaire écran.
`preview_texture.h` échantillonne avec répétition et filtre bilinéaire au
centre des texels. Les coordonnées négatives, les coutures et les dimensions
unitaires sont prises en charge ; les entrées invalides préservent les sorties.
Les texels sRGB sont décodés avant filtrage et éclairage, puis réencodés pour
la texture SDL. Cette combinaison avec l'éclairage du viewer est une convention
de prévisualisation, pas la restitution du matériau original.

`TemplePreview` limite cette étape à `menu-temple.Prototype0.Submesh21` et
cherche son unique paramètre `g_tColorMap` via l'API possédée existante.
Nom cooked, format, dimensions, taille et opacité sont contrôlés ; aucune
adresse ni index d'image n'est deviné. Le snapshot de l'image reste vivant
pendant la rasterisation du mesh. Les autres matériaux gardent leur affichage
précédent ; les packages restent Resolved et leurs logins restent en attente.

Résultat : **un mesh texturé, 590 779 écritures passant le test de profondeur**.
Ces écritures ne comptent pas les pixels finalement visibles distincts.
La capture montre des détails gris de pierre et de moulures sur les surfaces
du matériau nommé « colonnes », notamment les corniches entourant la statue.
Les grands pans verts au premier plan appartiennent à d'autres sous-meshes.

[Capture réelle du build final](preuves_avancement/2026-11-12/06-temple-pierre-texturee.png)
et [galerie autonome](preuves_avancement/2026-11-12/preuves_avancement.html).
Framebuffer SDL 1920×1080, conversion BMP vers PNG sans retouche, inspection
visuelle effectuée. L'image embarquée dans le HTML est identique au PNG.
Le contrôle visuel de l'affichage HTML n'est pas poursuivi, à la demande de
l'utilisateur ; les cinq images antérieures et les octets de la sixième
image embarquée sont vérifiés sans modifier les captures.
Build : base `338d654` avec les changements de cette itération ; SHA-256 de
`out/uncharted2-native.exe` :
`fb162bb13dc1f7f16f4208c61b43853b860cfe0fe1b2f48d532efe5e4e0bddde`.

## Validation

Baseline fonctionnelle réutilisée : `abfe63c`, 456 assertions natives / 462
hybrides, même géométrie, UV, images et associations. La tentative de capture
baseline de cette session a été interrompue par le script de déploiement ; son
code -1 n'est pas présenté comme un succès. La capture éclairée précédente
reste la référence visuelle conservée.

Treize assertions ajoutées : centres de texels, répétition négative,
coutures, éclairage linéaire et sRGB, filtrage sRGB, entrées invalides,
grandes coordonnées finies, image 1×1 et alpha, perspective à w inégaux,
équivalence du clipping UV, absence de mutation sur rejet et équivalence
avec l'ancien rendu pour une texture constante linéaire.

- `git diff --check` : réussi.
- `powershell -ExecutionPolicy Bypass -File scratch/build_msys2.ps1` : compilation,
  liens et déploiement réussis ; binaires `out` et `out/gpu` identiques.
- `scratch/run_native_test.ps1 6` via PowerShell : **469/469**, sandbox full-body
  **831**, fermeture propre, code **0**, capture du framebuffer réussie.
- `scratch/run_test.ps1 30` via PowerShell : SelfTest **886**, full-body **831**,
  **475/475**, **STOPPED_AFTER_TIMEOUT**, aucun crash, Guest fault ou nouvelle assertion.
- Capture diagnostique originale : 45 secondes, **475/475**, timeout attendu.

[Journal natif final](preuves_avancement/2026-11-12/validation-pierre-texturee-native.txt)
et [journal hybride final](preuves_avancement/2026-11-12/validation-pierre-texturee-hybrid.txt).
Compilation : `user/diag_columns_final_build.log` ; diagnostics :
`user/diag_columns_probe3.log`. Une tentative intermédiaire de déploiement
avait rencontré un exécutable encore ouvert ; après arrêt du processus du
workspace, le déploiement final est propre. Les avertissements et ressources
optionnelles absentes préexistants restent présents.

## Limites et prochaine expérience

Seul le premier mip résident 64×64 est utilisé. Pas de choix LOD, trilinearité,
anisotropie, normal map, mélange entre cartes, matériau ou éclairage original.
L'aperçu reste statique, calculé sur CPU et présenté par SDL, sans code PS4
exécuté en natif. Le résultat ne prouve pas une équivalence pixel à pixel au jeu.

Prochaine expérience recommandée pour un gain visible : établir le trajet
carte de couleur/UV/sampler du grand visage et appliquer sa carte primaire
si son format résident est déjà décodé. Vérifier d'abord la disponibilité des
pixels et les instructions actives ; s'il exige un format non décodé, porter
ce format avec comparaison des données originales avant application.
