# Frontière LevelMgr / Level / Package

Sous-système : game/level.cpp, game/level-mgr.cpp et ndlib/io.
Symptôme : le standalone possède les pages et une propriété LEVEL_INFO isolée,
mais aucun propriétaire Level complet pour le rendu.
Dernier point fonctionnel : les pages menu-temple sont relogées et inventoriées.
Premier point incorrect : aucun cycle de création/chargement/destruction de Level.
Hypothèse : un niveau est alloué par LevelMgr, initialise ses champs, puis
demande des packages auxquels il est associé comme propriétaire.

## Preuve statique

La cartographie u2_engine_map.json associe 0x56EA40 à Init dans level.cpp,
0x56A620 à AllocLevelSlot dans level-mgr.cpp. La recherche ciblée des appels
relatifs à Init trouve un appel à 0x56A782.

- 0x56A774..0x56A782 : slot = pool + index * 0xA60, puis Level::Init.
- 0x56EA40..0x56EC35 : arguments SysV rdi=Level, rsi=nom, edx=index.
  Nom limité à 63 caractères à +0x34, StringId à +0x74 ; compteur global
  0x14C79D0 incrémenté et stocké à +0x914 ; index à +0x910.
- Init remet explicitement à NULL les références géométrie (+0x950/+0x958),
  caméra (+0x988), lumière (+0x990), ainsi que plusieurs autres sous-systèmes.
  Le champ LEVEL_INFO à +0xA50 est initialisé à zéro. Cela ne prouve pas que
  tous les autres membres puissent être remplacés par un memset global.
- 0x56EC40..0x56EC65 rattache un autre objet à Level+0x908, puis choisit
  une phase de chargement. Les dépendances de cet objet restent à établir.
- 0x56ECB1..0x56ECCD et 0x56ED60..0x56ED88 demandent les packages via
  0xB6CD10 avec rdx=Level et stockent les retours dans Level+0x80.
- 0x56C2AB..0x56C2E4 appelle AllocLevelSlot, manipule les flags et un
  compteur de références Level+0x938. Cette routine opère donc sur des Level.
- 0x48DEB2..0x48DEC3 : le login CAMERA_TABLE_1 lit Package+0x28 et y
  attache metadata+0x20 au champ Level+0x988.

Correction documentaire : 0x56C0E0 ne peut plus être présenté comme une
implémentation établie de PackageMgr::RequestPackage. Son nom exact reste
à confirmer, mais l'environnement voisin et le propriétaire 0x1130390
relèvent du chemin LevelMgr. Le registre hôte package_manager.cpp est une
facilité native indépendante ; sa résolution directe des fichiers PAK ne
reproduit pas ce cycle de niveaux.

## Preuve runtime et limites

Commande reproductible (dans un processus PowerShell dédié) :

```powershell
$env:BB_GUEST_HOOKS='0x56ea40:10,0x48d920:13,0x56a620:13'
powershell -ExecutionPolicy Bypass -File scratch/run_test.ps1 20
```

Premier diagnostic : 26 appels Init et AllocLevelSlot, 84 appels du callback
0x48D920, timeout atteint ; SelfTest et EngineTest passent (187/187).
Derniers arguments observés :

- Init : rdi=0x10484b3e70, rsi=0xc7c25e8, edx=0x19.
- AllocLevelSlot : rdi=0xc430390, rsi=0xc7c25e8.
- Login : rdi=0x1048051048, rsi=0x24bad3070, rdx=0x24bad3000.

Ces captures établissent l'exécution de ces routines dans ce scénario,
pas le contenu de Package+0x28 ni leur invocation pour menu-temple.

Une seconde expérience avec BB_HOOK_DUMP pour capturer Package+0x28 échoue
avant le premier callback : allocation guest de 6080 Mio refusée (Windows
1450), assertions puis Fault à 0xB309DB. Le fichier user/level_owner_probe.bin
est vide. Cette expérience n'apporte donc aucune preuve de pointeur propriétaire.
Le timeout du harnais ne doit pas être interprété comme un succès fonctionnel.

Conclusion : propriétaire statique établi, capture dynamique de l'association
et destruction encore manquantes. Aucune construction guest spéculative ajoutée.
Modification : correction du commentaire de provenance et documentation.
Fichiers modifiés : package_manager.cpp (commentaire), UNCHARTED_NATIVE_PLAN.md,
ce rapport. Aucun comportement modifié, aucun nouveau test fonctionnel requis.
Build : inchangé depuis le commit précédent ; baseline native 187/187 sortie 0.
Test prolongé : première trace exploitable, seconde invalidée par erreur mémoire.
Régression : aucune attribution possible de cet échec à un changement de code,
car aucun changement comportemental n'a été effectué pendant l'investigation.
Commit : documentation atomique de cette investigation.
## Capture réussie après nettoyage des processus de test

