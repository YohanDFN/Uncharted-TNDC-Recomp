# Quaternion natif comparé au calcul caméra original

Sous-système : game/camera, préparation des données CAMERA_TABLE_1.
Symptôme : la définition native possède deux vecteurs, mais aucune orientation
calculée utilisable par le futur consommateur caméra.
Dernier point fonctionnel : nom, position, vecteurs A/B et présence du transform
externe copiés depuis le PAK relogé.
Premier point incorrect : la conversion vers l'orientation quaternion manque.
Hypothèse : le chemin à transform externe nul construit une base de colonnes
C = normalisé(A × B), normalisé(A), normalisé(B), puis un quaternion xyzw.
Offset(s) étudié(s) : 4355E6..435A11, dans CameraManager à 434000.

## Preuve statique

- 43563E..4356C6 normalise A et B par rsqrt et une correction de Newton.
- 4356CA..435756 calcule le produit vectoriel des vecteurs stockés, puis
  le normalise séparément. Il ne recalcule pas A à partir de B et C.
- À 43575A, les registres contiennent C, A, B. Le masque E2D650 conserve
  xyz, sa quatrième composante est nulle. E60274 = 1, E60278 = 0,5,
  E6027C = 0 ; E2D660 est le masque de signe flottant.
- 43578C teste Bz contre zéro. Si Bz est négatif, Cx > Ay choisit X,
  sinon Y. Si Bz est positif ou nul, Cx < -Ay choisit Z, sinon W.
- Les quatre radicandes sont respectivement 1+Cx-Ay-Bz, 1-Cx+Ay-Bz,
  1-Cx-Ay+Bz et 1+Cx+Ay+Bz. Les signes des autres composantes suivent
  la matrice de colonnes C/A/B. La composante dominante est positive.
- 4359DF..435A0B applique 0,5/sqrt(radicande) et assemble xyzw dans xmm0.
  Le quaternion n'est pas renormalisé après cette conversion.
- 435A11 commence la lecture de la translation de contexte. Le quaternion
  est donc capturable avant la transformation de position et l'envoi du message.

## Preuve runtime et reproduction

Le diagnostic BB_GUEST_HOOKS expose maintenant les 128 bits du dernier xmm0.
Un vmovups vers la mémoire du diagnostic conserve le registre ; aucun appel
C ni traitement flottant ne s'insère dans le calcul invité. Un contrôle de
compilation établit l'offset du champ dans GuestHook. Ce diagnostic ne
s'installe que lorsque BB_GUEST_HOOKS est fourni.

```
C:/msys64/clang64/bin/python.exe scratch/diag_hybrid_compare.py --probe out/gpu/bb-probe.exe --label camera_quaternion_original --seconds 25 --hooks 0x435A11:7,0x4355E6:7
```

À 435A11, les sept octets déplacés sont un mov depuis rbp vers rax, sans
référence RIP. À 4355E6, les sept octets déplacés sont le chargement du premier
float du vecteur A. Ces points ne dépendent pas des flags modifiés par le
compteur préexistant du hook. Aucun calcul caméra original n'est remplacé.

La capture initiale observe exactement un passage par chaque point, avec le
même rcx (enregistrement) et rsi (table). Le chemin inline a bien été pris.
À la sortie, les mots de xmm0 sont :

```
3e6ad52ebe5a8cda 3f72c1043d536b35
xyzw = (-0.213427931070, 0.229328840971, 0.051615912467, 0.948257684708)
```

Le rapport périodique est un instantané, pas une trace synchronisée de chaque
appel. Cette preuve concerne un appel isolé dont la valeur est stable après
son achèvement ; elle ne suppose pas la cohérence d'appels concurrents.
La capture est conservée dans user/diag_compare_camera_quaternion_original.log.
Le test final reproduit cette observation dans diag_compare_camera_quaternion_final.log.

Conclusion : le calcul natif reproduit la convention de colonnes et les signes
du jeu pour la caméra du temple. La comparaison utilise une tolérance de
2e-6 par composante, car le C++ utilise sqrt/division scalaire ; ce n'est
pas une garantie d'identité binaire sur toute entrée ou toute architecture.
L'application du quaternion à la base locale Y/Z retrouve aussi A/B réels.

Modification : CalculateCameraOrientation dans game/camera/camera_orientation.cpp,
intégré au décodage possédé dans game/loading. La définition conserve xyzw
et un booléen orientation_calculated ; un refus ne bloque pas la lecture
des champs bruts déjà établis. L'accesseur de package retourne ces valeurs
par copie comme les autres champs.
Fichiers modifiés : camera_orientation.h/.cpp, level_resources.h/.cpp,
package_manager.cpp, u2_engine_tests.cpp, probe.c, plan natif et ce rapport.
Tests ajoutés : 14 assertions, incluant la capture ELF réelle, l'identité,
les trois demi-tours, les trois quarts de tour positifs, des axes redimensionnés,
la reconstruction de base, le transform externe, la sortie NULL, les axes
dégénérés et les composantes non finies. Les refus préservent la sortie.
Les tests du lecteur vérifient aussi orientation_calculated=false pour le
transform externe non résolu et pour les vecteurs nuls d'une seconde caméra.
Build : deux exécutables compilés et déployés avec build_msys2.ps1.
Test 6 secondes : SelfTest 886, full-body 831, moteur 253/253,
STOPPED_AFTER_TIMEOUT, sans Guest fault ni nouvelle assertion invitée.
Test natif : moteur 247/247 ; quaternion calculé depuis le PAK réel ;
fermeture au délai de six secondes, sortie 0.
Test prolongé : 25 secondes avec les deux points de capture ; 253/253,
STOPPED_AFTER_TIMEOUT, zéro faute/assertion/erreur d'allocation signalée,
et un passage par point. Les bits du quaternion original sont identiques
à la première capture. Le passage caméra nécessite le chargement du menu.
Régression observée : aucune dans les scénarios vérifiés ; registre hybride
inchangé à 889 détours et calcul caméra toujours exécuté par l'ELF original.
Journaux : user/diag_camera_quaternion_baseline.log, diag_camera_quaternion_build.log,
diag_camera_quaternion_native.log, diag_camera_quaternion_hybrid_6s.log et
diag_camera_quaternion_hybrid_console.log, plus les deux captures ci-dessus.
Commit : feat(native): derive camera quaternion against original runtime.
Prochaine hypothèse : établir la translation du niveau et les consommateurs
des champs de projection pour préparer une caméra de rendu native.

## Limites explicites

Le chemin de transform externe n'est pas implémenté. Les vecteurs nuls,
parallèles, non finis et les calculs de longueur/produit qui débordent ou
sous-débordent sont refusés, sans produire une orientation factice. Ce sont
des limites du calcul hôte, pas des gardes attribuées au moteur original.
Les vecteurs non orthogonaux ne sont pas orthogonalisés artificiellement.
Ce jalon ne construit aucun objet Camera/Level, n'achève aucun login moteur,
ne déclare aucun package Ready et n'affiche pas la scène originale dans
le standalone. Il n'exécute aucun octet de .text PS4 dans la cible native.
