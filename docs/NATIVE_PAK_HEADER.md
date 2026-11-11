# Lecture native du préfixe PAK

Sous-système : ndlib/io, registre natif de packages.

Symptôme : les PAK sont résolus sans aucune lecture de leur contenu.

Dernier point fonctionnel : ouverture VFS et taille réelle des deux PAK menu.

Premier point incorrect : aucune inspection du préfixe ou de la table des pages.

Hypothèse : les champs employés par GetPakHdrPageEntry permettent de borner
une première lecture sans interpréter les pointeurs internes des pages.

Offsets étudiés : 0x00B6DE20..0x00B6DE8A et 0x00B69774..0x00B697DD.

Preuve statique : GetPakHdrPageEntry compare index avec le uint32 à +0x10,
charge l'offset signé à +0x14, l'ajoute au pointeur de base fourni par le
deuxième argument, puis ajoute index * 12. Son appelant dans le chargement
de header reproduit ce calcul. L'assertion identifie m_pageCt et package-util.cpp.

Preuve runtime : menu.pak et menu-temple.pak contiennent tous deux 2 à +0x10
et 0x2C à +0x14. Le natif lit exactement 24 octets de préfixe puis chaque
entrée de 12 octets via pread. Les journaux affichent header=inspected,
pages=2, table=0x2C pour les deux fichiers.

Conclusion : première lecture bornée effectuée ; cela ne prouve ni la validité
complète du format ni la disponibilité des ressources moteur. Les autres
champs du préfixe et le contenu des entrées ne sont pas interprétés.

Modification : inspection sans allocation de taille contrôlée par le fichier,
rejet des offsets signés négatifs et des tables hors fichier, contrôle exact
des lectures. L'état reste resolved, avec header_inspected séparé.

Fichiers modifiés : package_manager.cpp, package_manager.h,
u2_engine_tests.cpp, UNCHARTED_NATIVE_PLAN.md et ce rapport.

Tests ajoutés : vrai header menu-temple, table finissant exactement à EOF,
table tronquée, préfixe tronqué, offset négatif, préfixe NULL.

Build : build_msys2.ps1 réussi.

Test 6 secondes : SelfTest et EngineTest passent ; 153/153 assertions C++23.
Test natif 6 secondes : header=inspected pour les deux packages, sortie 0.

Test prolongé : inutile pour une lecture synchrone au boot.

Régression observée : aucune dans les scénarios de 6 secondes.

Commit : commit atomique feat(native) accompagnant ce rapport.

Prochaine hypothèse : identifier les champs des entrées de pages dans le
lecteur original avant toute lecture de payload ou relocation. La signature
globale et la version du PAK restent à établir ; inspected ne signifie pas validé.

## Itération suivante : première page menu-temple

Sous-système : ndlib/io.

Symptôme : aucune page PAK n'était conservée en mémoire native.
Dernier point fonctionnel : préfixe et table lus.
Premier point incorrect : absence de lecture du payload.
Hypothèse : l'entrée 0 décrit une étendue fichier dont la taille est répétée
dans l'en-tête de page à +0x0C.

Offsets étudiés : 0x00B697D8 (taille entrée +4), 0x00B69C60..0x00B69C78
(entrée +0 comparée à la position consommée du flux),
0x00B69EAC..0x00B69EF1 (page +0x0C égale pageEntry.m_pageSize).
Le champ +8 est discriminé à 0x00B6982F ; sa sémantique complète reste inconnue.
La preuve statique vient du même appelant LoadingHeader/LoadingPages identifié
dans u2_engine_map.json et package-mgr.cpp/package-util.cpp.

Preuve runtime : entrée 0 de menu-temple à 0x2C : offset 0x8280,
taille 0x1E8174, champ +8 nul. Lecture VFS exacte de 1 999 220 octets ;
page+0x0C contient 0x1E8174. Journal : page0=verified.

