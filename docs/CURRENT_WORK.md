# Travail courant

- Cible : Uncharted 2, branche dev-native ; itérations unitaires. Premier écran-titre visuel sur le temple, base ad88d7b ; SHA-256 natif 40c35ed58507b4de52e4bf8eb7a6add1d616578bc686f5fca50d4711ea194b22.
- État validé : logo original 2048×512 couleur BC1 + masque BC4 depuis menu.pak, invitation française et mentions relevées, affichage statique. Pierre 1024×1024 conservée ; autres meshes encore en couleurs de prévisualisation.
- Preuves : deux chaînes de 702464 octets identiques aux images GPU originales Thin1DThin. Référence visuelle hybride conservée du 11 novembre ; nouvelles tentatives de recapture originale noires/inexploitables, non utilisées comme preuve d'image.
- Tests : build/déploiement réussis ; natif 493/493, full-body 831, sortie 0 après 6 s ; hybride 30 s, SelfTest 886, full-body 831, 499/499, STOPPED_AFTER_TIMEOUT, sans crash, Guest fault ou nouvelle assertion. F1 masque/réaffiche le titre, testé 12 s et sortie 0.
- Rapport : [écran-titre et limites](NATIVE_TITLE_SCREEN_PREVIEW.md). [Capture native réelle](preuves_avancement/2026-11-13/01-ecran-titre-natif.png), inspectée ; galerie autonome et provenance enregistrées sans contrôle d'affichage HTML.
- Limites : police SDL et disposition provisoires, logo affiché avec inversion verticale de prévisualisation. Invitation statique : aucune transition/menu jouable. Pas de nouveau détour, de code PS4 exécuté en natif ou de promotion des packages à Ready.
- Prochaine expérience : relever dans bb-probe l'écran après X, ses contrôles et textes ; afficher cette liste vérifiée avant de porter la transition et les actions une à une. Ne pas réutiliser la liste manuelle du prototype comme preuve.
- Stratégies globales et familles conservées dans le plan pour plus tard ; streaming, LOD, éclairage et matériaux originaux restent incomplets.
- Préserver : sous-module gpu/third_party/fsr-vulkan modifié, .agents/, scratch/diag_baseline_3d5dded/, trace antérieure temple-samplers-capture.txt non commitée.
- Journaux : user/diag_title_validated_{build,native,hybrid}.log, user/diag_title_toggle_native.log, user/diag_menu_title_original.log. Premier rejet dû au budget VRAM du lecteur résident ; correction ciblée sans élargir son budget. Diagnostics temporaires retirés.
