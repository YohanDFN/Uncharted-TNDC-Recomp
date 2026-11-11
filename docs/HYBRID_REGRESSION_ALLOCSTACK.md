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