Conclusion : cette étendue brute est cohérente avec le lecteur original.
Cela n'établit pas les règles générales de compression, les relocations,
les autres champs de page ou la validité complète du PAK.

Modification : conservation de la première page dans un vector hôte appartenant
à la requête, libéré avec elle ; contrôle EOF par soustraction, lecture exacte,
taille minimale 16, égalité des tailles. Champ +8 différent de zéro refusé.
Budget d'inspection hôte de 4 Mio, sans prétendre à une limite du format original.
L'état du package reste resolved. Aucun détour ELF ajouté.

Fichiers modifiés : package_manager.cpp/.h, u2_engine_tests.cpp et ce rapport.
Tests ajoutés : page réelle, fin exacte à EOF, dépassement EOF, taille d'en-tête
incohérente, payload court, champ +8 non supporté.
Build : réussi.
Test 6 secondes : SelfTest/EngineTest passent, 159/159 assertions C++23,
STOPPED_AFTER_TIMEOUT ; natif sortie 0.
Test prolongé : non nécessaire, lecture synchrone au boot.
Régression observée : aucune dans les deux scénarios de six secondes.
Incident de validation : un premier hybride lancé en concurrence avec le test
natif a échoué à allouer 6080 Mio (Windows 1450), puis a produit une faute.
Le résumé du harnais indiquait pourtant un timeout ; le journal complet a
permis de détecter l'échec. Relancé seul, le test hybride passe sans erreur
d'allocation, assertion ou faute. La concurrence est une explication plausible,
pas une cause établie par ces deux essais.
Commit : feat(native), atomique avec ce rapport.
Prochaine hypothèse : identifier les tables internes et la procédure de
relocation de la page avant de publier ses ressources au moteur.

## Itération : prévalidation des relocations

Sous-système : ndlib/io.
Symptôme : page brute sans connaissance des pointeurs à reloger.
Dernier point fonctionnel : page 0 conservée et taille vérifiée.
Premier point incorrect : table de fixups non examinée.
Hypothèse : les enregistrements de FixUpPakPointers permettent de valider
les futurs emplacements d'écriture sans modifier la page.

Offsets étudiés / preuve statique : 0x00B6E800..0x00B6E928,
FixUpPakPointers (package-util.cpp, assertion répertoriée dans engine_map).
Arguments SysV : rdi header, rsi bases des données de tables, rdx bases des
pages à reloger. Header+0x18 doit valoir 8 ; header+0x1C est un offset signé
vers huit descripteurs de 12 octets (index de base, offset signé, compte).
Chaque fixup fait 8 octets : uint16 destination (bit 15 spécial), int16 base
cible, int32 emplacement. L'original ajoute la base cible au uint64 à cet
emplacement. Destination absente : ignorée ; cible absente : sentinelle 0xDEB060.
Le mode spécial déclenche une assertion ; il n'est pas implémenté ici.

Preuve runtime : menu-temple a un groupe actif, données à header+0xB0,
4148 enregistrements. 4088 concernent destination 0 / cible 0 ; 60 concernent
destination 1 / cible 0. La prévalidation native vérifie les 4088 emplacements
(alignement 8, étendue de 8 octets valide) et leurs offsets cibles dans page 0.
Elle compte séparément les 60 emplacements de page 1 non encore chargée.
Journal : fixups audited local=4088 pending=60 (no writes).

Conclusion : plan local vérifié, pas de relocation appliquée. La page 1 manque
pour contrôler les 60 autres emplacements. Le format général et les modes
spéciaux restent hors couverture ; ce jalon ne publie aucune ressource moteur.

