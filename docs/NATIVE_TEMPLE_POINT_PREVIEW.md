# Premier aperçu natif visible des assets du temple

Sous-système : u2_standalone, présentation SDL de données possédées.
Symptôme : le standalone affiche un décor de prototype, alors que les positions
réelles du temple sont maintenant accessibles.
Dernier point fonctionnel : a9cbe22, treize sous-meshes et 12 383 positions copiés.
Premier point incorrect : aucune de ces positions n'est dessinée dans la fenêtre native.
Hypothèse : un aperçu de positions depuis la caméra de départ suffit à produire
un premier résultat visible sans inventer la topologie ou les matériaux.
Offset(s) étudié(s) : ceux des rapports caméra, projection et GEOMETRY_1 ;
aucun nouvel appel ou détour dans l'ELF.
Preuve statique : les données et leur format ont été établis avant ce changement
dans NATIVE_GEOMETRY_PREVIEW.md. Le viewer prend des copies de la géométrie,
de la caméra et du décalage par les accesseurs du package. Il exige la caméra
de départ, un quaternion calculé et une translation de niveau nulle pour ce
scénario spécifique. Il n'applique aucune transformation d'instance non établie.
Les colonnes C/A/B du quaternion servent à transformer les différences de
positions par rotation inverse. Le calcul de perspective déjà testé est réutilisé.
Preuve runtime : l'aperçu affiche 11 576 positions dans le frustum, sur 12 383
provenant des treize sous-meshes. Une capture du framebuffer SDL, sans capturer
le bureau ni une autre fenêtre, montre les silhouettes des colonnes et éléments
du temple sous forme de points colorés. La capture a été ouverte et inspectée.
Conclusion : premier affichage natif de positions des vrais assets du temple,
avec caméra de départ réelle ; aucun décor hôte n'est fabriqué pour cet aperçu.
Il s'agit d'un nuage de sommets, pas encore de triangles, de fil de fer ou du
menu original interactif. Les couleurs distinguent les sous-meshes, sans simuler
les matériaux. L'occlusion et la profondeur par pixel ne sont pas implémentées.
Modification : TemplePreview prépare les points écran une fois, filtre le
frustum et les présente par lots SDL. La toile logique 1920x1080 est conservée
avec letterboxing au redimensionnement. L'aperçu remplace l'écran de prototype
lorsque ses données sont disponibles ; seuls Échap et la fermeture de fenêtre
agissent dans cet écran, pour éviter des actions sur des boutons invisibles.

Le viewer utilise explicitement une lentille de référence issue de la capture
originale : zoom 1,303225398, plans 0,24 et 5000. Ce réglage est limité au viewer
d'assets demandé ici. Il ne prétend pas remplacer le producteur de paramètres
caméra du moteur, toujours à porter ; il n'est pas injecté dans l'hybride ou
dans un objet Camera/Level. La position et l'orientation viennent du PAK.

Fichiers modifiés : u2_standalone/temple_preview.h, engine_context.h,
UNCHARTED_NATIVE_PLAN.md et ce rapport.
Tests ajoutés : aucun test qui reproduit les opérations SDL. La capture effective
du framebuffer et les compteurs de points servent à la validation visuelle.
Les 288 assertions natives, dont celles du lecteur et de la projection, restent actives.
Build : les deux exécutables construits et déployés avec build_msys2.ps1.
Test 6 secondes : SelfTest 886, full-body 831, moteur 294/294, timeout sans faute.
Test natif : premier essai 8 s, 288/288 et sortie 0 ; capture du framebuffer réussie.
Test prolongé : 25 secondes sur le build final, aperçu natif et fermeture propre.
288/288 assertions, 11 576 points visibles, capture réussie, sortie 0 et aucune faute.
Régression observée : aucune dans les scénarios vérifiés ; le code de l'aperçu
n'est compilé et appelé que par le standalone, aucun détour hybride modifié.
Journaux : user/diag_temple_preview_build.log, diag_temple_preview_native.log,
diag_temple_preview_native_25s.log, diag_temple_preview_hybrid_6s.log et
diag_temple_preview_hybrid_console.log.
Capture : user/diag_temple_native_preview.bmp, convertie sans retouche en
user/diag_temple_native_preview.png.
Diagnostic reproductible : définir BB_NATIVE_CAPTURE=user/diag_temple_native_preview.bmp
puis lancer scratch/run_native_test.ps1 ; le premier framebuffer de l'aperçu
est enregistré une seule fois, avant la présentation.
Commit : feat(native): preview real temple mesh positions in standalone.
Prochaine hypothèse : établir les indices et la topologie du mesh pour passer
du nuage de points aux arêtes puis aux surfaces, avec validation dans l'original.
Le pipeline moteur de géométrie, les instances, les matériaux, l'éclairage,
le producteur de projection et les interactions du menu restent à porter.
Les packages restent Resolved avec neuf logins en attente. Aucun octet de
.text PS4 n'est exécuté pour afficher cet aperçu.
