# Preuves d'avancement — Uncharted 2 natif

Dossier constitué le **12 novembre 2026**, fuseau Europe/Paris.

[Ouvrir le document HTML autonome](preuves_avancement/2026-11-12/preuves_avancement.html).
Les deux images sont intégrées dans ce fichier HTML, consultable hors ligne.

## Première étape : vrais sommets du temple

Capture conservée du premier aperçu validé, associé au commit `8199a62`.
Treize sous-meshes issus de `menu-temple.pak` : **12 383 positions**, dont
**11 576 visibles** depuis la caméra de départ.

![Nuage de sommets natif du temple](preuves_avancement/2026-11-12/01-temple-sommets.png)

## Deuxième étape : triangles en fil de fer

Nouvelle capture réalisée pour ce dossier sur la version `5c6750f`.
**13 680 triangles réels**, **24 130 arêtes visibles**. Les buffers d'indices
des treize sous-meshes ont été retrouvés dans les dessins du moteur original,
avec des indices de 16 bits et une topologie TriangleList sans primitive restart.

![Fil de fer natif du temple](preuves_avancement/2026-11-12/02-temple-fil-de-fer.png)

## Validation et provenance

| Contrôle | Résultat | Scénario |
| --- | --- | --- |
| Nouvelle capture native | 304/304 assertions, sortie 0 | Délai de 8 secondes atteint |
| Dernière validation hybride | 310/310 assertions, sans faute signalée | Timeout de 6 secondes |
| Stabilité du fil de fer | Sortie 0, sans faute signalée | Test précédent de 25 secondes |

Les captures sont des images réelles du framebuffer de `uncharted2-native.exe`,
en 1920 × 1080. La conversion BMP vers PNG ne comporte aucune retouche.
La première image est historique ; la seconde vient d'une nouvelle exécution.

- [Journal de la nouvelle capture](preuves_avancement/2026-11-12/validation-native.txt).
- [Empreintes SHA-256 des images et de l'exécutable capturé](preuves_avancement/2026-11-12/provenance.json).
- [Preuves du nuage de points](NATIVE_TEMPLE_POINT_PREVIEW.md).
- [Validation des indices dans l'original](NATIVE_TEMPLE_TRIANGLE_INDICES.md).
- [Preuves du fil de fer et du clipping](NATIVE_TEMPLE_WIREFRAME.md).

Les surfaces pleines, la profondeur et l'occlusion, les textures, l'éclairage
et les interactions du menu restent à porter. Les couleurs servent à distinguer
les sous-meshes, sans reproduire leurs matériaux. Cet aperçu emploie la caméra
de départ et une lentille de référence capturée ; son affichage n'exécute aucun
octet de code PS4.
