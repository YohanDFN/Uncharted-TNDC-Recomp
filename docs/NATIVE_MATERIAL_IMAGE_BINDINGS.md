# Liaison des références de matériaux aux images — 12 novembre 2026

Sous-système : snapshots natifs ndlib/render/ngen, étape 2.

Symptôme : les références locales des matériaux et les images TEXTURE_TABLE
sont chargées séparément, sans accès à l'image depuis un paramètre de matériau.

Dernier point fonctionnel : 100 références de textures comparées au résultat
PrepareBinary original ; 60 images possédées, dont 44 premiers mips BC1 décodés.

Premier point incorrect : absence d'association entre les deux snapshots.

Hypothèse initiale : les noms cooked sont strictement identiques entre tables.
Cette hypothèse est réfutée : les 100 comparaisons échouent, car les noms des
descripteurs ajoutent le suffixe littéral `.ndb` aux clés des références.

Hypothèse validée : descriptor.cooked_name = reference.cooked_name + `.ndb`.
La relation est vérifiée sur les 100 références avec une seule image par nom.
L'index de géométrie n'est pas supposé être l'index dans TEXTURE_TABLE.

Offset(s) étudié(s) : pas de nouvelle routine détournée. Champs déjà établis
par PrepareBinary C2BA20h..C2BC6Ah et login TEXTURE_TABLE 48EC43h..48EC5Ch.

Preuve statique : comparaison des noms des descripteurs du PAK brut avec les
noms capturés après PrepareBinary original. La sortie conserve pour chaque
paramètre son matériau, sa clé cooked, son index local et les indices d'images
correspondants :
[material-image-names.json](preuves_avancement/2026-11-12/material-image-names.json).
La méthode ne retire pas arbitrairement une extension et ne normalise pas la
casse, les chemins ou le hash final. Un suffixe différent échoue.

Preuve runtime : les nouveaux lecteurs retrouvent les 100 références des
treize meshes. 59 références donnent des pixels décodés, 41 désignent des
images compressées uniquement. Ces nombres comptent les références et non
les images distinctes. Le paramètre g_tColorMap du matériau de colonnes
retrouve le BC1 64×64 précédemment comparé au GPU, pixel initial FF8C8E94.

Conclusion : une image est désormais accessible par package, mesh et numéro
de référence de matériau, avec contrôle du nom. Aucun shader ou sampler
original n'est initialisé par cette association de snapshots hôtes.

Modification : ajout de `image_index`, distinct de `texture_index`, dans les
références possédées. Le helper prépare tous les indices avant de les publier ;
nom absent, vide, ambigu ou référence locale non résolue font échouer l'ensemble
sans modifier les anciens indices. L'API PackageMgr retourne une copie possédée
de l'image ; les bornes et le nom sont revérifiés sous le verrou du registre.
Les pixels ne sont pas dupliqués dans chaque référence. Les images compressées
restent explicitement sans pixels. Le package reste Resolved, login en attente.

Fichiers modifiés : geometry_preview.h/.cpp, package_manager.h/.cpp,
u2_engine_tests.cpp, plan natif et documentation de preuves.

Tests ajoutés : 14 assertions. Comparaison des 100 associations avec l'API
d'accès par index, paramètre des colonnes, entrées API invalides, ordre inversé
des images avec indices locaux différents, échec tardif atomique, doublons,
casse, référence non résolue, nom vide, reprise après rejet, image non décodée,
suffixe en majuscules, suffixe non terminal, suffixe absent et sortie NULL.

Build : compilation et déploiement réussis, user/diag_image_bindings_build.log.

Test 6 secondes : SelfTest 886, full-body 831, moteur 462/462,
STOPPED_AFTER_TIMEOUT. Aucun crash, Guest fault ou nouvelle assertion.
Les 889 détours hybrides sont inchangés. Les avertissements Vulkan et les
ressources optionnelles absentes restent présents.

Test natif : 456/456, fermeture propre après six secondes, sortie 0. Aperçu
éclairé du temple actif, zéro page de code invité. Baseline préalable :
user/diag_image_bindings_baseline.log, 442/442 et sortie 0.

Test prolongé : non nécessaire, association synchrone avant la boucle de rendu.

Régression observée : aucune dans les scénarios finaux testés. Les deux échecs
initiaux dus à l'hypothèse sans suffixe sont conservés dans
user/diag_image_bindings_without_suffix_native.log et
user/diag_image_bindings_without_suffix_hybrid.log.

Journaux finaux : user/diag_image_bindings_native.log,
user/diag_image_bindings_hybrid.log et user/diag_image_bindings_hybrid_console.log.
Copie du journal natif :
[validation-material-images.txt](preuves_avancement/2026-11-12/validation-material-images.txt).

Commit : feat(native): associate material references with resident texture images.

Prochaine hypothèse : interpoler les UV avec correction de perspective puis
échantillonner une carte de couleur décodée. Les modes de sampler originaux
restent à établir ; aucun changement d'affichage n'est produit à cette étape.

Documentation : ROADMAP.md conserve une feuille de route Bloodborne historique ;
le plan Uncharted natif et les preuves locales décrivent ce jalon observé.
