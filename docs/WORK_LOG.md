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

