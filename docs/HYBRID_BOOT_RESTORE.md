# Restauration du chemin de boot hybride

## Preuve de données privées non initialisées

Sur 7418aa2, A5A270 est appelée une fois avec edi=1. Elle résout
`LookupPointerInModule(6E51B48F, 67723261)` puis parcourt les tâches.
L'exclusion de ce résolveur seul révèle trois assertions A1734D et une faute
A572EF : remplacer seulement la recherche ne restaure pas ses dépendances.

Le symbole 6E51B48F est unique dans les 113 fichiers DC inspectés :
`dc1/ps4/tasks.bin`, offset 98. SHA256 du fichier :
`b50a0a85d74be7f90e3a13f5293f4771563ccace6978bb98d74a90c3d0838a6d`.
La table a 24 groupes et 492 tâches. Tous les champs +88 sont zéro et aucun
n'est marqué dans le bitmap de relocation : relocaliser ne les initialise pas.

Une trace runtime ponctuelle confirme 24 groupes, 492 tâches et 492 champs +88
nuls dans la copie hôte renvoyée, avec chaque lecture bornée aux vecteurs du
préchargeur. Le helper original A17020, dont l'ABI à deux uint32 est établie
par A17280 et son corps, retourne NULL pour cette même paire, dans le même
contexte guest. La liste de modules guest est pourtant non vide.
Journal : `user/diag_compare_task_lookup_identity.log`.

A59FAB, dans le login A59960, écrit le pointeur de liaison à +88. Cette routine
n'est pas appelée sur la branche cassée ; ne pas fabriquer ces pointeurs ni
l'appeler hors du login normal. L'instrumentation temporaire a été retirée.
La comparaison baseline A5A270 est négative dans sa fenêtre de 20 s : ce
chemin de préparation ne doit pas être présenté comme l'unique appelant
attendu au boot. Le login sain peut venir du binding DcGameTaskGraphLogin.

## Isolement historique de 08f8668

Les sources sont compilées dans le worktree diagnostique, dépendances locales
identiques à celles des expériences précédentes (dont fsr-vulkan utilisateur).
Le binaire sain 3d5dded est conservé sous `bb-probe-3d5dded.exe`.
AllocateAllocStack est exclu dans tous ces essais, selon la cause mémoire
déjà démontrée. Les restaurations suivantes sont cumulatives et diagnostiques.

| Configuration historique | Observation |
| --- | --- |
| Boot natif de 08f8668 | 20 s sans faute, bloqué après SubmitDone Keep-Alive |
| Montage original ajouté | Faute en 9B5C72 : racine ProcessMgr NULL, lecture +34 |
| ProcessMgr original ajouté | Premier login A59960 rétabli, pas de boucle D59A50 |
| UCCApp, ProcessList, NewSetting puis Settings originaux | Pas de reprise supplémentaire démontrée |
| PlayGoWait original ajouté | Second login, SaveWorker, MovieDecodeThread et boucle D59A50 rétablis |
| IntroState original ajouté | Boucle stable : D59A50 1431 appels, A551B0 705, 25 s sans faute |

Deux essais intermédiaires state/processlist se sont partiellement chevauchés ;
ils ne servent pas à établir une causalité. Les contrôles décisifs montage,
ProcessMgr, attente puis Intro sont séquentiels. Les logs sont conservés sous
`user/diag_compare_first_boot_*.log`. Une fermeture à 25 s vient du runner.
Le stub PlayGoWait historique retourne sans le travail ni les notifications
du corps B78790..B7909F. Le montage natif ne monte pas réellement l'archive.
Les constructions natives de ProcessMgr et ProcessList ne remplissent pas
les dépendances d'objets originales. Il ne suffit pas de rétablir un appel
isolé quand le propriétaire de ses données reste remplacé.

## Contrôle de la branche actuelle

Avec les huit routines de boot originales, la copie DC privée provoque encore
une division par zéro A4BDFE. Avec les deux résolveurs DC originaux en plus,
le premier login réapparaît, mais la boucle principale ne démarre pas.
Avec uniquement les 889 détours de référence, la boucle reste également absente.

Les deux patchs StateTask A5301B et A5A4C0, ajoutés après la baseline, sont
appliqués avant les filtres de détours. En conservant exactement les mêmes
889 détours et en laissant ces deux plages à l'ELF, le second login, le thread
vidéo et la boucle D59A50 reprennent (1434 appels / 25 s, sans faute).
Contrôles : `diag_compare_current_baseline_registry.log` puis
`diag_compare_current_baseline_original_checks.log`.
Le commutateur diagnostique temporaire a été retiré après cette expérience.

## Itération 1 — frontière du registre

