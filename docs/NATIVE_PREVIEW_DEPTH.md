# Profondeur par pixel pour l'aperçu statique natif

Sous-système : u2_standalone, calcul hôte de l'image d'aperçu.
Symptôme : le fil de fer présente aussi les arêtes des faces cachées.
Dernier point fonctionnel : 13 680 triangles et projection validés.
Premier point incorrect : absence de remplissage opaque et de profondeur par pixel.
Hypothèse : un tampon de couleur et un tampon de profondeur hôtes permettent
de remplir les vrais triangles et de choisir la surface la plus proche,
indépendamment de leur ordre de soumission.
Offset(s) étudié(s) : aucun nouveau ; la convention de projection déjà établie
est x/y dans [-w,w], z dans [0,w] (NATIVE_CAMERA_PROJECTION.md).
Preuve statique : PreviewDepthImage clippe chaque triangle sur les six plans,
triangule le polygone convexe résultant, puis couvre les centres des pixels.
La profondeur z/w est interpolée linéairement en écran ; seul un z inférieur
au z enregistré remplace la couleur. Les deux orientations de faces sont
acceptées, sans supposer le culling original. Les égalités gardent le premier
échantillon : cet outil ne prétend pas reproduire la règle de couverture GPU.
Preuve runtime : quinze assertions indépendantes vérifient le recouvrement
dans les deux ordres, le recouvrement partiel, les deux orientations, les six
plans, les entrées dégénérées/non finies, les w inégaux et les bornes mémoire.
Le test aux plans exacts a d'abord détecté une faute d'arrondi : z pouvait
dépasser 1 de quelques ulps et laisser des trous au plan lointain. La profondeur
interpolée est désormais bornée dans [0,1], comme les profondeurs des sommets
clippés. La couverture proche/lointaine identique est vérifiée par le test.
Une collision de noms avec les macros Windows near/far dans le test initial
a aussi été corrigée avant validation, sans changement du moteur invité.
Conclusion : calcul opaque hôte validé sur des scénarios contrôlés, prêt à
produire une image statique du viewer ; il ne modifie pas le renderer hybride.
Modification : preview_depth.h, tampon RGBA32 et float, initialisation atomique
et budget maximal de 16 millions de pixels. Aucun pointeur invité n'est conservé.
Fichiers modifiés : preview_depth.h, u2_engine_tests.cpp et ce rapport.
Tests ajoutés : quinze assertions fonctionnelles, dont la profondeur attendue
0,51875 pour un triangle aux w différents et la correction au plan lointain.
Build : deux exécutables compilés et déployés avec build_msys2.ps1.
Test 6 secondes : SelfTest 886, full-body 831, moteur 325/325 et timeout sans faute.
Test natif : 319/319, délai de six secondes atteint et sortie 0.
Test prolongé : non nécessaire pour ce helper, pas encore relié à l'affichage.
Régression observée : aucune dans les scénarios vérifiés ; 889 détours inchangés.
Journaux : user/diag_solid_baseline.log, diag_depth_helper_build.log,
diag_depth_helper_native.log et diag_depth_helper_hybrid_6s.log.
Commit : feat(native): add tested depth rasterization for the static asset viewer.
Prochaine hypothèse : remplir une image du temple une fois depuis les triangles
possédés, la présenter avec SDL et inspecter les surfaces/occlusions obtenues.
Ce calcul CPU de prévisualisation ne porte pas encore le pipeline GPU du moteur,
les matériaux, l'éclairage, les instances ni les interactions du menu original.
