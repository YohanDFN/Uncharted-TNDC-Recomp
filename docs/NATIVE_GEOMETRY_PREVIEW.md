# Lecture possédée des positions du temple

Sous-système : ndlib/render/ngen, préparation de données de prévisualisation.
Symptôme : GEOMETRY_1 est cataloguée mais aucune position de mesh n'est lisible en natif.
Dernier point fonctionnel : caméra et calcul de projection comparés à l'ELF.
Premier point incorrect : absence de lecture des descripteurs et streams de positions.
Hypothèse : les meshes du temple utilisent un premier stream R32G32B32Float
qui peut être copié indépendamment des matériaux et du login moteur.
Offset(s) étudié(s) : 48ED11, 48EFA7..48EFC2, B8F4B0, BE3D82..BE3DF1,
C29394/C29398, C29660..C296F6, C36E27..C36FAC, C4E509..C4E543, C4E6AD.
Preuve statique : hash GEOMETRY_1 D97F2C44 ; le login passe metadata+20
à Background::CreateGeometry puis aux tables de GeoGroup. Les descripteurs
de mesh sont à payload+10, compte à +8 et pas 88 hex (C29398).
MeshDesc+20 donne le compte de sommets ; +28/+30 le compte et le pointeur
des streams. Un stream a un pas de 24 octets ; u16+0 = nombre d'attributs,
u16+2 = stride, u32+4 = taille, pointeurs +8 = attributs et +10 = données.
Les attributs sont copiés par mots de quatre octets ; le premier attribut
des treize meshes vaut 00 00 0C 06 : ID/offset 0, taille 12, format 6.
Preuve runtime : hook C36DD0:6, prologue SysV vérifié, observé pendant le
chargement hybride (550 appels sur 45 s). Une lecture Windows ReadProcessMemory
du seul processus bb-probe du workspace, sans modification de sa mémoire,
confirme l'entrée 6 de la table des formats image+16737A0 :
0dc73a0000600000 suivi d'un pointeur de nom, résolu dans la rodata comme
R32G32B32Float (EBD2A7). Le dernier descripteur observé était rpg.Prototype1.Submesh0 ;
on ne l'attribue pas au temple. Le format commun est confirmé, pas chaque mesh original.
Le PAK réel place GEOMETRY_1 à page0+1E80E0, table à 1E79E0 : treize entrées,
dont menu-temple.Prototype0.Submesh0, 321 sommets. Son premier float3 est
(-69.251220703125,38.54443359375,-9.87060546875). Total natif : 12 383 positions.
Conclusion : copie possédée des noms et float3, sans indices, topologie,
matériaux, transformations d'instances ni simulation de login. Les formats
autres que le premier stream canonique et les pointeurs inter-pages sont refusés.
Modification : ReadGeometryPreview et accesseur de package par copie sous verrou.
Fichiers modifiés : geometry_preview.h/.cpp, package_manager.h/.cpp,
u2_engine_tests.cpp et ce rapport.
Tests ajoutés : seize assertions couvrant le PAK réel, les valeurs signées,
deux descripteurs adjacents, les formats/strides non pris en charge, comptes,
tailles, pointeurs étrangers, NULL, NaN, publication atomique et copie possédée.
Le total de positions est borné par le budget de taille de la page fournie.
Build : deux exécutables compilés et déployés avec build_msys2.ps1.
Test 6 secondes : SelfTest 886, full-body 831, moteur 294/294 et timeout sans faute.
Test natif : 288/288, treize meshes et 12 383 positions, délai six secondes, sortie 0.
Test prolongé : diagnostic original 45 s, timeout et zéro faute/assertion signalée.
Régression observée : aucune sur les scénarios vérifiés, 889 détours inchangés.
Journaux : user/diag_geometry_baseline.log, diag_geometry_build.log,
diag_geometry_native.log, diag_geometry_hybrid_6s.log, diag_compare_mesh_original.log,
diag_mesh_runtime.json.
Commit : feat(native): decode owned temple mesh position streams.
Prochaine hypothèse : afficher ces positions comme nuage de points depuis la
caméra établie, avant de revendiquer une topologie de triangles ou un rendu du jeu.
Les packages restent Resolved, neuf logins restent en attente ; aucun appel au
code PS4 ni nouveau détour hybride n'est introduit par ce lecteur.
