# Vecteurs d'orientation stockés de CAMERA_TABLE_1

Sous-système : game/loading, données caméra natives.
Symptôme : la définition native expose le nom et la position, mais aucun
champ d'orientation utilisable par le futur consommateur caméra.
Dernier point fonctionnel : décodage possédé de la caméra de menu-temple.
Premier point incorrect : les six composantes d'orientation restent opaques.
Hypothèse : les deux vecteurs lus par CameraManager peuvent être copiés,
sans anticiper sa conversion en quaternion ni une direction de vue.
Offset(s) étudié(s) : 4353AA..435B38, fonction englobante 434000,
message F2ACC8EC ; métadonnées page0+5FA0 et enregistrement page0+5FD0.

Preuve statique :

- 4353C1 lit record+40 ; le cas nul rejoint 4355E6. Le cas non nul appelle
  un résolveur à 9C0230, puis utilise une transformation externe.
- 4355E6/4355F3/4355FA lisent les floats +20/+24/+28 (vecteur A).
- 43560F/435616/435629 lisent les floats +14/+18/+1C (vecteur B).
- 43563E..4356C6 normalise les deux vecteurs ; 4356CA..435756 construit
  et normalise leur produit vectoriel A × B. La suite convertit la base
  en quaternion ; cette conversion n'est pas réimplémentée dans ce jalon.
- Le pas de 48 hexadécimal à 435B28 reste inchangé. Les accès directs
  aux enregistrements examinés dans cette branche ne lisent pas les floats
  +2C..+38 : leurs valeurs seules ne prouvent pas une sémantique de projection.

Preuve runtime : baseline native avant changement, 229/229 assertions et
sortie 0. Après changement, le lecteur du PAK réel retourne :

```
A = (-0.195780724, 0.903568625, -0.381095320)
B = ( 0.412893116, 0.428443372,  0.803713620)
external-transform = false
```

Les assertions comparent ces six composantes au payload réel après les
relocations existantes. Ceci valide la lecture native, pas une comparaison
dynamique avec le quaternion final du moteur original.

Conclusion : orientation_axis_a et orientation_axis_b conservent exactement
les vecteurs stockés, sans les normaliser ni les renommer « avant/haut ».
uses_external_transform conserve uniquement la présence du pointeur +40,
jamais son adresse ; aucune référence externe n'est suivie. Ces vecteurs
ne représentent donc pas nécessairement l'orientation effective lorsque
ce booléen vaut true. Tous les floats copiés doivent être finis : c'est une
limite explicite du lecteur natif, pas un comportement attribué à l'ELF.
Les vecteurs nuls ou non unitaires sont préservés sans inventer de correction.

Modification : extension de la copie possédée et du diagnostic caméra.
Fichiers modifiés : level_resources.h/.cpp, package_manager.cpp,
u2_engine_tests.cpp, UNCHARTED_NATIVE_PLAN.md et ce rapport.
Tests ajoutés : quatre assertions supplémentaires, couvrant le PAK réel,
les composantes signées non unitaires, la présence externe sans déréférencement
et NaN/+Inf/-Inf dans chacun des six champs (18 cas, sortie préservée).
Les tests existants couvrent aussi les axes nuls du second enregistrement,
le pas de table et l'indépendance après écrasement du buffer source.
Build : les deux exécutables compilés et déployés avec build_msys2.ps1.
Test 6 secondes : SelfTest 886, full-body 831, moteur 239/239,
STOPPED_AFTER_TIMEOUT ; aucun Guest fault ni nouvelle assertion invitée.
Test natif : 233/233 assertions ; délai de six secondes atteint, sortie 0.
Test prolongé : non nécessaire pour cette lecture effectuée au démarrage.
Régression observée : aucune sur ces tests ; les avertissements GPU et
ouvertures d'assets optionnels absents préexistants restent présents.
Journaux : user/diag_camera_orientation_baseline.log,
user/diag_camera_orientation_build.log, user/diag_camera_orientation_native.log,
user/diag_camera_orientation_hybrid_console.log et
user/diag_camera_orientation_hybrid_6s.log.
Commit : feat(native): preserve stored camera orientation vectors.
Prochaine hypothèse : établir et comparer la conversion des vecteurs en
quaternion, puis identifier le consommateur des paramètres de projection.
Le login moteur, la transformation du niveau et le rendu original restent
à porter ; les packages restent Resolved, avec neuf logins en attente.
Aucun nouveau détour hybride ou appel au code PS4 n'est introduit.
