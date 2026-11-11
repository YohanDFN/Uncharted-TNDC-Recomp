# Divergence du boot et fiabilité du filtre de détours

Sous-système : boot, graphe de tâches et sélection diagnostique des détours.
Symptôme : écran noir sur 07bd6fe malgré la séparation des AllocStack.
Dernier point fonctionnel : les threads de chargement/audio démarrent ; le
chemin original 9B5C20 n'est appelé que deux fois sur les essais courants.
Premier point absent établi : entrée A59960 (initialisation/login du graphe de
tâches et de sa table), appelée deux fois sur 3d5dded et zéro fois sur la
branche actuelle pendant deux contrôles de 20 secondes.

## Comparaison instrumentée

Le runner `scratch/diag_hybrid_compare.py` accepte désormais `--hooks`.
Les prologues de 6 octets utilisés sont établis par désassemblage ciblé
(`push rbp; mov rsp,rbp; push r14/r15`). Les hooks comptent les appels et
capturent les arguments, sans remplacer les fonctions originales. Ils sont
installés avant les détours : pour observer une routine normalement détournée,
il faut aussi exclure son détour, sinon ce dernier écrase le hook.

| Expérience | Preuve runtime |
| --- | --- |
| 3d5dded, 25 s, hooks boot | ProcessMgr_Init : 1 appel ; 9B5C20 : 49 ; D59A50 : 1444 ; A551B0 : 703 ; MovieDecodeThread et SaveWorker présents ; aucune faute |
| ProcessMgr_Init original seul, 20/25 s | 1 appel original ; 9B5C20 : 2 ; pas de MovieDecodeThread ; aucune faute |
| Ajout de ProcessList_StartUp original, 25 s | 1 appel original de chaque init ; 9B5C20 : 2 ; aucune faute |
| Ajout du montage original, 25 s | Même absence de progression ; aucune faute |
| Settings_Init original seul | Division par zéro en A4BDFE, après environ 8 s |
| Settings_Init original avec processus et montage originaux | Même division par zéro, après environ 7 s |
| Streaming_ResolvePath original seul, 25 s | 224 appels originaux ; 9B5C20 : 2 ; aucune faute |
| Settings_NewSetting original seul, 25 s | 118 appels originaux ; 9B5C20 : 2 ; aucune faute |
| LookupPointer seul, avant correction du filtre | Essai invalide : 906 détours, exclusion inopérante |
| LookupPointer seul, après correction du filtre | 905 détours ; 86 appels originaux puis symboles absents, assertions A15F25 et faute 5C543A |
| 3d5dded, hook A59960 seul, 20 s | 2 appels ; MovieDecodeThread présent ; aucune faute |
| Branche courante, hook A59960 seul, 20 s | 0 appel ; 199/199 assertions moteur passent ; aucune faute |

Les journaux sont dans `user/diag_compare_boot_*.log`. Les deux derniers
contrôles sont `boot_baseline_graphlogin` et `boot_current_graphlogin`.
Les exclusions utilisent les mêmes conditions d'autotest filtré : les tests
moteur fonctionnels sont sautés, comme documenté dans le diagnostic mémoire.
Les deux contrôles A59960 sans filtre conservent les tests fonctionnels.

## Preuves statiques et limites

Hypothèse initiale : la construction des processus ne remplit plus les
dépendances des tâches. Les exclusions montrent que la restaurer seule, ou
avec ProcessList et le montage, ne suffit pas ; aucune de ces configurations
ne constitue un correctif validé du menu.

A4BDF5 lit le nombre de buckets à 153C03C ; A4BDFE divise par ce nombre.
Le C++ Settings_Init le remplaçait artificiellement par 1 avec un bucket vide.
L'échec avec Settings_Init original révèle donc une dépendance absente,
masquée par cette compensation. Ne pas rétablir une table factice.

A59B46..A59B9B, dans la routine A59960, alloue et configure la véritable table
(153C000..153C03C). Deux appels directs sont repérés : 9FE8F7 (binding
DcGameTaskGraphLogin) et A5A95C. La cause qui empêche d'atteindre ces appelants
sur la branche actuelle reste à établir. Ne pas appeler A59960 directement
comme contournement : ses données et son contexte doivent être construits.

NewSetting à A1CD50 consomme aussi un argument sur pile à rbp+10 et utilise
edi comme identifiant entier ; la déclaration native à cinq arguments avec
`void *self` ne décrit pas le contrat original complet. L'exclusion seule ne
rétablit pas la progression : pas de réécriture de cette ABI pendant ce tour.

## Correction validée pendant l'investigation

Un défaut distinct rendait l'isolation de certains noms impossible :
`strstr(allow_env, name)` autorisait LookupPointer quand seul
LookupPointerInModule était conservé. Hypothèse falsifiable : le nombre de
détours doit passer de 906 à 905 lorsque LookupPointer seul est exclu.
Après comparaison exacte des tokens séparés par des virgules, c'est le cas,
et le hook original devient actif (86 appels). Le contrôle précédent est
explicitement invalidé, sans remettre en cause les exclusions sans collision.

Modification : comparaison exacte dans `runtime_u2_detour_filter.h`, utilisée
par le dispatcher ; option --hooks et restitution des compteurs dans le runner.
Fichiers modifiés : runtime_u2_script.c, runtime_u2_detour_filter.h,
test_u2_detour_filter.c, gpu/CMakeLists.txt, diag_hybrid_compare.py,
DETOUR_VALIDATION.md et ce rapport.
Tests ajoutés : 12 assertions sur noms préfixes, tokens en début/milieu/fin,
liste vide/NULL et séparateurs répétés ; cible u2-detour-filter-test.
Build : bb-probe et standalone compilés avec succès.
Test 6 secondes : SelfTest 903, full-body 831, moteur 199/199,
STOPPED_AFTER_TIMEOUT ; aucune faute, assertion invitée ou erreur mémoire.
Test prolongé : contrôle courant A59960 de 20 s, mêmes assertions, sans faute.
Régression observée : aucune sans filtre ; le correctif change exclusivement
la sélection demandée par BB_U2_DETOUR_ALLOW. Le menu reste non restauré.
Conclusion : sélection diagnostique réparée et premier stade absent mesuré ;
cause du blocage de boot encore non prouvée. Aucun changement de boot appliqué.
Commit : correction atomique du filtre et preuves de diagnostic.
Prochaine hypothèse : la tâche qui doit atteindre A5A95C n'achève pas son
initialisation ; instrumenter son entrée et ses dépendances avant/après login,
en comparaison avec la base saine. Préserver l'ELF et les journaux actuels.
