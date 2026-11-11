# Projection perspective centrée comparée à RenderCamera

Sous-système : game/rendercamera.cpp, calcul de projection hôte.
Symptôme : la caméra native possède position et quaternion, mais la convention
de projection et la provenance de ses paramètres ne sont pas établies.
Dernier point fonctionnel : position translatée comparée à l'ELF original.
Premier point incorrect : assimiler les floats voisins du PAK à la projection
active serait une hypothèse non validée.
Hypothèse : la projection effectivement utilisée passe par RenderCamera,
avec des paramètres distincts de la définition CAMERA_TABLE_1 de départ.
Offset(s) étudié(s) : 43535C..435B28, 9DE130..9DE1F5,
71C085..71C166, 9DC750 et 9DC8EE..9DCCD0.

## Preuve statique

Le chemin caméra de départ établi ne lit pas record+2C/+30/+34/+38.
Leur valeur ne suffit donc toujours pas à les associer à la projection active.

SetupCamera à 9DE130 vérifie `fFov > 0` pour xmm2. Il sauvegarde xmm0/xmm1
à objet+258/+25C et xmm3 à +250 ; 9DE1C7 multiplie xmm2 par 0,5 et appelle
DE70B0, puis écrit le réciproque à +254. Le trampoline porte l'index D1,
soit 209 ; analysis.json identifie l'import 209 ZE6RNL+eLbk, résolu comme
tanf par import_names.inc. Le FOV passé à ce calcul est donc en radians.
Le même calcul est visible inline dans UpdatePlayerAiming à 71C085..71C166,
avec atan puis tan. Ces deux chemins ne constituent pas une preuve de leur
exécution dans le menu : aucun appel du prologue 9DE130 n'a été observé.

Le chemin actif à 9DC750 traite les données de RenderCamera. Dans sa branche
perspective finie, 9DCB45 charge +258 (n), +250 (rapport), +254 (zoom),
avec +25C (f) déjà chargé. L'assertion est `f > n && zoom > 0.0f`.
9DCBDA..9DCC58 construit les quatre vecteurs sauvegardés à +90/+A0/+B0/+C0
par 9DCCD0..9DCCE8. Avec le vecteur +270 égal à (1,1,0,0), ils correspondent à :

```
colonne 0 = (-zoom, 0, 0, 0)
colonne 1 = (0, zoom / rapport, 0, 0)
colonne 2 = (-0, 0, f / (f-n), 1)
colonne 3 = (0, 0, -(f*n) / (f-n), 0)
```

Le rapport est hauteur/largeur pour conserver un FOV horizontal. Les points
à z=n et z=f donnent les profondeurs normalisées 0 et 1. Le signe négatif
de X appartient au calcul original ; aucune adaptation GPU n'est ajoutée ici.

## Preuve runtime

Le runner diagnostique accepte désormais --dump pour le BB_HOOK_DUMP existant
et efface aussi BB_HOOK_DUMP_BEFORE hérité. Reproduction aux prologues vérifiés :

```
C:/msys64/clang64/bin/python.exe scratch/diag_hybrid_compare.py --probe out/gpu/bb-probe.exe --label projection_object --seconds 25 --hooks 0x9DC750:5,0x9DE130:6 --dump 0x9DC750:656:user/diag_projection_objects.bin
```

9DC750:5 déplace push rbp, mov rbp,rsp et push rbx. 9DE130:6 déplace
push rbp, mov rbp,rsp et push r15. Aucun adressage RIP déplacé.
Un premier essai exploratoire comportait aussi 71C080, qui n'est pas une
frontière d'instruction valide. Cet essai a été écarté ; aucun passage à ce
point n'a été observé. Il n'est pas utilisé comme preuve de projection.

Le dump écrit [temps double][656 octets] toutes les 50 ms, soit un pas de
664 octets par échantillon. Il peut intercepter plusieurs caméras, y compris
des projections non perspectives et des objets réutilisés : ce n'est pas une
capture atomique ni l'identification certaine d'un seul objet écran.

