# Journal cumulatif des suivis

Chaque remplacement de CURRENT_WORK ajoute son contenu intégral, daté en UTC
et identifié par SHA-256. Ne pas réécrire les entrées historiques.
Consulter par recherche ciblée. Le premier CURRENT_WORK est versionné ;
la première rotation en conservera automatiquement le contenu ici.


---

## 2026-11-12T21:59:40.142326+00:00

SHA-256: `5de0da14dc36f62294bc95d1d309ee3ea48b8b0c61b61b40d7a2486dadf7d040`

# Travail courant

- Cible : Uncharted 2, branche dev-native ; code validé abfe63c.
- Natif : 60 images, 44 premiers mips BC1 décodés ; 100 références de matériaux associées, dont 59 avec pixels.
- Validation du code : natif 456/456, hybride 462/462, six secondes sans crash.
- Preuves : [matériaux/images](NATIVE_MATERIAL_IMAGE_BINDINGS.md), [chargement](NATIVE_TEXTURE_TABLE_LOADING.md).
- Capture sampler : hybride 45 secondes, 462/462, STOPPED_AFTER_TIMEOUT. Deux samplers présents avec la signature des colonnes ; shader préparé f1ef5c58, 10 images, 2 samplers.
- Trace non commitée : preuves_avancement/2026-11-12/temple-samplers-capture.txt. Raw0 : 00fff00000000400 / 00fff00000000200 ; raw1 commun : 4000000008f00000.
- Champs backend : Wrap XYZ, AnisoLinear min/mag, mip Linear ; anisotropie différente. Co-présence prouvée, association à l'instruction précise encore ouverte.
- Instrumentation retirée, rebuild propre réussi ; test après retrait et rapport sampler à terminer. Logs : user/diag_temple_sampler_*.
- Prochaine expérience : identifier l'utilisation des samplers dans le shader, puis interpolation UV et échantillonnage testés avant application au viewer.
- Préserver : sous-module gpu/third_party/fsr-vulkan modifié, .agents/, scratch/diag_baseline_3d5dded/.
- Travail actuel : instructions compactes et archivage du suivi ; aucun changement de comportement du jeu.

<!-- End of archived handoff -->


---

## 2026-11-12T22:21:26.765953+00:00

SHA-256: `ef9880df48be36cc7a4276baacd9a7bcf7946ca0bc531a8c99d30bcdf1bfbdbd`

# Travail courant

- Cible : Uncharted 2, branche dev-native ; code validé abfe63c.
- Natif : 60 images, 44 premiers mips BC1 décodés ; 100 références de matériaux associées, dont 59 avec pixels.
- Validation du code : natif 456/456, hybride 462/462, six secondes sans crash.
- Preuves : [matériaux/images](NATIVE_MATERIAL_IMAGE_BINDINGS.md), [chargement](NATIVE_TEXTURE_TABLE_LOADING.md).
- Capture sampler : hybride 45 secondes, 462/462, STOPPED_AFTER_TIMEOUT. Deux samplers présents avec la signature des colonnes ; shader préparé f1ef5c58, 10 images, 2 samplers.
- Trace non commitée : preuves_avancement/2026-11-12/temple-samplers-capture.txt. Raw0 : 00fff00000000400 / 00fff00000000200 ; raw1 commun : 4000000008f00000.
- Champs backend : Wrap XYZ, AnisoLinear min/mag, mip Linear ; anisotropie différente. Co-présence prouvée, association à l'instruction précise encore ouverte.
- Instrumentation retirée, rebuild propre réussi ; test après retrait et rapport sampler à terminer. Logs : user/diag_temple_sampler_*.
- Prochaine expérience : identifier l'utilisation des samplers dans le shader, puis interpolation UV et échantillonnage testés avant application au viewer.
- Préserver : sous-module gpu/third_party/fsr-vulkan modifié, .agents/, scratch/diag_baseline_3d5dded/.
- Travail actuel : instructions compactes et archivage du suivi ; aucun changement de comportement du jeu.

