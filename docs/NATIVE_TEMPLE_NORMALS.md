# Lecture des normales réelles du temple

Sous-système : ndlib/render/ngen, données possédées du viewer natif.
Symptôme : les snapshots du temple ne contenaient que positions et triangles.
Dernier point fonctionnel : aperçu opaque avec profondeur par pixel.
Premier point incorrect : aucune direction de normale disponible pour préparer l'éclairage.
Hypothèse : l'attribut ID 2 des streams du temple contient quatre composantes
binary16 ; ses xyz représentent les normales, ce qui doit être compatible avec
leur longueur et les normales géométriques calculées depuis les triangles.

## Preuves et limites

Offset(s) étudié(s) : C36EA2, C36F80..C36FAC, C4E420..C4E57F.
Preuve statique : MeshDesc+28/+30 donne le compte et la table des streams,
avec un pas de 24 octets. Les attributs sont copiés par mots de quatre octets
à C4E4F0 ; le nombre est compris entre 1 et 4 (assertions C4E453/C4E4B1).
Le stride vient de u16+2, les octets de u32+4, les attributs de ptr+8 et les
données de ptr+10 hex. Pour un seul attribut, son octet de format est chargé
à C4E54F puis transmis à AllocateShaderResourceWithStride à C4E57F.

Les treize meshes du PAK réel ont un attribut `02 00 08 0A` : ID 2, offset 0,
taille 8, format 10, un attribut par stream et un stride de 8. Son nombre
d'octets vaut exactement le nombre de sommets multiplié par 8.
La capture originale déjà conservée dans `user/diag_mesh_runtime.json`
contient l'entrée 10 de la table image+16737A0 : `0cc7fa0000400000`, suivie
du même pointeur de nom que l'entrée 9. Après retrait de la base de cette
capture, il pointe à EBD2D2, chaîne `R16G16B16A16Float` vérifiée dans l'ELF.
Cette table est commune ; la capture de son descripteur rpg ne prouve pas
une liaison de mesh du temple, et n'est pas présentée comme telle.

Preuve sur les données réelles : 12 383 float4 décodés, w toujours égal à 1.
La longueur xyz se situe entre environ 0,993413 et 1,006544. Les moyennes
par mesh du produit scalaire entre ces directions et les normales des triangles
(orientées par les indices déjà validés) vont de 0,81260 à 0,99998.
L'identification comme normales repose sur cette concordance géométrique ;
la consommation de cet attribut par le shader actif reste à établir.

Premier vecteur : `(-0.00390625, 1, -0.00390625, 1)` ; mots little-endian
`9C00 3C00 9C00 3C00`. Aucune normalisation ne modifie les données copiées.
Le relevé complet des streams et de leurs mesures est conservé dans
`user/diag_temple_vertex_streams.json`.

Preuve runtime : le standalone publie `normals=12383`, et les assertions sur
les treize snapshots valident compte, valeurs et longueur des données réelles.
Le contrôle hybride prolongé retrouve le dessin original du premier mesh :
672 indices, TriangleList, 16 bits, empreinte `c63694f3788cbcac` déjà établie.
Les traces temporaires tentées dans les entrées Vulkan puis les buffers lus
par le vertex shader ne retrouvent pas les streams recherchés. Ce résultat
n'établit pas leur chemin de liaison ; aucune conclusion sur ce chemin n'est
retenue. L'instrumentation est retirée du build final.

## Implémentation et validation

Conclusion : le format de lecture est établi ; les directions identifiées
géométriquement comme normales sont disponibles dans des copies possédées.
Leur liaison GPU, l'éclairage et les matériaux originaux ne sont pas portés.
L'affichage reste en couleurs plates pendant cette itération.

Modification : champ normals optionnel, recherche par ID plutôt que par slot,
conversion binary16 exacte en float, conservation des quatre composantes et
du zéro signé. Format, offset, stride, taille et pointeurs sont contrôlés ;
les doublons, NaN et infinis sont refusés avant publication atomique.
Les pages sans attribut 2 restent lisibles sans fabriquer de normales.
Les snapshots et le journal appartiennent au PackageMgr ; aucun pointeur invité
n'est conservé et aucun détour hybride n'est modifié.

Fichiers modifiés : geometry_preview.h/.cpp, package_manager.cpp,
u2_engine_tests.cpp, plan natif et ce rapport.
Tests ajoutés : dix-huit assertions sur les vrais assets, les frontières binary16,
les zéros signés, les valeurs non finies, les bornes exactes de page, les tableaux
d'attributs, les doublons, le changement de slot et la propriété des copies.
Build : deux exécutables compilés/déployés, diag_normals_final_build.log.
Test 6 secondes : SelfTest 886, full-body 831, moteur 343/343 et
STOPPED_AFTER_TIMEOUT ; aucune faute ni assertion nouvelle dans le journal complet.
Test natif : 337/337, 12 383 normales, délai de six secondes atteint, sortie 0.
Test prolongé : diagnostic hybride de 45 secondes, 340/340 avant les trois
derniers tests de bornes ; timeout sans faute, empreinte du dessin retrouvée.
Régression observée : aucune sur les scénarios vérifiés, 889 détours inchangés.
Un premier build de la trace a échoué car regs n'était pas en portée ;
l'utilisation de Regs() a permis le diagnostic, retiré ensuite intégralement.
Journaux : user/diag_normals_baseline.log, diag_normals_original_50s.log,
diag_normals_shader.log, diag_normals_final_native.log,
diag_normals_final_hybrid.log et diag_normals_final_hybrid_console.log.
Commit : feat(native): decode owned temple normal streams.
Prochaine hypothèse : utiliser ces normales pour un éclairage de prévisualisation
explicite afin de lire les volumes ; reproduire l'éclairage original nécessitera
ensuite la preuve des matériaux et de la chaîne de shaders correspondante.
