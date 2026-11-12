# Éclairage de prévisualisation du temple

Sous-système : u2_standalone, viewer statique des assets.
Symptôme : les surfaces opaques en couleurs plates cachent le relief interne des meshes.
Dernier point fonctionnel : profondeur par pixel et 12 383 normales possédées.
Premier point incorrect : aucune variation de luminosité selon ces normales.
Hypothèse : une lumière directionnelle du viewer calculée depuis les normales,
avec interpolation continue, rend les volumes lisibles sans changer la géométrie.
Offset(s) étudié(s) : aucun nouveau ; provenance des normales dans NATIVE_TEMPLE_NORMALS.md.

Preuve statique : preview_light_intensity normalise des copies temporaires des
directions et calcule I = 0,22 + 0,78 max(0, N·L). La direction vers la lumière
est -0,35 droite +0,6 haut -1 avant dans la base de la caméra de référence.
Elle est ensuite normalisée. Il s'agit d'un preset du viewer, distinct des
lumières, matériaux et shaders originaux, dont le comportement reste à établir.

L'intensité est calculée aux sommets (Gouraud), interpolée lors du clipping
homogène puis avec correction de perspective : somme(lambda I/w) divisée
par somme(lambda/w). Le z/w de profondeur conserve son interpolation linéaire
en écran. RGB est multiplié par l'intensité et arrondi à l'octet ; alpha reste
inchangé. Le viewer accepte toujours les deux orientations de faces. Les
normales ne sont ni inversées selon la vue, ni modifiées dans les snapshots.
Sans normales exploitables, le mesh conserve ses couleurs plates avec un diagnostic.

Preuve runtime : 12 383/12 383 normales utilisées, 13 680 triangles, 2 073 600
pixels couverts et 6 091 777 écritures passant la profondeur, comme avant.
Image calculée en 154 ms sur la capture finale, puis présentée depuis une texture
SDL en cache. Capture réelle inspectée : visage de la statue, doigts, bracelets,
moulures et colonnes présentent désormais des variations de luminosité.
Le framebuffer final est identique à la première capture inspectée après correction
du test numérique ; la correction du test ne change pas le rendu.

Conclusion : éclairage de prévisualisation visible et basé sur les normales du PAK.
Les couleurs distinguent les meshes ; aucun matériau, texture, lumière originale,
ombre portée ou pipeline GPU du moteur n'est reproduit par cette étape.
L'image reste statique et calculée sur CPU, sans exécution de .text PS4.

Modification : helper de lumière propre au viewer, intensités aux sommets,
interpolation au clipping et à la rasterisation, légende explicite dans la fenêtre.
Espace conserve le cycle surfaces éclairées, fil de fer et sommets.
Fichiers modifiés : preview_lighting.h, preview_depth.h, temple_preview.h,
u2_engine_tests.cpp, plan natif et ce rapport.
Tests ajoutés : douze assertions. Alignement, côté opposé, perpendicularité,
normalisation sans mutation, directions extrêmes, entrées non finies/nulles ;
perspective à w inégaux, orientations inversées, RGB/alpha, intensités invalides,
compatibilité des surfaces plates et clipping comparé à un polygone découpé à la main.
Pour le cas à w inégaux, I attendu au pixel de contrôle est 0,46875, soit RGB 120,
alors que z/w reste 0,51875. La première assertion d'inversion des sommets exigeait
à tort l'égalité binaire de profondeur : le diagnostic mesure 18 pixels différents
de 5,96e-8 au maximum, aucune différence de couverture ou de couleur. La comparaison
de profondeur utilise désormais 1e-6 ; l'égalité exacte des couleurs est conservée.
Une déclaration auto regroupant deux types vector différents a aussi été séparée
après l'erreur du premier build de tests.

Build : deux exécutables compilés et déployés, diag_lighting_final_build.log.
Test 6 secondes : SelfTest 886, full-body 831, moteur 355/355,
STOPPED_AFTER_TIMEOUT ; journal complet sans faute ni assertion nouvelle.
Test natif : 349/349, capture à huit secondes, fermeture propre et sortie 0.
Test prolongé : non nécessaire pour ce changement de cache statique au démarrage.
Régression observée : aucune dans les scénarios vérifiés, 889 détours inchangés.
Journaux : user/diag_lighting_baseline.log, diag_lighting_final_native.log,
diag_lighting_final_hybrid.log et diag_lighting_final_hybrid_console.log.
Capture : user/diag_temple_lighting_final.bmp, convertie sans retouche en PNG
et ajoutée au dossier PREUVES_AVANCEMENT.md après enregistrement du code.
Commit : feat(native): light temple preview using decoded vertex normals.
Prochaine hypothèse : identifier les UV réels puis leur association aux textures
et matériaux avant de remplacer les couleurs de repérage par l'apparence originale.