Modification : audit en lecture seule du sous-ensemble observé, refus des
indices/modes inconnus, bornes des tables et offsets signés contrôlées ; limite
hôte d'inspection de 4 Mio aussi sur le bloc d'en-tête conservé temporairement.
Les compteurs ne sont publiés qu'après succès complet de l'audit.
Fichiers modifiés : package_manager.cpp/.h, u2_engine_tests.cpp, ce rapport.
Tests ajoutés : vrais 4088/60, cas local synthétique, emplacement hors page,
cible hors page, table tronquée.
Build : réussi.
Test 6 secondes : SelfTest/EngineTest, 164/164 assertions, timeout propre.
Test natif : sortie 0, audit 4088/60 confirmé.
Test prolongé : non nécessaire pour cet audit synchrone.
Régression observée : aucune dans ces scénarios.
Commit : feat(native), atomique avec ce rapport.
Prochaine hypothèse : charger et vérifier page 1, compléter les bornes des
60 emplacements, puis préparer une relocation transactionnelle à exécution unique.

## Itération : page 1 et application des relocations

Sous-système : ndlib/io.
Symptôme : 60 emplacements non vérifiés car page 1 absente.
Dernier point fonctionnel : audit page 0, 4088 locaux / 60 en attente.
Premier point incorrect : page 1 et pointeurs absolus absents.
Hypothèse : la seconde entrée permet de lire page 1 puis de valider toutes
les écritures du sous-ensemble observé avant de les appliquer une fois.
Offsets étudiés : 0x00B6E840..0x00B6E908, mêmes preuves que l'itération précédente.
Preuve statique : 0xB6E88C charge l'index de base cible, 0xB6E895 l'emplacement
signé ; 0xB6E89D ajoute la base au uint64 de destination.
Preuve runtime : page 1 à 0x1F0400, taille 696 (0x2B8), champ +8 nul,
taille à page+0x0C identique. 4148 écritures validées et appliquées ; journal
page1 verified bytes=696; relocated=4148.

Conclusion : les deux pages du package menu-temple contiennent désormais les
pointeurs absolus correspondant au sous-ensemble observé de FixUpPakPointers.
Il ne s'agit pas du login des ressources moteur ni de leur enregistrement.
L'état du package reste resolved et pointers_relocated indique ce jalon séparément.

Modification : page 1 bornée et lue exactement, construction d'un plan
d'écritures, validation de tous les slots/cibles et rejet des doublons avant
la première mutation. Toutes les écritures utilisent memcpy sur huit octets.
Un drapeau empêche la seconde application. Les vecteurs sont possédés par la
requête statique et ne sont plus redimensionnés après succès ; déduplication
des demandes conserve leurs adresses. Le reset de tests détruit les pages ;
aucun consommateur externe ne reçoit actuellement leurs pointeurs.
Modes spéciaux, indices autres que ceux observés et bases manquantes restent refusés.

Fichiers modifiés : package_manager.cpp/.h, u2_engine_tests.cpp, ce rapport.
Tests ajoutés : vrai package relogé, échec tardif sans mutation des deux pages,
adresses locales et inter-pages exactes, seconde application refusée.
Build : réussi.
Test 6 secondes : SelfTest/EngineTest passent, 169/169 assertions ; timeout propre.
Test natif 6 secondes : sortie 0 et 4148 relocations.
Test prolongé : non nécessaire pour cette opération synchrone.
Régression observée : aucune dans ces scénarios.
Commit : feat(native), atomique avec ce rapport.
Prochaine hypothèse : comprendre le login et les descripteurs de ressources
de page (notamment les 538 entrées indiquées à page0+0x12), avant publication
au moteur ou exécution de scripts.

## Itération : catalogue de types de ressources

Sous-système : ndlib/io.
Symptôme : pages relogées sans inventaire natif consultable des métadonnées.
Dernier point fonctionnel : 4148 fixups appliqués sur deux pages.
Premier point incorrect : descripteurs de page non examinés.
Hypothèse : la boucle de normalisation des métadonnées permet de lire les
types de ressources avant leur login spécifique au moteur.

Offsets étudiés / preuve statique : 0xB69FB0..0xB6A026 (compte uint16 à
page+0x12, descripteurs de 16 octets à +0x14, offset signé des métadonnées à
descripteur+8). L'original ajoute la base page aux deux premiers uint64 des
métadonnées, puis calcule deux StringId. Ces chaînes sont examinées avant
normalisation ; leurs offsets ne doivent pas être confondus avec des pointeurs
absolus produits par les fixups généraux.

