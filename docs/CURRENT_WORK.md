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
