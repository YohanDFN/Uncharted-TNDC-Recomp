# Travail courant

- Cible : Uncharted 2, branche dev-native ; itérations unitaires demandées, généralisation reportée. Carte de pierre 1024×1024 appliquée au sous-mesh 21. Base 30aff7a, SHA-256 natif 0ab848d5656ffb75a9c51de0e517d5448ccbeebafa8e7ac08f25b5ad67d00a50.
- État validé : ressource cooked exacte du matériau colonnes retrouvée dans chunk-21-tstrm, lecture d'une page et de sa seule chaîne BC1 ; UV et éclairage du viewer conservés. Le snapshot résident reste en 64×64 et les autres meshes gardent leurs couleurs.
- Preuves : chaîne 701440 octets identique à l'image GPU originale 1024×1024 Thin1DThin, SHA-256 b13de58bb674b994997db6f48f2ede2afa2075ba81b1058be40c0af40f96d0d8 ; mip4 de 2048 octets identique au premier mip résident 64×64.
- Tests : build et déploiement réussis ; natif 484/484, full-body 831, sortie 0 après 6 s ; hybride 30 s, SelfTest 886, full-body 831, 490/490, STOPPED_AFTER_TIMEOUT, sans crash, Guest fault ou nouvelle assertion. Original avant modification : 45 s, 475/475.
- Rapport : [carte détaillée et limites](NATIVE_TEMPLE_DETAILED_COLOR.md). [Capture réelle](preuves_avancement/2026-11-12/07-temple-pierre-1024.png) inspectée, galerie et provenance mises à jour. Aucun contrôle d'affichage HTML.
- Limites : chargement ponctuel du viewer, pas de politique de streaming, LOD ou anisotropie ; premier mip en bilinéaire, matériau original incomplet, aperçu statique CPU. Packages Resolved et logins encore en attente. Aucun nouveau détour, aucun code PS4 exécuté en natif.
- Prochaine expérience unitaire : vérifier la carte de couleur du grand visage, ses pixels disponibles et le trajet UV/sampler original ; l'appliquer à ce mesh seulement après preuve.
- Stratégies globales et familles de matériaux conservées dans le plan pour plus tard ; elles ne sont pas déclarées implémentées.
- Préserver : sous-module gpu/third_party/fsr-vulkan modifié, .agents/, scratch/diag_baseline_3d5dded/, trace antérieure temple-samplers-capture.txt non commitée.
- Journaux : user/diag_column_mips_build_validated.log, user/diag_column_mips_native_final.log, user/diag_column_mips_hybrid_final.log, user/diag_column_mips_original.log. Deux assertions initiales réfutent l'égalité entre nom de table relatif et nom cooked complet ; correction et test dédiés conservés dans le rapport.
