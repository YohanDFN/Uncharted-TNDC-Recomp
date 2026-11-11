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