- Suivi : archivage testé (deux rotations, conservation exacte CRLF/LF, SHA-256, échec d'archive, entrée vide et verrou concurrent). Instructions optimisées appliquées.

<!-- End of archived handoff -->


---

## 2026-11-12T22:29:35.684934+00:00

SHA-256: `cc92eb64db9284ed6f99c0fd35f12f53e34c06b87a575c913b3b70e75e3bc3a9`

# Travail courant

- Cible : Uncharted 2, branche dev-native ; première carte de couleur appliquée au viewer natif. Base 338d654, build natif SHA-256 fb162bb13dc1f7f16f4208c61b43853b860cfe0fe1b2f48d532efe5e4e0bddde.
- État validé : sous-mesh 21 (matériau colonnes), carte BC1 64×64 réelle, UV corrigés en perspective, répétition et bilinéaire ; éclairage de prévisualisation en espace linéaire. Les autres meshes gardent leurs couleurs.
- Preuves originales : image comparée sur 5120 octets, UV sur 6648 octets ; shaders actifs f1ef5c58 / 50411a0b, carte primaire image1 / sampler0. Aucun diagnostic GPU conservé dans le code.
- Tests : build et déploiement réussis, natif 469/469 et sortie 0 après 6 s ; hybride 30 s, SelfTest 886, full-body 831, moteur 475/475, STOPPED_AFTER_TIMEOUT, sans crash ni nouvelle assertion.
- Rapport : [échantillonnage et limites](NATIVE_TEMPLE_COLOR_SAMPLING.md). [Capture réelle](preuves_avancement/2026-11-12/06-temple-pierre-texturee.png), galerie autonome et provenance mises à jour. Capture inspectée ; contrôle d'affichage HTML arrêté à la demande de l'utilisateur.
- Limites : premier mip seulement ; pas de LOD, anisotropie, mélange de cartes, matériau ou éclairage original. Packages Resolved, logins encore en attente. Zéro code PS4 exécuté en natif.
- Prochaine expérience : vérifier pixels résidents et trajet UV/carte/sampler du grand visage ; appliquer sa carte primaire si décodable, sinon établir le format manquant avant de le porter.
- Préserver : sous-module gpu/third_party/fsr-vulkan modifié, .agents/, scratch/diag_baseline_3d5dded/, trace antérieure temple-samplers-capture.txt non commitée.
- Journaux : user/diag_columns_final_{build,native,hybrid}.log, user/diag_columns_probe3.log ; captures brutes et cache shader conservés. Tentatives intermédiaires interrompues consignées dans le rapport.

<!-- End of archived handoff -->


---

## 2026-11-12T22:32:36.094032+00:00

SHA-256: `2104e3a36482324974c675ea73081b676f833bebc451a1f4921b0414df349bae`

# Travail courant

- Cible : Uncharted 2, branche dev-native ; première carte de couleur appliquée au viewer natif. Base 338d654, build natif SHA-256 fb162bb13dc1f7f16f4208c61b43853b860cfe0fe1b2f48d532efe5e4e0bddde.
- État validé : sous-mesh 21 (matériau colonnes), carte BC1 64×64 réelle, UV corrigés en perspective, répétition et bilinéaire ; éclairage de prévisualisation en espace linéaire. Les autres meshes gardent leurs couleurs.
- Preuves originales : image comparée sur 5120 octets, UV sur 6648 octets ; shaders actifs f1ef5c58 / 50411a0b, carte primaire image1 / sampler0. Aucun diagnostic GPU conservé dans le code.
- Tests : build et déploiement réussis, natif 469/469 et sortie 0 après 6 s ; hybride 30 s, SelfTest 886, full-body 831, moteur 475/475, STOPPED_AFTER_TIMEOUT, sans crash ni nouvelle assertion.
- Rapport : [échantillonnage et limites](NATIVE_TEMPLE_COLOR_SAMPLING.md). [Capture réelle](preuves_avancement/2026-11-12/06-temple-pierre-texturee.png), galerie autonome et provenance mises à jour. Capture inspectée ; contrôle d'affichage HTML arrêté à la demande de l'utilisateur.
- Limites : premier mip seulement ; pas de LOD, anisotropie, mélange de cartes, matériau ou éclairage original. Packages Resolved, logins encore en attente. Zéro code PS4 exécuté en natif.
- Direction planifiée : [attribution automatique par famille de matériaux](UNCHARTED_NATIVE_PLAN.md#attribution-automatique-materiaux), sans sélection manuelle des meshes ; jalon documentaire, pas encore implémenté.
- Prochaine expérience : inventorier les variantes du temple et les pixels disponibles, puis vérifier le trajet UV/carte/sampler du grand visage pour établir une famille réutilisable. Traiter les mips détaillés séparément de l'attribution.
- Préserver : sous-module gpu/third_party/fsr-vulkan modifié, .agents/, scratch/diag_baseline_3d5dded/, trace antérieure temple-samplers-capture.txt non commitée.
- Journaux : user/diag_columns_final_{build,native,hybrid}.log, user/diag_columns_probe3.log ; captures brutes et cache shader conservés. Tentatives intermédiaires interrompues consignées dans le rapport.

<!-- End of archived handoff -->