0xB6B9F2..0xB6BA49 : le login utilise une autre liste de couples
index-page/offset, puis appelle le callback global à 0x12441C0. Le corps de
BigPackageLoginResItem (repéré par les assertions dans u2_engine_map.json)
implique notamment des dépendances de géométrie. Ce login reste à étudier.

Preuve runtime : 538 types lus dans les descripteurs de page0 de menu-temple,
dont CAMERA_TABLE_1 trouvé exactement une fois. Les premières chaînes de type
comprennent COLLISION_DATA_HAVOK_BG et DEBUG_INFO_1. Le journal confirme
resource catalog entries=538 (login pending).

Conclusion : catalogue hôte consultable par type, pas un registre moteur de
ressources initialisées. Aucun callback de login ni constructeur moteur appelé.
La liste de login n'est pas encore décodée et peut différer de cet inventaire.

Modification : lecture bornée des descripteurs, métadonnées et deux chaînes
terminées dans la page. Copie des types dans des std::string possédés par la
requête. Publication après succès complet et relocation réussie ; catalogue
effacé si la relocation est refusée. L'état du package reste resolved.
Fichiers modifiés : package_manager.cpp/.h, u2_engine_tests.cpp et ce rapport.
Tests ajoutés : catalogue réel 538, recherche CAMERA_TABLE_1, cas synthétique,
offset de chaîne invalide avec sortie préservée, chaîne non terminée,
descripteurs tronqués.
Build : réussi.
Test 6 secondes : SelfTest et EngineTest passent, 175/175 assertions,
STOPPED_AFTER_TIMEOUT ; natif sortie 0.
Test prolongé : non nécessaire pour cette lecture synchrone.
Régression observée : aucune dans ces scénarios.
Commit : feat(native), atomique avec ce rapport.
Prochaine hypothèse : identifier la liste de login et la résolution du callback
BigPackageLoginResItem ; choisir un type dont l'initialisation est entièrement
établie avant d'enregistrer une ressource comme utilisable par le moteur.

## Itération : liste ordonnée de login

Sous-système : ndlib/io, préparation du login moteur.
Symptôme : catalogue de 538 types sans liste des callbacks réellement demandés.
Dernier point fonctionnel : catalogue et pages relogées.
Premier point incorrect : absence de résolution de la racine et de sa liste.
Hypothèse : la boucle DoLogin décrit une liste ordonnée distincte du catalogue.

Offsets étudiés / preuve statique : 0xB6B9F2..0xB6BA49. La racine est résolue
par le couple signé page/offset (Package+0x240/+0x244, soit PakHdr+8/+12).
Le nombre signé est à racine+0x20, les couples page/offset de métadonnées
à racine+0x28, stride 8. Le callback global 0x12441C0 reçoit en SysV
rdi=package, rsi=métadonnées, rdx=base de la page référencée.
0x48D920 (BigPackageLoginResItem, data-loading.cpp) lit le type à metadata+8,
calcule son StringId puis choisit une branche. Les routines par type ne sont
pas encore suffisamment établies pour leur initialisation native.

Preuve runtime : header racine page1+0x30, liste de 10 couples, tous vers page0.
Ordre observé : LEVEL_INFO_1, DEBUG_INFO_1, LEVEL_PATS_1, EFFECT_TABLE,
TEXTURE_TABLE, LEVEL_OFFSET_1, GEOMETRY_1, CAMERA_TABLE_1, LIGHT_TABLE_1,
COLLISION_DATA_HAVOK_BG. Ces dix entrées sont lues par le natif et consultables
par index. Les logs de cette liste sont bornés par son compte validé.

