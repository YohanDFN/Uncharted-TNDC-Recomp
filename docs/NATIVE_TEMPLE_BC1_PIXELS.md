# Premiers pixels BC1 décodés depuis le PAK du temple

Sous-système : ndlib/render/ngen, décodage CPU pour la prévisualisation.
Symptôme : les matériaux ont des références de textures, mais aucune image RGBA possédée.
Dernier point fonctionnel : 100 références résolues dans 60 entrées locales.
Premier point incorrect : format et rangement des blocs compressés non établis.
Hypothèse : des textures résidentes du PAK sont en BC1 avec microtuiles Thin1DThin ;
leurs octets doivent correspondre à des images GPU originales avant de les décoder.

## Preuves de provenance et de format

Offset(s) étudié(s) : cas TEXTURE_TABLE à 48EBCC..48EC5C,
CreateSubResDescription BEB9A0..BEBA71, table image+16737A0.
Le login copie un compte et trois pointeurs depuis metadata+20/+28/+30/+38
dans son objet de table. Le premier pointeur désigne des clés, le deuxième
des noms de ressources et le troisième des pointeurs de descripteurs.
Il ne faut pas interpréter la première table comme des pointeurs d'images.

Dans le PAK brut, TEXTURE_TABLE est à page0+3CC00, compte 60 ; la table de
pointeurs de descripteurs est à 3CC40. Les descripteurs observés ont un offset
VRAM à +8, une taille à +10 hex, format à +1C, mip count à +24, largeur/hauteur
à +28/+2C. La lecture runtime des entrées 70 à 74 de la table des formats
confirme 71 = Bc1Unorm et 72 = Bc1UnormSrgb. Elle conserve aussi les entrées
voisines comme contrôles, sans leur attribuer une implémentation native.

Le fichier contient 85B800 hex octets après les pages CPU, à partir de
1F06B8. Cette étendue concorde avec le champ du préfixe à +20 hex. Son rôle
est confirmé ici par comparaison d'octets avec les images GPU, pas par la
seule position des données dans le fichier. Aucun lecteur PAK générique n'est
ajouté par cette itération.

Preuve runtime : BB_IMAGE_LOG=1 donne format Vulkan, adresse, taille, mipmaps
et mode de tuilage des images originales. Une lecture Windows ReadProcessMemory
du seul bb-probe du workspace (PID 16344) compare les images BC1 de 16 Kio au
maximum au bloc VRAM du PAK : 22 images correspondent par SHA-256 sur toute
leur étendue, toutes en Thin1DThin. Les tentatives préalables via le descripteur
CPU puis des slots TextureMgr n'avaient pas retrouvé le pointeur de pixels ;
ce résultat négatif ne permettait pas de conclure que les données manquaient.

Le décodeur reproduit l'adressage du backend existant : image_info.cpp exprime
BC1 en blocs 4×4, tile.h aligne les dimensions de blocs sur 8, et tiling.comp
intercale x0,y0,x1,y1,x2,y2 pour une microtuile Thin. Les microtuiles sont rangées
par lignes. Aucune règle de tuilage macro, volume, array ou multisample n'est supposée.

Échantillon décodé : index local 37, carte de couleur des colonnes
(g_tColorMap du matériau 7), 64×64, format 72, 7 mipmaps résidentes.
Les 5120 octets compressés à fichier+A262B8 correspondent à l'image GPU
1157BEC000, Bc1RgbaSrgbBlock, Thin1DThin, SHA-256 :
`4b9f0e9526ade0aab5edd273636e69236efc7b24b46b9e7d06dda2109829923f`.
Le premier mip occupe 2048 octets ; seul ce mip est décodé.

## Implémentation et validation

Conclusion : une première texture réelle du temple donne 4096 pixels RGBA possédés.
La comparaison GPU porte sur les données compressées, pas sur un readback RGBA
du GPU. Le PNG conserve les valeurs RGB encodées ; aucune conversion sRGB/linéaire,
correction de couleur, reconstruction de haute résolution ou retouche n'est appliquée.
L'image est un asset décodé, pas une capture du menu texturé.

Modification : texture_preview.h/.cpp, décodeur du premier mip BC1 RGBA
Thin1DThin, une couche et un échantillon, pitch de blocs arrondi à 8.
RGB565 est étendu par réplication des bits ; la palette conserve les quatre
couleurs ou la branche à trois couleurs et transparence, selon l'ordre des
endpoints. Les sélecteurs, bords partiels et franchissements de microtuiles
sont pris en charge. Entrées NULL, dimensions nulles/excessives, extent
compressé tronqué ou allocation impossible échouent avant publication.
Budget du viewer : 4096×4096 pixels au maximum, sans prétendre à une limite du format.

Fichiers modifiés : texture_preview.h/.cpp, u2_engine_tests.cpp, plan natif et ce rapport.
Tests ajoutés : douze assertions : endpoints RGB565, interpolation, ordre des
sélecteurs, transparence, endpoints égaux, sRGB conservé, Morton et changement
de tuile, bords partiels, octet manquant, NULL, dimensions/budgets, copie possédée,
et vrai bloc de colonnes lu par le VFS. L'offset fixe de cet échantillon est
réservé au test documenté ; le décodeur ne recherche pas les textures par offset fixe.
Une erreur arithmétique dans la première valeur attendue du test réel a été
corrigée : l'expansion RGB565 de 9471 hex donne R=148, G=142, B=140, donc
RGBA packé FF8C8E94. Les motifs de blocs connus passaient déjà ; le code du
décodeur n'a pas changé pour cette correction du test.

Build : deux exécutables compilés/déployés, diag_bc1_final_build.log.
Test 6 secondes : SelfTest 886, full-body 831, moteur 420/420,
STOPPED_AFTER_TIMEOUT ; journal complet sans faute ni assertion nouvelle.
Test natif : 414/414, délai de six secondes atteint, sortie 0.
Test prolongé : comparaison GPU originale de 45 secondes avant modification,
408/408 et timeout sans faute. Aucun diagnostic GPU activé dans les tests finaux.
Régression observée : aucune dans les scénarios vérifiés, 889 détours inchangés.
Journaux : user/diag_texture_pixels_baseline.log, diag_texture_pixels_match.log,
diag_bc1_final_native.log, diag_bc1_final_hybrid.log et diag_bc1_final_hybrid_console.log.
Artefact : user/diag_texture_column_decoded.png, produit par un petit pilote C++
appelant le même décodeur, puis conversion BMP vers PNG sans retouche ; image inspectée.
Commit : feat(native): decode GPU-matched BC1 temple texture pixels.
Prochaine hypothèse : porter la lecture bornée des descripteurs TEXTURE_TABLE et
de leur extent VRAM, puis fournir ces images au viewer avec interpolation des UV.
Le viewer du temple reste éclairé sans textures pendant cette itération ;
le streaming haute résolution, les autres formats et les matériaux restent à porter.