Les processus résiduels des deux diagnostics, PID 5304 et 25864, ont été
identifiés par chemin et heure de lancement, puis arrêtés. Une troisième
expérience identique écrit user/level_owner_probe_retry.bin.
La capture observe notamment Package+0x28 = 0x10484a4570 et 0x10484b3e70,
qui correspondent exactement aux Level* observés dans Init pendant ce run.
Les autres propriétaires capturés sont 0x10484a3b10 et 0x10484b1f50.
Les adresses ne sont pas des constantes à réutiliser.

L'association dynamique package -> Level est donc confirmée dans ce scénario,
sans preuve d'identité menu-temple. La trace retrouve 26 Init et 84 logins.
Prochaine expérience : suivre la libération des références à +0x938 et celle
des slots LevelMgr avant d'implémenter un propriétaire natif. Le harnais doit
aussi être surveillé : il a laissé des processus bb-probe actifs après timeout.

## Investigation de la libération différée

Sous-système : Level / LevelMgr / durée de vie des packages.
Symptôme : destruction et retour du slot non établis pour le propriétaire natif.
Dernier point fonctionnel : association Package+0x28 -> Level prouvée.
Premier point incorrect : absence de preuve du moment où les pages cessent
d'être référencées par les consommateurs de rendu/scripts.
Hypothèse : référence nulle programme un déchargement différé ; elle ne rend
pas directement le slot au pool.

Preuve statique, plages ciblées :

- 0x570490..0x5704B7 : si Level flags +0x18 bit 0x40 est présent, retour.
  Sinon le int16 à +0x938 est décrémenté puis borné à zéro ; la valeur nulle
  provoque un tail-call à 0x5704C0. Variante via recherche à 0x56E752..0x56E76D.
- 0x5704C0..0x570899 : interactions avec la géométrie +0x958 et les flags de
  frame ; phase à +0x30 mise à 8 ; frame courante et échéance conservées à
  +0x91C/+0x930. Appels complémentaires dont 0x573100 et 0x570D30.
  Le label automatique GetMutableFlags est une attribution issue d'un appel
  inliné, pas un nom établi pour le corps complet de cette routine.
- 0x5708A0..0x570A28 : nettoyage différé, notamment vérification d'un delta
  de 0x1A frames, interaction avec des objets de processus, puis packages.
  0x5709A9..0x5709CD sélectionne Level+0x88 ou +0x80 et appelle 0xB6CC30.
  0x5709F0..0x570A02 boucle sur 0x571110. Cette dernière dépend elle-même
  d'autres structures moteur ; la traiter comme un simple free serait faux.
- 0x56AC30..0x56AD5F : deux arguments, rdi=gestionnaire, rsi=slot Level.
  L'index est calculé depuis le pointeur de pool, puis borné à 179 inclus.
  Le bit dans le bitmap à manager+0x80 est effacé à 0x56AC80 ; l'entrée
  correspondant au StringId Level+0x74 est ensuite retirée de la table.
  C'est une étape de retour au pool, distincte des routines précédentes.

Les offsets +0x938 sont trouvés par recherche ciblée des opérandes mémoire
dans la plage LevelMgr/Level, puis confirmés par désassemblage des routines
et de leurs appelants. Aucun offset n'est attribué par seule proximité.

Preuve runtime :

```powershell
$env:BB_GUEST_HOOKS='0x56ea40:10,0x5704c0:13,0x5708a0:11'
powershell -ExecutionPolicy Bypass -File scratch/run_test.ps1 20
```

Le scénario observe 26 appels Init ; aucun appel des deux routines de
déchargement n'est rapporté. Le compteur imprime tout changement depuis zéro.
Cela prouve leur absence dans cette fenêtre de boot seulement, pas leur
inutilité. SelfTest/EngineTest passent (187/187), timeout, aucune faute ou
assertion relevée. Le processus résiduel PID 4924 est arrêté après inspection.

Conclusion : l'hypothèse du déchargement différé est soutenue statiquement,
mais aucune transition dynamique de déchargement n'a été observée. Le lien
entre fin de nettoyage, fin des références GPU et retour du slot doit encore
être prouvé. Aucun propriétaire guest, destructeur ou fonction native ajouté.
Modification : documentation uniquement.
Fichiers modifiés : ce rapport.
Tests ajoutés : aucun, pas de comportement modifié.
Build : non relancé, aucun code modifié.
Test 6 secondes : non relancé, diagnostic ciblé de 20 secondes à la place.
Test prolongé : ci-dessus, observation négative bornée et exploitable.
Régression observée : aucune dans le scénario diagnostique.
Commit : documentation atomique de l'investigation.
Prochaine expérience : obtenir un scénario original qui décharge effectivement
un niveau (transition de niveau ou fermeture moteur propre), instrumenter
0x5704C0, 0x5708A0, 0x56AC30 et le traitement des packages avant le retour du
slot. Ne pas forcer le compteur ou la phase pour fabriquer cette transition.
