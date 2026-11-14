# Suivi courant — Port PC Uncharted 2

- Cible/branche : Chantier C3, normal maps BC5 menu-temple ; dev-native.
- État validé : build MSYS2 complet (`bb-probe.exe` + `uncharted2-native.exe`), `run_native_test.ps1 6` (sortie 0, EngineTest 679/679), `run_test.ps1 6` (`STOPPED_AFTER_TIMEOUT`, SelfTest 886, full-body 831, EngineTest 685/685). 58/60 images décodées, 228352 pixels ; 13 normal maps mises à niveau depuis chunk-21-tstrm.
- Preuves : [rapport](NATIVE_TEMPLE_NORMALS.md), [galerie](preuves_avancement/2026-11-14/preuves_avancement.html), [journal](preuves_avancement/2026-11-14/validation-c3-native.txt), PNG et provenance associés. Capture réelle inspectée, image embarquée vérifiée.
- Limite : lumière de viewer, CPU statique, pas de lightmaps ni ombres portées. UVs dégénérés : repli Gouraud.
- Prochaine expérience : prouver les données d'éclairage originales avant leur reproduction.
- Travail à préserver : fichiers locaux préexistants, `.agents/` et sous-module `gpu/third_party/fsr-vulkan` inchangés par cette tâche. Aucun code PS4 exécuté.
