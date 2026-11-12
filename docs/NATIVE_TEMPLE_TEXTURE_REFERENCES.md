# Références de textures des matériaux du temple

Sous-système : ndlib/render/ngen, snapshots possédés du viewer.
Symptôme : les références de matériaux sont résolues, mais leurs paramètres
de textures et leurs indices locaux sont absents des snapshots.
Dernier point fonctionnel : treize indices de matériaux comparés à l'original.
Premier point incorrect : les tableaux de textures des définitions sont ignorés.
Hypothèse : PrepareBinary associe chaque référence de matériau à une entrée
locale de géométrie en hachant son nom de ressource préparée (cooked name),
distinct du chemin source de l'image. Le natif doit retrouver les mêmes indices.

## Preuves statiques

Offset(s) étudié(s) : C2A960, C2BA20..C2BC6A, B26FC0, C2D4D6..C2D4E7.
PrepareBinary utilise le compte de textures à payload géométrie+38 hex et le
pointeur de table à +40 hex, soit metadata+58/+60 dans GEOMETRY_1.
C2BA2E multiplie l'index par 5 puis utilise l'échelle 8 : entrées de 40 octets.
Le premier pointeur est le cooked name, haché à C2BA41 par le callback CRC
déjà attribué à B26FC0. La table triée conserve le premier index par hash.

Pour chaque définition de matériau de 72 octets, C2BB0A lit le compte de
références à +38 hex, C2BB28 leur pointeur à +28 hex. Chaque référence a un pas
de 40 octets et trois pointeurs de chaînes à +0, +8 et +10 hex : nom du paramètre,
chemin source et cooked name. C2BB6D hache ce troisième champ, recherche le
hash dans la table locale, vérifie l'index >= 0, puis C2BC6A écrit à +18 hex.
Initialize relit aussi le tableau et son compte à C2D4D6/C2D4E2 ; son utilisation
complète par les shaders n'est pas portée ici.

Le PAK du temple fournit 60 entrées locales et 100 références dans les treize
matériaux. Les 100 références désignent 59 indices distincts, de 1 à 59.
Exemple pour la première référence :

- Paramètre : `g_tColorMap`.
- Chemin source : `art/levels/temple/textures/walls/tmp-wall-red2-color.tga`.
- Cooked name : `d:/dev/CP9/Data/U2/CookData/ND/build/texture2/art/levels/temple/textures/walls/tmp-wall-red2-color.tga/4246CBC0DBEC44C4`.
- Index local résolu : 1.

Les autres noms comprennent g_tBlendMap, g_tNormalMap, g_tSpecMaskMap et
g_tLightMap. Leurs noms sont conservés, sans supposer qu'ils sont tous des
couleurs à appliquer directement sur les UV. Les chemins d'auteur d:/dev ne
sont pas utilisés comme chemins de fichiers sur le PC utilisateur.

## Preuve runtime

Test hybride original de 50 secondes avec le hook de comptage vérifié
`BB_GUEST_HOOKS=0xC2A960:8` et
`BB_HOOK_DUMP=0xC2A960:96:user/diag_texture_prepare_objects.bin`.
Lecture Windows ReadProcessMemory du seul processus bb-probe du workspace
(PID 9708), sans écriture. Parmi les objets observés, le nom du premier mesh
identifie le temple. Ses références après PrepareBinary ont toutes un index
valide et leur cooked name est identique à celui de l'entrée locale choisie.

[Capture originale des cent références](preuves_avancement/2026-11-12/material-texture-bindings-runtime.json).
Cette observation établit les liaisons de noms et d'indices, pas le chargement
des pixels, la disponibilité GPU, le format d'image ou le sampler.

## Implémentation et validation

Conclusion : la liaison mesh → matériau → références de textures est désormais
disponible dans les snapshots natifs, avec les 100 indices comparés à l'original.
Les pixels ne sont pas encore décodés : l'image éclairée reste inchangée.
Modification : U2NativeTextureReferencePreview conserve paramètre, chemin source,
cooked name, hash et index local. Le lecteur construit la table CRC triée,
préserve le premier index des doublons, puis résout chaque matériau utilisé.
Les pointeurs, tailles et terminaisons sont bornés ; les budgets cumulatifs de
références et de chaînes dépendent de la taille de page. Une référence introuvable
fait échouer la lecture avant publication, conformément à l'assertion originale
sur un index négatif. Aucun index guest à +18 n'est écrit, aucun login moteur,
aucune instance de texture ni mémoire exécutable n'est ajouté.

Fichiers modifiés : geometry_preview.h/.cpp, package_manager.cpp,
u2_engine_tests.cpp, plan natif, ce rapport et la capture JSON.
Tests ajoutés : vingt et une assertions. Les 100 indices et le premier triplet
de chaînes sont comparés à la capture ; données synthétiques pour ordre des
tables, doublons, chemins source distincts de la clé cooked, absence/casse,
comptes excessifs, tables de 40 octets tronquées et bornes exactes, chaînes non
terminées, pointeurs étrangers, noms NULL, publication atomique, reprise après
rejet et conservation des chaînes après destruction de la page source.
Le premier test synthétique de doublon supprimait aussi cook/a encore demandé
par un autre paramètre ; le lecteur rejetait correctement l'ensemble incomplet.
Le jeu de données a été corrigé pour préserver cook/a et tester le doublon seul.

Build : deux exécutables compilés/déployés, diag_texture_refs_final_build.log.
Test 6 secondes : SelfTest 886, full-body 831, moteur 408/408,
STOPPED_AFTER_TIMEOUT ; journal complet sans faute ni assertion nouvelle.
Test natif : 402/402, texture-refs=100, délai de six secondes, sortie 0.
Test prolongé : référence originale 50 secondes avant modification, 387/387,
timeout sans faute. Aucun hook dans les validations finales ; pas de test long
supplémentaire pour cette lecture synchrone.
Régression observée : aucune dans les scénarios vérifiés, 889 détours inchangés.
Journaux : user/diag_material_texture_baseline.log, diag_texture_refs_original.log,
diag_texture_refs_final_native.log, diag_texture_refs_final_hybrid.log et
diag_texture_refs_final_hybrid_console.log. Les journaux sans suffixe final
conservent aussi l'échec initial du jeu de données synthétique.
Commit : feat(native): resolve temple material texture references.
Prochaine hypothèse : retrouver les données d'images correspondant aux noms
préparés dans TEXTURE_TABLE. Dans le PAK brut, sa metadata est à page0+3CC00,
son payload annonce 60 entrées. Les pointeurs, formats, éventuel tiling et
pixels restent à établir avant un décodage ou une présentation texturée.
