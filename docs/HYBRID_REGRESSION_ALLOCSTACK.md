# Première régression hybride introduite par le natif

Sous-système : corelib/memory, frontière de propriété hôte/guest.
Symptôme : disparition du menu original après développement du standalone.
Dernier point fonctionnel : commit 3d5dded, menu original confirmé visuellement
par l'utilisateur pendant le test de cette investigation.
Premier point incorrect : b50f534, premier commit d'implémentation native.
Hypothèse : AllocateAllocStack fournit un bloc hôte alors que sa libération
ELF attend une entrée de pool guest.

## Expériences isolées

Worktree de diagnostic : scratch/diag_baseline_3d5dded. Les sources de chacun
des commits ont été compilées intégralement avec MSYS2 Clang64, C++23,
RelWithDebInfo, BB_LTO=OFF et BB_PGO=off. Les sous-modules sont reliés aux
dépendances locales par junctions : mêmes dépendances pour les essais, mais
pas une reproduction historique de leurs révisions ; fsr-vulkan est modifié
dans l'espace utilisateur. Aucun fichier de ce sous-module n'a été édité.
Le lanceur et les scripts n'ont pas changé entre 3d5dded et HEAD ; le runner
diagnostique utilise le lanceur courant et son BB_PROBE pour sélectionner
l'exécutable isolé, sans remplacer out/bb-probe.exe.

| Sources / configuration | Résultat |
| --- | --- |
| 3d5dded, tous les détours | Menu original visible (utilisateur), 25 s, aucune faute ; SelfTest et 64/64 EngineTest passent |
| b50f534, tous les détours | Faute en ~1,8 s à 0xB35E47, lecture 0xFFFFFFFFFFFFFFFF ; SelfTest et 75/75 EngineTest avaient passé |
| b50f534, seul AllocateAllocStack exclu | Menu original visible (utilisateur), 25 s sans faute ; 893 détours au lieu de 894 |
| b50f534, contrôle filtre sans exclusion effective | Même faute à 0xB35E47 en ~2 s ; 894 détours |

Le contrôle filtre est nécessaire : BB_U2_DETOUR_ALLOW saute les tests
fonctionnels moteur. Le contrôle conserve donc tous les détours et les mêmes
conditions d'autotest que l'essai réussi. Il échoue malgré tout ; la différence
causale est bien l'exécution de l'AllocateAllocStack original.

Journaux conservés dans user/diag_compare_baseline_3d5dded.log,
user/diag_compare_first_native_b50f534.log,
user/diag_compare_b50_original_allocstack.log et
user/diag_compare_b50_filter_control.log.
Le wrapper peut sortir avec code 0 malgré une faute du probe : le journal
Fault prévaut sur ce code. Les timeouts ne sont pas des arrêts guest propres.

## Preuves statiques et cause

b50f534 ajoute cinq entrées moteur au registre partagé de bb-probe et du natif,
dont AllocateAllocStack à 0xB36D40. Sa version C++ crée une structure de 0x780
octets avec calloc, l'enregistre dans le TLS guest et la retourne.

L'original à 0xB36D40 prend un slot sous verrou dans un pool avec bitmap.
FreeThreadAllocStack à 0xB35D30 lit la pile active dans le TLS guest ;
0xB35E20..0xB35E47 calcule l'index depuis la base du pool, puis efface un bit
du bitmap. Le bloc calloc n'appartient pas au pool : le calcul produit un
index invalide et la libération faute à 0xB35E47.

Il s'agit d'une incompatibilité de provenance/durée de vie, pas d'un simple
problème de taille ou de convention d'appel. Les tests sandbox vérifiaient
la réimplémentation contre ses propres attentes et ne couvraient pas le couple
allocation hôte / libération originale.

Conclusion : première cause de régression démontrée par A/B et contrôle.
Elle précède les modifications de boot d'08f8668. Les défauts de montage et
de boot identifiés plus tard restent suspects séparés ; ce diagnostic ne
prouve pas que retirer un seul détour suffira pour HEAD.

Modification : aucun correctif de production appliqué. Ajout d'un runner
diagnostique et de ce rapport. Worktree isolé conservé pour reproduire les essais.
Fichiers modifiés : scratch/diag_hybrid_compare.py et ce rapport.
Tests ajoutés : les quatre expériences ci-dessus.
Build : 3d5dded et b50f534 réussis, backend compris.
Test 6 secondes : pas utilisé comme preuve du menu ; essais de 25 secondes.
Test prolongé : résultats dans le tableau.
Régression observée : b50f534 introduit la première régression ; état courant
inchangé par cette investigation.
Commit : diagnostic atomique, sans correction de runtime.
Prochaine correction proposée : conserver les allocations/libérations ELF
ensemble dans bb-probe et réserver le système mémoire hôte au standalone ;
valider ensuite les modifications partagées ajoutées après b50f534 une par une.