Sous-système : installation des prototypes standalone.
Symptôme : des objets hôte et des approximations de boot remplacent l'ELF.
Dernier point fonctionnel : contrôle 889 détours et instructions originales.
Premier point incorrect : registre unique mêlant référence et prototypes.
Hypothèse : deux registres par cible empêcheront la réintroduction de ces
contrats dans bb-probe, tout en conservant les prototypes pour le natif.
Offsets étudiés : B783F0, 9B9530, B78790, A551B0, D59A50, A17020..A1736D,
A5A270..A5A961, A59960..A59FDF, A5301B et A5A4C0.
Preuve statique : copies privées DC, ABI NewSetting différente, pool invité
et cycle de vie original non reproduits ; pas de nouvelle décompilation.
Preuve runtime : expériences ci-dessus et 25 assertions de frontière du registre.
Modification : 55 entrées hybrides ; 25 descriptions natives installées
seulement dans le standalone ou dans l'image isolée des tests.
Fichiers modifiés : u2_engine_registry.cpp, u2_engine.h, u2_engine_tests.cpp,
ADR.md, UNCHARTED_NATIVE_PLAN.md et ce rapport.
Tests ajoutés : 17 contrôles de frontière supplémentaires (25 au total).
Build : deux exécutables réussis.
Test 6 secondes : 889 détours, SelfTest 886, full-body 831, moteur 216/216,
STOPPED_AFTER_TIMEOUT ; aucune faute, assertion invitée ou erreur mémoire.
Test natif : 80 méthodes moteur, 212/212 assertions, sortie 0 après 10 s.
Test prolongé : contrôles de 25 s ci-dessus ; patchs StateTask encore actifs
dans cette première itération, donc restauration complète non revendiquée.
Régression observée : aucune nouvelle faute ; blocage restant déjà isolé.
Conclusion : frontière du registre vérifiée indépendamment des patchs.
Commit : correction atomique du registre par cible.
Prochaine hypothèse : retirer les deux patchs StateTask restaurera la
progression constatée dans le contrôle, avec les autotests complets actifs.

## Itération 2 — retour aux contrôles StateTask originaux

Sous-système : patchs automatiques du code de tâches Data-Core.
Symptôme : avec le registre hybride isolé, le premier login se produit mais
la boucle d'états et le thread vidéo ne démarrent pas.
Dernier point fonctionnel : registre hybride de 55 méthodes / 889 détours.
Premier point incorrect : les deux substitutions automatiques A5301B et
A5A4C0 restent appliquées avant les filtres, malgré l'exclusion des prototypes.
Hypothèse : conserver les instructions ELF de ces deux plages suffit à
retrouver la progression saine une fois le registre séparé.
Preuve statique : les patchs remplacent des lectures/branches internes par
des trampolines ajoutant des chemins NULL, avec des effets sur R11 absents
des instructions originales. Le champ +88 appartient au login original.
L'expérience ne départage pas l'effet propre de chacun des deux patchs :
la causalité démontrée porte sur cette famille de substitutions, sans
attribuer tout le blocage à un registre ou à une branche particuliers.
Preuve runtime : contrôle A/B identique à 889 détours, puis tests complets
sans filtre ; second login, vidéo, chargement menu/menu-temple et rendu repris.
Modification : suppression des deux trampolines et de leurs installations
(94 lignes), sans nouvelle garde, état forcé ni appel direct ajouté.
Fichiers modifiés : runtime_u2_script.c, u2_engine_tests.cpp, ce rapport,
ADR.md, DETOUR_VALIDATION.md et capture_hybrid_window.ps1.
Tests ajoutés : vérification des 14 et 18 octets originaux dans l'image
hybride réelle aux deux adresses, avant la création de la sandbox.
Build : deux exécutables réussis ; copie déployée dans out/ et dist/bin
lorsque le répertoire de distribution existe.
Test 6 secondes : SelfTest 886, full-body 831, moteur 218/218,
STOPPED_AFTER_TIMEOUT ; aucun crash, Guest fault, trap ou assertion nouvelle.
Tests prolongés : 25, 45 puis 60 secondes, mêmes tests, aucun défaut relevé ;
journaux diag_restored_final_25s.log, diag_restored_final_45s.log et
diag_restored_final_60s.log. Les fichiers effect1 facultatifs absents et les
avertissements SanitizeCopyLayers existaient dans la trace de référence ;
ils ne sont pas utilisés comme critère de panne.
Test natif : 80 méthodes moteur, 212/212 assertions, fermeture à 10 s, code 0.
Résultat visuel : vérifié par capture PrintWindow du bb-probe du workspace,
examinée par l'agent pendant le test de 60 s. Écran titre original Uncharted 2,
temple 3D et « Appuyer sur X pour commencer » visibles. La capture ne lit
aucune autre fenêtre et ne change pas le premier plan (Apex reste ouvert).
Preuve conservée : [capture du titre](../user/diag_hybrid_menu_restored.png).
Le script peut aussi capturer le rectangle du jeu s'il est au premier plan ;
PrintWindow n'est pas fiable pour tous les renderers, mais l'image obtenue
ici contient effectivement la scène 3D et le texte lisible.
Régression observée : aucune dans les scénarios testés. La navigation après
« commencer » et le gameplay ne font pas partie de cette validation.
Conclusion : référence hybride restaurée jusqu'à l'écran titre original.
Ce résultat ne signifie pas que le standalone affiche le menu original.
Commit : retrait atomique des patchs StateTask compensatoires.
Nettoyage : instrumentation C++ et commutateur temporaire retirés ; worktree
isolé remis sur 3d5dded et recompilé. Modifications utilisateur préservées.
Prochaine hypothèse : reprendre le port natif en comparant chaque nouveau
contrat au login et au streaming originaux, sans l'installer dans bb-probe.
