# Résolution des références de matériaux du temple

Sous-système : ndlib/render/ngen, snapshots possédés de géométrie.
Symptôme : les UV sont lus, mais les meshes n'ont pas de référence de matériau résolue.
Dernier point fonctionnel : positions, indices, normales, UV et aperçu éclairé.
Premier point incorrect : MeshDesc+48 hex contient un nom encore ignoré ;
l'index brut à +50 hex vaut -1 dans le premier descripteur du PAK.
Hypothèse : PrepareBinary résout ce nom dans les définitions locales de
matériaux avant de créer le mesh ; une lecture native doit produire les mêmes indices.

## Preuves statiques

Offset(s) étudié(s) : BE3D74, C2A960, C2AF6F, C2B070..C2B1EA,
B26FC0..B26FFB, table DF0390, C29660..C296F6.
Le job original appelle PrepareBinary à C2A960 avant la création de géométrie.
Son argument rdi est un objet dont le premier pointeur désigne le payload géométrie.
Le payload a le compte de matériaux à +48 hex et leur table à +50 hex,
soit metadata+68/+70 pour la ressource GEOMETRY_1.

C2B081 multiplie l'index par 9 puis utilise une échelle 8 : chaque définition
fait 72 octets. C2B085 lit son pointeur de nom au début de l'entrée.
La routine fabrique une table triée de couples (hash uint32, index initial),
sans remplacer le premier index lorsque deux noms ont le même hash.
C2B15D lit le nom demandé à MeshDesc+48 hex ; après recherche, C2B1EA écrit
l'index à MeshDesc+50 hex, ou -1 si le hash n'est pas trouvé.
C29668 relit cet index signé pour GetBoundMaterial, puis C296CD emploie
le stride de 70 hex des instances de matériaux avant l'appel de création du mesh.
Ce stride d'instance ne doit pas être confondu avec les 72 octets des définitions.

Le callback image+10666C8 résout à B26FC0 pendant cette observation : hash CRC
à décalage vers la gauche, polynôme 04C11DB7, seed 0, sans complément final.
Les caractères sont traités comme octets non signés et la casse est conservée.
La table originale DF0390 commence par 0, 04C11DB7, 09823B6E, 0D4326D9.
Ce hash diffère du FNV utilisé par le registre hôte de demandes de packages.
Certaines anciennes fonctions runtime nomment le pointeur 10666C8 GetSelf :
cette attribution ne correspond pas à la routine observée ici. Aucun de ces
bindings étrangers à cette itération n'est modifié.

## Preuve runtime

Hook de comptage `BB_GUEST_HOOKS=0xC2A960:8`, prologue vérifié : push rbp,
mov rsp/rbp, push r15, push r14. Dump borné de l'objet avec
`BB_HOOK_DUMP=0xC2A960:96:user/diag_material_prepare_objects.bin`.
Pendant le test hybride de 50 secondes, ReadProcessMemory ne lit que les objets
observés de ce bb-probe ; le nom du premier mesh sert à identifier le temple.
Le payload retrouvé est 1048BC5100 et son compte local de matériaux vaut 13.
Les treize indices préparés sont exactement 0 à 12 ; chaque nom demandé est
identique au nom de la définition choisie. Le pointeur de callback est aussi lu
pour attribuer la fonction de hash. Aucun appel arbitraire à une adresse ELF.

[Capture des treize correspondances originales](preuves_avancement/2026-11-12/material-bindings-runtime.json).
L'index 0 correspond à la pierre peinte des murs, 2 au matériau de visage
`tmp-face-gold:03,...`, 7 aux colonnes et 9 aux plantes ; ces noms ne sont pas
des chemins de textures ni une preuve de leur rôle précis dans le shader.

Le premier lancement diagnostique utilisait C2A960 sans préfixe 0x : le harness
l'a rejeté avec `bad BB_GUEST_HOOKS entry`, suivi d'une faute hôte lors de sa sortie.
Il n'a pas exécuté le guest. La syntaxe correcte a permis la capture de 50 secondes
et le timeout sans faute. Les validations finales n'utilisent aucun hook.

## Implémentation et validation

Conclusion : la correspondance mesh → index de matériau local est établie et portée.
Cela ne crée aucune instance de matériau, ne complète aucun login moteur et
n'associe pas encore les textures au rendu. L'image éclairée reste inchangée.
Modification : champs material_name, material_name_hash et material_index dans
les snapshots ; préparation d'une table hôte triée par (hash, index), puis lookup
du premier hash correspondant. Les collisions reproduisent la règle originale,
même si deux noms différents partagent un CRC. Un nom NULL a le hash 0.
La table source n'est jamais modifiée, y compris le champ brut d'index +50.
Les pointeurs, les terminaisons, les comptes et les étendues sont contrôlés avant
publication atomique ; le budget cumulé de chaînes est borné par la taille de page.
Les noms et indices sont possédés après destruction des pages sources.

Fichiers modifiés : geometry_preview.h/.cpp, package_manager.cpp,
u2_engine_tests.cpp, plan natif, ce rapport et la capture JSON.
Tests ajoutés : seize assertions. Treize indices réels comparés à l'observation,
premier nom exact, CRC de beta/alpha, table réordonnée, clé avec bit haut,
absence et casse, doublon, collision de deux noms différents
(`material:5bc97c8a` et `material:d5bf1bfc`, hash 2C62520D), comptes invalides,
table tronquée et borne exacte, chaînes non terminées/étrangères, noms NULL,
table vide et conservation des données après destruction de la page.
Build : deux exécutables compilés/déployés, diag_material_build.log.
Test 6 secondes : SelfTest 886, full-body 831, moteur 387/387,
STOPPED_AFTER_TIMEOUT ; journal complet sans faute ni assertion nouvelle.
Test natif : 381/381, materials=13, délai de six secondes atteint et sortie 0.
Test prolongé : référence originale 50 secondes avant modification, 371/371,
timeout sans faute avec le hook valide. Pas de test long supplémentaire pour
le lookup synchrone natif ; instrumentation désactivée dans les validations finales.
Régression observée : aucune dans les scénarios vérifiés, 889 détours inchangés.
Journaux : user/diag_material_baseline.log, diag_material_original.log (rejet),
diag_material_original_retry.log, diag_material_native.log,
diag_material_hybrid.log et diag_material_hybrid_console.log.
Commit : feat(native): resolve temple mesh material references.
Prochaine hypothèse : suivre les paramètres de chaque définition de matériau,
identifier leurs références de textures et les résoudre dans TEXTURE_TABLE.
Le chargement des images, leurs formats et l'échantillonnage restent à établir.
