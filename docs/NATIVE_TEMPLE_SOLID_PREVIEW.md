# Surfaces opaques du temple dans l'aperçu natif

Sous-système : u2_standalone, viewer statique des vrais assets.

Symptôme : le fil de fer montre les arêtes cachées et rend le volume difficile à lire.
Dernier point fonctionnel : 13 sous-meshes, 13 680 triangles et profondeur testée.
Premier point incorrect : le calcul opaque n'était pas relié à l'affichage.
Hypothèse : remplir une image depuis les triangles possédés puis la présenter
avec SDL révèle les surfaces proches en masquant celles derrière elles.

Offset(s) étudié(s) : aucun nouvel offset. Géométrie et caméra déjà établies
dans NATIVE_TEMPLE_TRIANGLE_INDICES.md et NATIVE_CAMERA_PROJECTION.md.
Preuve statique : PreviewDepthImage reçoit les vrais sommets projetés et indices,
clippe les triangles, puis compare la profondeur par pixel. Les couleurs plates
distinguent les sous-meshes ; elles ne représentent pas les matériaux originaux.
Les deux orientations sont acceptées sans supposer le culling du moteur.

Preuve runtime : capture réelle en 1920 × 1080 inspectée visuellement ; bras de
la statue et colonnes visibles en surfaces pleines, avec masquage des surfaces
éloignées. 2 073 600 pixels couverts et 6 091 777 écritures passant le test de
profondeur. Cache calculé en environ 100 ms sur le test, puis texture SDL réutilisée.
Les 25 secondes de validation incluent plusieurs cycles surfaces → fil de fer →
sommets → surfaces, confirmés dans le journal, et une fermeture propre.

Conclusion : l'aperçu statique présente les vrais triangles opaques avec profondeur.
Ce calcul CPU ne porte pas le pipeline GPU du jeu. La texture SDL présente l'image
calculée ; elle n'apporte ni matériaux, ni éclairage, ni transparence, ni animation.
La caméra et la lentille restent le preset de référence précédemment documenté.

Modification : cache couleur/profondeur hôte, texture RGBA32 créée au premier
affichage, trois modes via Espace. La texture est libérée avant le renderer SDL ;
un échec d'upload est signalé et conserve la vue fil de fer. Aucun code PS4 exécuté.
Fichiers modifiés : temple_preview.h, engine_context.h, plan natif et ce rapport.
Tests ajoutés : aucun test reproduisant l'intégration SDL ; les quinze assertions
fonctionnelles du calcul de profondeur sont conservées (NATIVE_PREVIEW_DEPTH.md).
Build : les deux exécutables compilés et déployés, diag_solid_view_build.log.
Test 6 secondes : SelfTest 886, full-body 831, moteur 325/325,
STOPPED_AFTER_TIMEOUT ; journal hybride complet examiné sans faute ni assertion nouvelle.
Test natif : 319/319, capture à huit secondes et sortie 0.
Test prolongé : 25 secondes, 319/319, transitions des trois modes, sortie 0.
Régression observée : aucune dans les scénarios vérifiés ; 889 détours inchangés.
Journaux : user/diag_solid_view_native.log, diag_solid_modes_25s.log,
diag_solid_view_hybrid_6s.log et diag_solid_view_hybrid_console.log.
Commit : feat(native): preview opaque temple surfaces with per-pixel depth.
Prochaine hypothèse : un éclairage de prévisualisation explicitement distinct des
matériaux originaux permettrait de mieux lire les volumes ; l'étape de fidélité
suivante nécessite d'établir les normales et matériaux réels avant de les porter.
