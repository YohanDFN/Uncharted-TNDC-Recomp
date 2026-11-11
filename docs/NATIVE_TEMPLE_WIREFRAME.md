# Aperçu natif du temple en fil de fer

Sous-système : u2_standalone, présentation des triangles réels.
Symptôme : le nuage de points ne montre pas les contours des surfaces.
Dernier point fonctionnel : 76bdc95, 13 680 triangles lus et comparés aux
buffers utilisés comme TriangleList dans l'hybride original.
Premier point incorrect : aucune arête issue de ces triplets n'est présentée.
Hypothèse : les trois arêtes de chaque triangle, projetées et clippées selon
la caméra établie, donnent un aperçu natif exploitable du maillage.
Offset(s) étudié(s) : ceux de NATIVE_TEMPLE_TRIANGLE_INDICES.md ; aucun
nouveau point d'entrée ou changement de comportement dans l'ELF.
Preuve statique : les copies possédées fournissent des indices bornés pour
chaque mesh. Les arêtes sont dédupliquées par paire d'indices, au sein du mesh.
Le viewer conserve le même cadrage, la même caméra de départ et la même
lentille de référence que l'aperçu de sommets précédemment validé.

Le clipping se fait en coordonnées homogènes, avant la division par w :
z >= 0, z <= w, -w <= x <= w et -w <= y <= w. Un segment qui traverse
un plan est conservé sur sa portion visible ; un segment entièrement extérieur
est rejeté. Les valeurs non finies et les divisions homogènes non positives
sont refusées, avec préservation des entrées en cas d'échec.

Les segments visibles sont présentés comme de minces quads écran de 1,1 pixel,
mis en cache en lots SDL_RenderGeometry, un par sous-mesh. Ce sont des primitives
de présentation du fil de fer, pas des surfaces ajoutées au temple. Les couleurs
restent des couleurs d'inspection par sous-mesh, sans revendiquer les matériaux.
Les segments projetés de longueur nulle sont ignorés. Les faces et arêtes
occluses restent visibles : le viewer ne dispose pas encore d'un tampon de profondeur.

Preuve runtime : 13 meshes, 12 383 positions et 13 680 triangles ; le clipping
et la déduplication donnent 24 130 arêtes visibles. Le framebuffer capturé et
inspecté montre les contours du temple : colonnes, corniches, murs et ornements.
La capture provient du renderer de l'exécutable natif, sans lecture du bureau.
Conclusion : affichage natif des arêtes réelles, avec connectivité vérifiée
dans les dessins originaux. Les triangles sont désormais lisibles visuellement.
Modification : mode fil de fer par défaut et touche Espace pour alterner avec
le nuage de sommets. Le viewer conserve Échap et la fermeture de fenêtre.
Fichiers modifiés : temple_preview.h, preview_clip.h, engine_context.h,
u2_engine_tests.cpp, UNCHARTED_NATIVE_PLAN.md et ce rapport.
Tests ajoutés : huit assertions pour les segments intérieurs, les traversées
des plans proche/lointain et des quatre côtés, le rejet atomique des segments
extérieurs, les lignes derrière la caméra, w nul et NaN.
Build : deux exécutables construits et déployés avec build_msys2.ps1.
Test 6 secondes : SelfTest 886, full-body 831, moteur 310/310,
STOPPED_AFTER_TIMEOUT sans faute ni nouvelle assertion invitée.
Test natif : 304/304, 24 130 arêtes, capture réussie et sortie 0 sur huit secondes.
Test prolongé : 25 secondes sur le build final, fil de fer actif.
304/304 assertions, capture réussie, délai atteint et sortie 0, aucune faute signalée.
Régression observée : aucune dans les scénarios vérifiés ; le viewer est limité
au standalone et les 889 détours hybrides restent inchangés.
Journaux : user/diag_wireframe_build.log, diag_wireframe_native.log,
diag_wireframe_native_25s.log, diag_wireframe_hybrid_6s.log et
diag_wireframe_hybrid_console.log.
Capture : user/diag_temple_wireframe.bmp puis diag_temple_wireframe.png,
conversion de format sans retouche.
Commit : feat(native): display validated temple triangles as clipped wireframe.
Prochaine hypothèse : présenter les surfaces pleines avec une profondeur
correcte pour rendre les volumes et masquer les faces cachées, avant les textures.
Les transformations d'instances, matériaux, éclairage, interactions du menu
et producteur de paramètres caméra restent à porter ; la lentille de référence
reste celle du viewer, sans injection de paramètres dans le moteur original.
Les packages restent Resolved, aucun login moteur supplémentaire n'est appliqué
et aucun octet .text PS4 n'est exécuté pour dessiner cet aperçu natif.