L'échantillon 307 de la capture retenue contient une perspective cohérente,
à une position proche de la caméra de départ du temple (déjà animée) :
(-78.266701,26.894699,-29.700500). Les octets +280 et +282 sont nuls,
le vecteur +270 vaut (1,1,0,0), et les paramètres sont :

| Champ | Valeur |
| --- | --- |
| +250 hauteur/largeur | 0,5625 |
| +254 zoom horizontal | 1,3032253980636597 |
| +258 plan proche | 0,23999999463558197 |
| +25C plan lointain | 5000 |

Le FOV déduit du zoom vaut 1,3089969203 radians, environ 75 degrés.
La même projection apparaît dans de nombreux autres échantillons cohérents.
La matrice capturée contient les mots suivants, comparés tous les seize par le test :

```
BFA6D017 00000000 00000000 00000000
00000000 40144731 00000000 00000000
80000000 00000000 3F800193 3F800000
00000000 00000000 BE75C595 00000000
```

Conclusion : le calcul de perspective centrée peut être reproduit en C++
avec des entrées explicites. Les valeurs actives observées diffèrent des
floats voisins du PAK (notamment 0,01 et 10000) ; aucune liaison arbitraire
avec CAMERA_TABLE_1 ni constante de menu n'est installée dans le runtime natif.
Modification : U2NativePerspectiveParams, BuildCenteredPerspective et conversion
depuis un FOV horizontal en radians, dans le module original game/rendercamera.
Le consommateur doit fournir les paramètres ; la fenêtre native ne les utilise
pas encore pour rendre le temple. Aucun détour moteur n'est ajouté.
Fichiers modifiés : game/rendercamera.h/.cpp, u2_engine_tests.cpp,
scratch/diag_hybrid_compare.py, plan natif et ce rapport.
Tests ajoutés : dix assertions comparant les 16 floats de l'ELF, les points
des plans proche/lointain, les bords du frustum, le signe X, le format portrait,
le FOV en radians, le zoom observé et les refus atomiques (NULL, valeurs non
finies, plans invalides et débordements). Les paramètres de capture ne servent
que de référence dans les tests, pas de valeurs moteur imposées.
Build : les deux exécutables compilés et déployés avec build_msys2.ps1.
Test 6 secondes : SelfTest 886, full-body 831, moteur 278/278 et
STOPPED_AFTER_TIMEOUT, sans Guest fault ni nouvelle assertion invitée.
Test natif : 272/272 assertions ; fermeture au délai de six secondes, sortie 0.
Test prolongé : 25 secondes avec le seul hook 9DC750:5 et un nouveau dump.
278/278, STOPPED_AFTER_TIMEOUT, zéro faute/assertion/erreur d'allocation
signalée. Sur 361 échantillons, 215 contiennent les 16 mêmes mots de matrice,
dont trois avec une position à moins d'une unité par axe de la caméra de départ.
Régression observée : aucune dans les scénarios vérifiés ; les 889 détours
hybrides restent inchangés. Vérification Python du runner par py_compile.
Journaux : user/diag_projection_baseline.log, diag_projection_build.log,
diag_projection_native.log, diag_projection_hybrid_6s.log,
diag_compare_projection_object.log, diag_projection_objects.bin,
diag_compare_projection_final.log et diag_projection_final.bin.
Commit : feat(native): reproduce centered RenderCamera perspective projection.
Prochaine hypothèse : identifier le producteur actif des paramètres +250..+25C
dans le menu, pour les relier à la caméra native sans les figer aux valeurs observées.

Limites : perspective finie centrée uniquement, échelles (1,1) et décalages nuls.
Projections orthographiques, décentrées, personnalisées ou à plan infini exclues.
Le calcul hôte exige des entrées et coefficients finis et une plage de FOV
strictement entre 0 et pi ; ces refus sont des limites hôte, pas de nouvelles
gardes attribuées à l'ELF. Aucune page .text PS4 n'est exécutée par le natif,
aucun login moteur n'est achevé, aucun package n'est déclaré Ready.
