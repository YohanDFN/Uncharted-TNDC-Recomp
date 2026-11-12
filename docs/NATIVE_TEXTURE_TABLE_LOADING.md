# Chargement des images résidentes — 12 novembre 2026

Sous-système : TEXTURE_TABLE, étape 1 du chargement natif des textures.

Symptôme : PackageMgr ne possède pas les images ; le test BC1 réel utilise
encore un offset fixe dans menu-temple.pak.

Dernier point fonctionnel : treize meshes, cent références de textures et
un premier mip BC1 comparé aux données compressées d'une image GPU originale.

Premier point incorrect : absence de lecture des images par leurs descripteurs.

Hypothèse : le bloc VRAM suit la dernière page CPU et les descripteurs
donnent ses offsets relatifs. Confirmation attendue : retrouver les 60 images
et le pixel de colonnes précédemment validé sans offset de fichier constant.

Offset(s) étudié(s) : login original 48EC43h..48EC5Ch ; format des images
documenté dans `NATIVE_TEMPLE_BC1_PIXELS.md`.

Preuve statique : metadata+20h contient le nombre ; +28h pointe les UID uint32,
+30h les pointeurs de noms de ressources et +38h les pointeurs de descripteurs.
Ces trois tables sont relocatées. Chaque descripteur de 64 octets contient :

| Champ | Offset | Taille |
| --- | --- | --- |
| Offset relatif VRAM | 08h | uint64 |
| Taille résidente, chaîne de mips comprise | 10h | uint32 |
| UID, comparé à la table | 14h | uint32 |
| Couches, format, flags | 18h, 1Ch, 20h | uint32 chacun |
| Mips, largeur, hauteur | 24h, 28h, 2Ch | uint32 chacun |
| Tag de layout opaque | 34h | uint32 |
| Pointeur relocaté vers le nom cooked | 38h | uint64 |

Le champ +0 n'est pas traité comme pointeur CPU. La signification générale
du tag +34h reste ouverte. Décodage limité aux combinaisons observées :
formats 71/72, flags respectifs 01000000h/010F0000h, tag 2, une couche et
dimensions puissances de deux au plus égales à 64. La somme des tailles
de mips BC1, dimensions en blocs alignées sur 8, doit égaler la taille déclarée.

Preuve runtime : les deux pages non compressées proviennent de la table du
header ; leur dernière fin donne le début VRAM. La taille header+20h doit
rejoindre exactement EOF. Sur le fichier réel : début calculé 1F06B8h,
8763392 octets, 60 images, 44 premiers mips BC1 décodés et 177152 pixels.
Les 16 autres images restent compressées. L'index 37 donne 64×64, format 72,
sept mips, 5120 octets et le premier pixel FF8C8E94 déjà validé.
La comparaison GPU antérieure porte sur les octets compressés ; aucun
readback RGBA du GPU supplémentaire n'est revendiqué.

Conclusion : hypothèse confirmée pour le package observé à deux pages CPU
non compressées. Noms, octets compressés et pixels sont des copies possédées.

Modification : lecture VFS du bloc et API de copie par index
`u2_cpp_PackageMgr_GetNativeTexturePreview`. Pointeurs, chaînes terminées,
UID, dimensions, mips et extents sont vérifiés avant publication atomique.
Budgets du viewer : 16 Mio résidents et 16 millions de pixels cumulés.
Un échec conserve la sortie précédente. Aucun pixel fabriqué pour les
dispositions non prises en charge. Le package reste Resolved, login moteur
en attente ; aucun nouveau détour hybride ni exécution de code PS4 en natif.

Fichiers modifiés : texture_preview.h/.cpp, package_manager.h/.cpp,
u2_engine_tests.cpp, plan natif, rapport BC1 et dossier de preuves.

Tests ajoutés : 28 assertions couvrant données réelles, déplacement des tables
et pages, EOF, chevauchement des pages, budgets, UID incohérents, pointeurs
étrangers, descripteurs tronqués, chaînes non terminées, overflows, mips,
récupération après erreur et durée de vie. Le test BC1 utilise désormais l'API.

Build : réussi, `user/diag_texture_table_build.log`. Une comparaison signée/non
signée refusée par Werror a été corrigée avant les tests des nouveaux binaires.

Test 6 secondes : hybride SelfTest 886, full-body 831, moteur 448/448,
STOPPED_AFTER_TIMEOUT, aucun crash, Guest fault ou nouvelle assertion.
Journal complet examiné : `user/diag_texture_table_hybrid.log`, console
`user/diag_texture_table_hybrid_console.log`. Les avertissements Vulkan et
ressources optionnelles absentes demeurent présents.

Test natif : 442/442, fermeture propre à six secondes, sortie 0,
`user/diag_texture_table_native.log`, zéro page de code invité.

Test prolongé : non nécessaire, chargement avant la boucle de rendu.

Régression observée : aucune dans les scénarios testés ; 889 détours hybrides.
Le rendu conserve l'aperçu éclairé précédent, sans application des textures.

Commit : feat(native): load resident texture table images from package metadata.

Prochaine hypothèse : relier les images aux références de matériaux déjà
résolues en vérifiant noms cooked et indices. Échantillonnage UV et premier
mesh texturé restent des étapes ultérieures.
