# Port PC Uncharted

Faire progresser le port par petites modifications reproductibles fondées sur des preuves.

## Invariants
- bb-probe et son ELF original restent la référence comportementale.
- Le natif n'exécute aucun code PS4 ; données/rodata possibles en mémoire non exécutable.
- Aucun détour fondé sur le seul nom : établir comportement, ABI, structures et contexte runtime. Entrées ELF : ABI SysV obligatoire.
- Aucun succès fictif, pointeur factice, état forcé ou offset deviné. Accès guest : région, taille, alignement, durée de vie vérifiés ; écritures : offset et sémantique prouvés.
- Préserver changements utilisateur et sous-modules. Aucun Git destructeur ; ne pas committer .agents/ ni fichiers étrangers.
- Une hypothèse comportementale cohérente par itération. Distinguer observation, hypothèse et convention du viewer.

## Lectures à la demande
Commencer par git status --short et docs/CURRENT_WORK.md. Réutiliser les preuves encore valides dans le contexte ; ne pas relire inutilement les documents inchangés. Chercher avec rg, lire les sections utiles ; éviter fichiers entiers et sorties tronquées.
- ADR.md : décision d'architecture concernée.
- docs/DETOUR_VALIDATION.md : détours, ABI, sandbox.
- docs/UNCHARTED_NATIVE_PLAN.md : progression/architecture native.
- Rapport du sous-système : preuves et limites existantes.
- tools/u2_map/u2_engine_map.json : symbole/adresse ciblé.
- tools/u2_map/U2_SOURCE_TREE.md : propriétaire original incertain.
- HISTORIQUE_SCRATCH.md et ROADMAP.md : question historique pertinente seulement ; vérifier la cible.
- docs/WORK_LOG.md : historique recherché uniquement, jamais lecture intégrale systématique.

## Propriété du code
Dc* : src/runtime_u2_{player,npc,scene,event,audio}.c selon propriétaire ; moteur : src/u2_engine/ ; système/HLE : src/runtime_*.c ; GPU : gpu/ ; standalone/viewer : src/u2_standalone/.

## Itération
Définir le comportement attendu et son critère de validation ; établir la baseline (réutiliser un test récent du même état si pertinent) ; formuler une hypothèse falsifiable ; examiner code, désassemblage et appelants utiles ; capturer les preuves runtime manquantes avec instrumentation bornée ; modifier le minimum et vérifier le comportement.
Avant un nouveau détour, prouver son appel réel, ses arguments, retours et mutations utiles. Préserver les statuts VM et les durées de vie guest.
Si une preuve manque, exécuter la prochaine expérience non destructive autorisée. Demander seulement l'information indispensable inaccessible ; ne pas inventer de résultat.

## Validation
Code : git diff --check, puis PowerShell -ExecutionPolicy Bypass -File scratch/build_msys2.ps1 et scratch/run_test.ps1 6 via PowerShell. Ajouter scratch/run_native_test.ps1 pour natif/viewer.
Vérifier SelfTest, EngineTest, timeout/sortie attendue, aucun crash, Guest fault ou nouvelle assertion, et le comportement visé. Test prolongé seulement si nécessaire. Examiner les erreurs dans leur contexte et les événements attendus.
Nouvelles fonctions moteur : assertions fonctionnelles dans u2_engine_tests.cpp ; registre seulement si détour nécessaire et prouvé. Dc* : tester arguments absents/NULL/valides.
Documentation seule : contenu, liens, diff ; pas de build du jeu.
Outils : node scratch/u2_disasm.js <offset_hex> <taille> ; node scratch/status_report.js seulement si utile. Journal : user/last_run.log, à conserver avant écrasement.
Baseline historique e80b482 ; une référence spécifique doit être justifiée. dev_archive_lots est non normative.

## Suivi et clôture
CURRENT_WORK.md reste court : cible, état validé, preuves, incertitude, prochaine expérience et travail non enregistré.
Avant de remplacer, condenser ou vider docs/CURRENT_WORK.md, archiver son contenu intégral dans docs/WORK_LOG.md, cumulatif et non réécrit.
Utiliser C:/msys64/clang64/bin/python.exe scratch/rotate_current_work.py --next <nouveau_fichier_utf8>. Le script archive et vérifie avant remplacement. En cas d'échec, conserver le suivi ; ne jamais le tronquer manuellement. Après un crash, un doublon daté est acceptable. Ne pas supprimer d'entrée historique.
Le journal conserve date UTC, SHA-256 et contenu ; rechercher uniquement les entrées pertinentes.
Consigner hypothèse, preuves, résultat, tests, limites et prochaine expérience dans le rapport concerné. Retirer les diagnostics temporaires inutiles. Commit atomique ciblé sur la branche active seulement si validé ; sinon conserver les preuves et préciser le manque. Une capture diagnostique peut être terminée sans résoudre le problème global.
Réponse finale courte : résultat, tests, limite importante, commit.
