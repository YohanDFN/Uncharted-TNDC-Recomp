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