Conclusion : les 538 descripteurs ne sont pas 538 appels de login. Liste
préparée, callbacks encore pending ; aucune ressource moteur initialisée.
Modification : lecture des couples et des chaînes de type avant relocation,
indices et offsets signés contrôlés, limites des pages et chaînes vérifiées.
La sortie est publiée seulement après succès ; elle est effacée si relocation
échoue. Les chaînes copiées vivent avec la requête (reset de tests les détruit).
Fichiers modifiés : package_manager.cpp/.h, u2_engine_tests.cpp, ce rapport.
Tests ajoutés : nombre réel 10 et ordre premier/dernier, index hors limites,
couple synthétique, page absente avec sortie préservée, table tronquée.
Build : réussi.
Test 6 secondes : SelfTest/EngineTest passent, 181/181 assertions et timeout.
Test natif : sortie 0, liste ordonnée confirmée.
Test prolongé : non nécessaire pour cette lecture synchrone.
Régression observée : aucune dans ces scénarios.
Commit : feat(native), atomique avec ce rapport.
Prochaine hypothèse : suivre spécifiquement la branche LEVEL_INFO_1 dans
BigPackageLoginResItem et établir ses dépendances avant son implémentation.

## Itération : première propriété de niveau native

Sous-système : game/loading (data-loading.cpp).
Symptôme : les dix logins étaient tous en attente.
Dernier point fonctionnel : liste ordonnée validée.
Premier point incorrect : aucun payload transféré vers un état de niveau natif.
Hypothèse : la branche LEVEL_INFO_1 ne fait qu'une copie de quatre octets,
donc elle peut être reproduite sans initialiser les autres sous-systèmes.

Offsets étudiés / preuve statique : 0x48EF3B..0x48EF61, au sein de
BigPackageLoginResItem identifié par les assertions data-loading.cpp dans
u2_engine_map.json. Hash CRC du type avec la table originale : 0xDD7F3F25.
L'original lit package+0x28 (Level*), ignore le cas NULL, lit uint32 metadata+0x20
et écrit uint32 Level+0xA50. Aucun autre effet dans cette branche.

Preuve runtime : login[0] référence page0+0x3CB80 ; type LEVEL_INFO_1,
payload de quatre octets nul. Le standalone journalise le transfert de 0
vers U2NativeLevelInfo.value et loaded=true. Un test non nul 0x12345678
vérifie que cette implémentation ne force pas arbitrairement zéro.

Conclusion : une propriété issue du payload est maintenant initialisée dans
un état hôte possédé par la requête. Cet état représente uniquement le champ
établi ; ce n'est pas un objet Level guest construit, ni un niveau complet.
Aucun consommateur de rendu n'est encore relié à cette propriété.

Modification : helper game/loading/level_resources.cpp/.h, validation du type
exact et du payload, copie little-endian. Aucun offset n'est écrit dans un
objet guest, aucune adresse ELF n'est appelée, aucun détour ajouté. La liste
validée fournit la page et l'offset ; les neuf autres logins restent en attente.
Durée de vie : propriété possédée par la requête, détruite à son reset.
Fichiers modifiés : level_resources.cpp/.h, package_manager.cpp/.h,
u2_engine_tests.cpp et ce rapport.
Tests ajoutés : vraie propriété nulle, payload non nul, double application,
payload tronqué, mauvais type, niveau absent.
Build : réussi.
Test 6 secondes : SelfTest/EngineTest passent, 187/187 assertions, timeout propre.
Test natif : sortie 0 et LEVEL_INFO_1 loaded.
Test prolongé : inutile pour ce transfert synchrone.
Régression observée : aucune dans ces scénarios.
Commit : feat(native), atomique avec ce rapport.
Prochaine hypothèse : établir le constructeur et la durée de vie du niveau
natif, puis relier un type effectivement consommé (caméras/géométrie) ;
la branche CAMERA_TABLE_1 (0x48DEAB..0x48DECA) référence Level+0x988 et
requiert donc un propriétaire Level cohérent, pas seulement cette propriété.
