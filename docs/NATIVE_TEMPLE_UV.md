# Coordonnées UV possédées du temple

Sous-système : ndlib/render/ngen, données du viewer natif.
Symptôme : les snapshots possèdent positions, triangles et normales, mais pas
les coordonnées nécessaires à l'échantillonnage de textures.
Dernier point fonctionnel : éclairage directionnel de prévisualisation visible.
Premier point incorrect : l'attribut 1 des streams est ignoré par le lecteur.
Hypothèse : cet attribut stocke une paire de coordonnées binary16 par sommet ;
le format doit être confirmé dans la table originale avant de le décoder.

Offset(s) étudié(s) : C4E4F0, C4E509..C4E57F, C23BE0..C23C5E,
table image+16737A0, chaîne EBD3F2 ; pour la prochaine expérience, C29660..C296F6.
Preuve statique : les définitions d'attribut font quatre octets, leur octet
de format est chargé à C4E54F, puis transmis à AllocateShaderResource à C4E57F.
Les treize descripteurs du PAK ont `01 00 04 22` : ID 1, offset 0, taille 4,
format 34. Un seul attribut par stream, stride 4, taille = nombre de sommets × 4.

Preuve runtime : lecture Windows ReadProcessMemory de trois entrées seulement
dans le bb-probe du workspace lancé pour 40 secondes. PID 28796, image à B100000.
L'entrée 34 vaut `05c7220000200000f2d3fb0b00000000` ; son pointeur de nom
résout la chaîne `R16G16Float` à image+EBD3F2. Les entrées 6 et 10 sont relues
comme contrôles : R32G32B32Float et R16G16B16A16Float. Aucun octet du processus
n'est modifié, aucun hook ajouté. Le test original termine au timeout sans faute.
Cette table confirme l'encodage ; elle ne prouve pas la consommation d'un
stream du temple par un shader particulier ni son association à une texture.

Les 13 meshes fournissent 12 383 paires. La première est exactement
`(-2.51171875, -2.513671875)`, depuis page0+156900, mots C106 et C107.
Pour ce premier mesh, U va de -7.21484375 à 3.513671875 et V de -6.11328125
à 7.6953125. Les valeurs négatives ou hors [0,1] sont conservées : aucune
répétition, saturation, inversion de V ou conversion de sampler n'est supposée.
Le rôle UV de l'attribut 1 est compatible avec ces float2 ; sa liaison au shader
et les éventuelles transformations de coordonnées restent à établir.

Les relevés sont conservés dans [la table runtime](preuves_avancement/2026-11-12/uv-formats-runtime.json)
et [les streams du PAK](preuves_avancement/2026-11-12/uv-streams.json).

Conclusion : le natif publie `uv=12383`, avec une paire possédée par sommet,
sans pointeur dans les pages sources. La lecture du format est établie ; aucune
texture n'est attribuée aux meshes. L'image éclairée ne change pas à cette étape.
Modification : champ texcoords optionnel, recherche par ID, contrôle du mot
d'attribut, stride, taille, pointeur, doublons et valeurs non finies ; publication
atomique de l'ensemble du snapshot. Conversion binary16 commune aux normales
et UV, avec préservation des zéros signés et sous-normaux. Les pages sans
attribut 1 restent lisibles sans inventer de coordonnées.
Fichiers modifiés : geometry_preview.h/.cpp, package_manager.cpp,
u2_engine_tests.cpp, plan natif, ce rapport et deux relevés de preuves JSON.
Tests ajoutés : seize assertions. Comptes réels, premier couple exact, présence
de coordonnées hors [0,1], cohabitation avec les normales, limites binary16,
NaN/infinis sur les deux composantes, format/offset/taille/stride incorrects,
pointeur étranger, borne exacte de page et dépassement d'un octet, doublons,
permutation des slots et propriété des copies après destruction de la page.
Build : deux exécutables compilés/déployés, diag_uv_build.log.
Test 6 secondes : SelfTest 886, full-body 831, moteur 371/371,
STOPPED_AFTER_TIMEOUT ; journal complet sans faute ni assertion nouvelle.
Test natif : 365/365, 12 383 paires UV chargées, délai de six secondes et sortie 0.
Test prolongé : observation originale 40 secondes avant modification, 355/355,
timeout sans faute ; la lecture native synchrone ne nécessite pas un autre test long.
Régression observée : aucune sur les scénarios vérifiés, 889 détours inchangés.
Journaux : user/diag_uv_baseline.log, diag_uv_original.log,
diag_uv_native.log, diag_uv_hybrid.log et diag_uv_hybrid_console.log.
Commit : feat(native): decode owned temple texture coordinates.

Prochaine hypothèse : résoudre la liaison mesh → matériau → textures. Le champ
MeshDesc+48 hex pointe sur des chaînes telles que
`levels/temple/walls/tmp-stone-crumpled-blend:redpaint-to-lrg-bri...` et
`levels/temple/outsourced/tmp-face-gold:03,...`. Ce sont des pistes, pas encore
des liaisons de textures validées. C29668 lit l'index signé à MeshDesc+50 hex,
contrôle m_numMaterialInstances (assertion GetBoundMaterial) puis passe l'entrée
de stride 70 hex à la création du mesh (C296F6). Le premier index brut du PAK
vaut -1. Il faut identifier son producteur et la résolution des matériaux avant
de considérer ces noms comme des textures directement utilisables.
