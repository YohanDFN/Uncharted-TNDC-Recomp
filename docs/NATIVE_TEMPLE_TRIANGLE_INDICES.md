# Indices du temple validés contre les dessins originaux

Sous-système : ndlib/render/ngen, connectivité possédée de l'aperçu.
Symptôme : les positions sont affichables mais relier des triplets d'indices
sans connaître la topologie produirait des arêtes spéculatives.
Dernier point fonctionnel : aperçu natif de 12 383 sommets.
Premier point incorrect : indices et topologie non établis.
Hypothèse : MeshDesc+24/+40 fournit un buffer uint16 consommé comme TriangleList,
sans primitive restart, dans les dessins des treize sous-meshes du temple.
Offset(s) étudié(s) : C36E27..C36E9D, C34550, C34280, C012E0.
Preuve statique : C36E27 lit le compte +24 ; C36E71 lit le pointeur +40.
L'allocation est arrondie à (2*count+3)&~3 et le format passé est 39 hex.
Le chemin Render écrit les commandes indexées. La topologie n'est pas inférée
du champ voisin +2C : elle est vérifiée à l'étage GPU original.

Preuve runtime : un diagnostic borné, désactivé par défaut, est ajouté à
ResolveIndexBuffer. Il ne lit le buffer qu'après les contrôles de mapping
GPU et de taille déjà employés par le backend. Il relève son empreinte XXH3
complète, la largeur des indices, la primitive et le primitive restart.
Le compteur est borné à 32 empreintes distinctes ; aucune commande n'est changée.

```
BB_U2_MESH_INDEX_COUNT=672,1143,2349,8787,1545,1959,7707,3735,504,615,10743,171,1110
C:/msys64/clang64/bin/python.exe scratch/diag_hybrid_compare.py --probe out/gpu/bb-probe.exe --label mesh_topology_all --seconds 25
```

Les empreintes sont aussi calculées directement sur chaque buffer PAK avec
libxxhash.dll, disponibles dans user/diag_mesh_index_fingerprints.json.
Chaque empreinte a été retrouvée dans les dessins originaux ; les treize
dessins indiquent index_bytes=2, primitive=4, restart=0. L'enum locale
amdgpu/regs_vertex.h identifie 4 comme TriangleList.

| Mesh | Indices | Empreinte XXH3 complète |
| --- | ---: | --- |
| 0 | 672 | c63694f3788cbcac |
| 1 | 1143 | c4ec13d89e156301 |
| 2 | 2349 | e16f2d635eaf95b1 |
| 3 | 8787 | 3a196ac3ae6892d6 |
| 4 | 1545 | 6b174215417b46db |
| 5 | 1959 | 0661bb809d0df375 |
| 6 | 7707 | e0b2e2abdd06449b |
| 7 | 3735 | eafa60a78bab3d1e |
| 8 | 504 | e0b02887375c6c23 |
| 9 | 615 | e4a3e1d0174d47fc |
| 10 | 10743 | f67270ff9eb13fce |
| 11 | 171 | ebe19e8545138078 |
| 12 | 1110 | 96aacb5148cd6132 |

Conclusion : 13 680 triangles réels, dont 224 pour le premier sous-mesh.
Le premier triplet est (0,1,2). Les indices sont copiés dans des tableaux
uint16[3] possédés, sans conserver un pointeur PAK.
Modification : extension du lecteur d'aperçu aux indices +24/+40. Comptes,
triplets complets, budget global de taille, pointeurs intra-page et chaque
index < nombre de sommets sont contrôlés avant publication atomique.
Fichiers modifiés : geometry_preview.h/.cpp, package_manager.cpp,
u2_engine_tests.cpp, vk_rasterizer.cpp et ce rapport.
Tests ajoutés : huit assertions pour les triangles réels, la lecture little-endian,
la borne exacte du sommet, l'octet haut des indices, les triplets incomplets,
les comptes excessifs et les buffers tronqués/étrangers. Le test de destruction
du buffer source vérifie aussi l'indépendance des indices.
Build : deux exécutables compilés et déployés avec build_msys2.ps1.
Test 6 secondes : SelfTest 886, full-body 831, moteur 302/302 et timeout sans faute.
Test natif : 296/296 ; 13 680 triangles lus, sortie 0 après six secondes.
Test prolongé : 25 s, captures des treize empreintes et timeout sans faute/assertion.
Régression observée : aucune dans les scénarios vérifiés, 889 détours inchangés.
Journaux : user/diag_wireframe_baseline.log, diag_mesh_indices_build.log,
diag_mesh_indices_native.log, diag_mesh_indices_hybrid_6s.log,
diag_compare_mesh_topology_original.log et diag_compare_mesh_topology_all.log.
Commit : feat(native): decode temple triangle indices validated against GPU draws.
Prochaine hypothèse : projeter et clipper les trois arêtes de chaque triangle
pour remplacer le nuage de points par un aperçu natif en fil de fer.
Ce lecteur reste limité au format d'aperçu établi ; il ne réalise aucun login
moteur, changement d'état de niveau, matériau ou appel au code PS4 natif.
