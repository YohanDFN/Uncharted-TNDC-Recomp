# Translation de niveau appliquée à la position caméra native

Sous-système : game/loading et game/camera, données possédées du standalone.
Symptôme : la position caméra reste celle du PAK, sans le décalage que
CameraManager ajoute dans le jeu original.
Dernier point fonctionnel : vecteurs caméra et quaternion natif comparé à l'ELF.
Premier point incorrect : absence de lecture de LEVEL_OFFSET_1 et d'addition
de sa translation à la position stockée.
Hypothèse : le deuxième argument du message F2ACC8EC est le Level dont les
trois floats à +20/+24/+28 viennent de LEVEL_OFFSET_1.
Offset(s) étudié(s) : 48D920, 48D9F9, 48E9E5..48EA30,
56F432..56F4D1, 4350F5..435119, 4352E5..435307 et 435A11..435A5D.

Preuve statique :

- La table CRC existante donne A0C08245 pour LEVEL_OFFSET_1, et retrouve
  aussi les identifiants déjà établis CAMERA_TABLE_1 et LEVEL_INFO_1.
- 48D93B conserve les métadonnées dans rbx ; 48D9F9 les sauvegarde à
  rsp+78. À 48E9E5, A0C08245 sélectionne la branche qui charge le propriétaire
  Package+28 et vérifie sa présence. Les floats metadata+20/+24/+28 sont
  regroupés puis écrits dans Level+20 à 48EA2B.
- 56F461 place le Level r15 dans le deuxième argument typé du message
  F2ACC8EC, après la table caméra.
- 4350F5..435119 extrait le deuxième argument à +18 dans la liste typée.
  435304 charge son pointeur ; 435307 le conserve à rbp-2D8.
- 435A11 recharge ce Level ; 435A18 lit Level+20. 435A21..435A2C ajoute
  record+8/+C/+10 ; 435A4D sauvegarde les composantes obtenues. Ce chemin
  est celui sans transformation externe, déjà établi au jalon précédent.

Preuve runtime : l'entrée login[5] de menu-temple pointe sur page0+3CBC0.
Ses douze octets de payload à +20 sont nuls. La lecture native produit donc
une translation (0,0,0), issue du fichier et jamais choisie comme valeur de secours.
Le diagnostic XMM est étendu à xmm1 sans modifier le registre invité.

Reproduction de la capture originale :

```
C:/msys64/clang64/bin/python.exe scratch/diag_hybrid_compare.py --probe out/gpu/bb-probe.exe --label level_offset_original --seconds 25 --hooks 0x48EA2B:5,0x435A21:7,0x435A5D:10
```

Les instructions déplacées sont des instructions complètes sans adressage RIP :
un vmovaps de cinq octets, un vmovq de sept octets et un mov immédiat de dix
octets. Leurs chemins ne consomment pas les flags altérés par le compteur
préexistant du hook. Le diagnostic ne s'installe qu'avec BB_GUEST_HOOKS.

Le rapport observe deux passages du login LEVEL_OFFSET_1 : seule sa dernière
valeur est conservée, on ne prétend donc pas identifier les deux ressources.
Le consommateur caméra est en revanche appelé une seule fois : xmm1 avant
l'addition vaut zéro, puis contient les mots suivants après l'addition :

```
41d73055c29cc505 41d73055c1ed0831
xyz = (-78.384803772, 26.898599625, -29.628999710)
```

Seules xyz constituent la position établie ; la quatrième lane répétée ne
doit pas être interprétée comme une coordonnée homogène. Les trois composantes
natives sont comparées bit à bit à cette capture. Les autres niveaux et les
transformations externes n'ont pas été validés dynamiquement par cette expérience.

Conclusion : le lien ressource → Level → addition caméra est établi. Le lecteur
natif conserve un U2NativeLevelOffset possédé et son état decoded ; le calcul
caméra conserve translated_position et position_translated. L'absence de ressource,
le chemin externe ou un résultat non fini ne produisent aucune position de secours.
Les refus préservent la sortie précédente. Le nom translated_position décrit
cette addition précise sans revendiquer toutes les transformations du rendu.
Modification : lecture de LEVEL_OFFSET_1 après relocation, copie accessible sous
verrou et calcul de position dans game/camera/camera_position.cpp. Il s'agit de
préparation de données, sans appliquer le login moteur ni changer son ordre.
Fichiers modifiés : level_resources.h/.cpp, camera_position.h/.cpp,
package_manager.h/.cpp, u2_engine_tests.cpp, probe.c, plan natif et ce rapport.
Tests ajoutés : 15 assertions couvrant la ressource réelle, la position ELF,
une translation signée non nulle, la copie possédée, les bornes et le type de
ressource, NULL, NaN/Inf, débordement du résultat, offset absent et transform externe.
Build : les deux exécutables compilés et déployés avec build_msys2.ps1.
Test 6 secondes : SelfTest 886, full-body 831, moteur 268/268 et
STOPPED_AFTER_TIMEOUT sans faute ni nouvelle assertion invitée.
Test natif : 262/262 ; translation réelle lue et position obtenue ; fermeture
au délai de six secondes, sortie 0.
Test prolongé : 25 secondes avec les trois points de capture sur la version finale.
268/268 assertions, STOPPED_AFTER_TIMEOUT, zéro faute/assertion/erreur
d'allocation signalée ; mêmes bits de translation et de position que la capture initiale.
Régression observée : aucune dans les tests vérifiés ; 889 détours hybrides
inchangés, aucun calcul caméra original remplacé et aucun appel .text PS4 natif.
Journaux : user/diag_level_offset_baseline.log, diag_level_offset_build.log,
diag_level_offset_native.log, diag_level_offset_hybrid_6s.log,
diag_level_offset_hybrid_console.log, diag_compare_level_offset_original.log
et diag_compare_level_offset_final.log.
Commit : feat(native): apply decoded level translation to camera position.
Prochaine hypothèse : identifier le consommateur des paramètres de projection
et leur convention avant d'ajouter une caméra de rendu native.
Les packages restent Resolved ; LEVEL_INFO_1 reste la seule propriété appliquée
et neuf logins moteur restent en attente. Le menu original n'est pas encore rendu
par le standalone.
