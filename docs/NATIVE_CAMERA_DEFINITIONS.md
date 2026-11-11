# Première définition caméra issue du PAK natif

Sous-système : game/loading, lecture des définitions CAMERA_TABLE_1.
Symptôme : les pages sont relogées, mais aucune donnée de caméra n'est
disponible dans une représentation native possédée.
Dernier point fonctionnel : liste de dix ressources et propriété LEVEL_INFO_1.
Premier point incorrect : seule la chaîne de type CAMERA_TABLE_1 est connue.
Hypothèse : le consommateur original établit une table de définitions dont
le nom et la position stockée peuvent être copiés avant le port des objets caméra.

## Preuve statique

- 48DEB2..48DEC3 attache metadata+20 à Level+988, si le propriétaire existe.
- 56F432..56F4D1 planifie le message F2ACC8EC avec la table en argument.
- 4350EA identifie ce message dans CameraManager (cameramanager.cpp dans la
  cartographie u2_engine_map.json, fonction englobante 434000).
- 43535C lit le nombre d'entrées ; 43536B lit le pointeur à table+8.
- 435376 lit le nom à record+0 ; 43538C..4353A4 recherche `_cam_start_`.
- 435A21..435A2C charge les trois floats à record+8/+C/+10 et leur ajoute
  une translation de contexte. Le nouveau lecteur conserve les valeurs
  stockées, sans revendiquer la position monde finale ni la vue caméra.
- 435B28 avance de 48 hexadécimal, soit 72 octets. La première lecture de ce
  pas avait été erronée ; elle a été corrigée avant commit et couverte par
  deux enregistrements adjacents. Le cas réel à une entrée ne détectait pas l'erreur.

## Résultat réel

Dans menu-temple, login[7] référence page0+5FA0. Le payload à 5FC0 contient
une entrée ; les données commencent à 5FD0. Le nom est à page0+6020 :
`|_cam_start_1|_cam_start_Shape1`. La position stockée décodée est :
`(-78.384804, 26.898600, -29.629000)`.

Après les relocations déjà validées, le lecteur contrôle le type, les limites
des enregistrements et les pointeurs de noms. Seules les références dans la
page fournie sont acceptées ; les références inter-pages restent non prises
en charge. Les noms doivent être terminés dans cette page, les positions
finies, les tailles et produits bornés avant toute lecture.

Le résultat est un vector de noms std::string et de positions float[3],
possédé par la requête native. L'accesseur GetNativeCamera retourne une copie
sous verrou ; ni cette copie ni les définitions ne conservent de pointeur PAK.
Un refus préserve la sortie précédente. L'écrasement du buffer source est
testé pour vérifier l'indépendance des données copiées.

Cette lecture est une préparation de données, pas l'exécution du callback
de login, la construction d'un Level ou d'une Camera, ni une mise en rendu.
Elle ne préempte pas l'ordre des dix logins. Le package reste Resolved,
LEVEL_INFO_1 reste la seule propriété appliquée, et neuf logins restent en attente.
Aucun détour hybride ni appel ELF n'est ajouté. Aucune page PS4 n'est exécutée.

Modification : ReadCameraDefinitions dans level_resources.cpp/.h ; intégration
après relocation dans package_manager.cpp, compteurs de décodage distincts
dans sa vue, et GetNativeCamera pour copier la définition.
Fichiers modifiés : ces quatre fichiers, u2_engine_tests.cpp, ce rapport et
UNCHARTED_NATIVE_PLAN.md.
Tests ajoutés : 17 assertions couvrant l'entrée réelle, le nom de départ,
les valeurs non nulles, les positions signées, le pas de deux enregistrements,
la publication atomique, les erreurs d'index, les NULL, les pointeurs étrangers,
les tailles tronquées, les comptes excessifs, le mauvais type, les noms non
terminés, NaN et l'indépendance du buffer source.
Build : deux exécutables réussis ; une erreur de macro de test a été corrigée
avant validation, sans modification du comportement moteur.
Test 6 secondes : SelfTest 886, full-body 831, moteur 235/235 et timeout,
sans faute, trap, assertion invitée ou échec d'allocation.
Test natif : 229/229 assertions, caméra réelle décodée, sortie 0 ; fermeture
par l'option Quitter pendant le test interactif avant le timeout de 10 s.
Test prolongé : 25 s sur la version finale, 235/235 assertions et timeout sans
faute ; MovieDecodeThread et ressources menu/menu-temple observés. Journaux
préservés dans diag_native_camera_hybrid_6s.log et diag_native_camera_hybrid_25s.log.
Régression observée : aucune dans les scénarios vérifiés ; les 889 détours
hybrides restent ceux de la référence restaurée.
Conclusion : la définition de départ du temple est désormais exploitable en
mémoire native ; le standalone n'affiche pas encore la scène/menu original.
Commit : ajout atomique de la lecture de définitions caméra.
Prochaine hypothèse : établir les champs d'orientation et d'objectif ainsi
que la transformation de niveau, puis leur consommateur natif ; ne pas
instancier un objet caméra guest ou forcer la phase de chargement.