## Itération suivante : isolation de la famille AllocStack

Sous-système : frontière mémoire standalone / hybride.
Symptôme : huit routines de pile hôte remplacent encore le pool ELF ; les
appels C++ natifs peuvent aussi adopter ou modifier la pile TLS invitée.
Dernier point fonctionnel : A/B de b50f534 ci-dessus.
Premier point incorrect : enregistrement partagé et accès au TLS invité dans
`ndlib/memory/alloc_stack.cpp`.
Hypothèse : réserver la famille au standalone et son stockage au thread hôte
empêche de mélanger les deux propriétaires, y compris pendant les autotests.
Offsets étudiés : B36D40, B35D30, B360D0, B35EA0, B371A0, B361C0, B377B0,
B362A0 ; slot TLS invité TCB-0x20 établi par le runtime et l'ELF.
Preuve statique : pool/bitmap ELF incompatible avec calloc ; suppression des
accès TLS invités du C++ et des huit entrées du registre hybride.
Preuve runtime : tests de conservation du pointeur TLS et des 0x780 octets
d'une pile sentinelle après allocation, push, pop et libération répétée.
Les tests vérifient aussi les huit frontières du registre selon la cible.
Modification : définition CMake U2_STANDALONE pour le natif, registre mémoire
conditionnel, stockage exclusivement thread_local, libération du bloc hôte.
Les trampolines mémoire de test sont installés uniquement dans la sandbox.
Fichiers modifiés : gpu/CMakeLists.txt, alloc_stack.cpp, u2_engine_registry.cpp,
u2_engine_tests.cpp, ADR.md, UNCHARTED_NATIVE_PLAN.md et ce rapport.
Tests ajoutés : huit contrôles de registre et quatre contrôles TLS hybrides.
Build : deux exécutables compilés avec succès.
Test 6 secondes : un essai réussi, 903 SelfTest, 831 full-body et 199/199
assertions moteur ; STOPPED_AFTER_TIMEOUT sans faute.
Test prolongé : 20 s réussies, mêmes tests, sans Fault/Guest fault/Trap,
assertion nouvelle ni échec d'allocation ; journal diag_memory_after_20s.log.
Test natif : 195/195 assertions, fermeture à 10 s, code de sortie 0.
Un premier test TLS ajouté tentait aussi de créer un TCB dans le standalone
après l'initialisation graphique (échec de slot Windows <64) : corrigé en
réservant l'expérience de TLS invité à la cible hybride.
Conditions machine : certains essais, avant et après modification, échouent
avec l'erreur Windows 1450 sur une réservation de 6080 MiB puis fautent en
B309DB. Ils ne valident pas le boot ; aucun réglage système ni application
utilisateur n'a été modifié. Un contrôle 3d5dded de 20 s a réussi ensuite.
Régression observée : aucune faute dans le test prolongé, mais écran noir
confirmé par l'utilisateur. Fermeture finale provoquée par le timeout du test.
Conclusion : frontière mémoire isolée et vérifiée ; restauration du menu sur
HEAD non acquise. Les logs « Mounted » du montage réécrit ne prouvent pas le
montage effectif des archives et restent le prochain point à isoler.
Commit : correction mémoire atomique, indépendante du montage.
Prochaine hypothèse : le détour B783F0 omet le monteur original B72F00 ;
tester son exclusion seule avant toute autre correction de production.

Expérience complémentaire réalisée : exclure seulement
GameLoading_MountInitialArchives sur la branche corrigée ne restaure pas le
menu (écran noir confirmé par l'utilisateur), malgré 25 s sans faute et 905
détours. Le contrôle du filtre, sans exclusion effective, tient aussi 20 s
sans faute, avec 906 détours. Journaux :
diag_compare_memory_mount_original.log et diag_compare_memory_filter_control.log.
Conclusion limitée : rétablir B783F0 seul est insuffisant. Ne pas annoncer un
correctif du menu et ne pas remplacer une deuxième routine sans nouvelle
expérience. Prochaine recherche : progression du boot et créations de tâches
avec les autres ajouts natifs, comparées à la trace saine de 3d5dded.
