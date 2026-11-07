# Historique & Rétrospective Méthodologique de Recompilation (Dossier `scratch/`)

> Ce document consigne l'historique de construction du portage PC d'**Uncharted 2: Among Thieves** (`uncharted_pc`), la méthodologie de reverse engineering par lots (Lots 1 à 25), et l'utilité des ~1 100 fichiers temporaires générés au cours du processus.

---

## 1. Vision d'Ensemble & Démarche de Recompilation

Le projet repose sur l'exécution native du code x86_64 extrait des ELF PS4 (`big2-ps4_Shipping.elf`) sous Windows, couplée à un moteur de remplacement en C pur (`src/runtime_u2_*.c`) compilé avec MSYS2 CLANG64.

Pour porter et fiabiliser plus de **930 fonctions complexes** du moteur Naughty Dog (Dynamic Component system, physique, animation de Drake, cinématiques, rails, splines, audio WASAPI, etc.), le développement a été segmenté en **25 lots de recompilation séquentiels**.

Le dossier `scratch/` a servi de banc d'essai éphémère, de laboratoire d'expérimentation et de chaîne d'outillage automatisée.

---

## 2. Anatomie et Utilité des Fichiers Historiques

### A. Les Snippets Binaires Extraits (`*.elf`, `*.bin`, `*.asm`) — ~685 fichiers
* **Utilité** : Lors de la décompilation d'une fonction, plutôt que de manipuler l'ensemble de l'ELF (plus de 26 Mo), des scripts extrayaient la plage exacte d'octets de la routine (`lot24_a03700.elf`, `sub_da1c.elf`, `crash_snippet.bin`, etc.).
* **Rôle** : Ces petits fichiers permettaient un désassemblage instantané et isolé via `llvm-objdump -d`, l'inspection des opcodes AVX/SSE (`vmovaps`, `vucomiss`), et la vérification des prologues/épilogues sans bruit parasite.

### B. Les Scripts de Cartographie & Scanners de Références (`find_*.js`, `get_*.js`) — ~75 fichiers
* **Utilité** :
  * Découverte de chaînes internes et d'assertions (`find_strings.js`, `find_alloc_strings.py`, `find_asserts.ps1`).
  * Repérage des tables virtuelles et fonctions membres (`find_vtable_158.js`, `find_vtable_d0.js`, `find_bsf_ptr.js`).
  * Analyse des structures spécifiques : caméras (`find_cam.js`), rails (`find_rail.js`), train (`find_train.js`), audio (`find_audio.js`, `find_nearby_audio.js`), et système de scripts (`find_scoped.js`, `find_symbol_hash.js`).
* **Rôle** : Rapprocher les adresses virtuelles (VA) PS4 des fichiers sources C++ originaux de Naughty Dog identifiés dans les assertions (`ndlib/...`).

### C. Les Désassembleurs Thématiques Spécialisés (`disasm_*.js`, `dis_*.js`) — ~150 fichiers
* **Utilité** :
  * Analyse ciblée de sous-systèmes critiques :
    * Véhicules et séquence du train (`disasm_train_*.js`, `disasm_tank_helper.js`).
    * Système cinématique et lecture vidéo (`disasm_movieupdate.js`, `disasm_playmovie.js`, `disasm_startmovie.js`).
    * Gestion de la caméra et visée de Drake (`disasm_cam*.js`, `disasm_aim_dir*.js`, `disasm_stick*.js`).
    * Particules et FX (`disasm_fx_*.js`, `disasm_snow_*.js`).
    * Pipelines Splines et Rails (`disasm_rail_lot8.js`, `disasm_spline_lot8.js`).
    * Chaîne de mixage DSP Audio (`dis_137430.js`, `disasm_audio_lot7.js`).
* **Rôle** : Reconstituer la logique algorithmique précise, les décalages de champs (`struct offsets`) et les conventions de registres SysV AMD64 avant de rédiger les équivalents natifs en C.

### D. Les Inspecteurs de Structures & Vtables (`read_*.js`, `inspect_*.js`) — ~40 fichiers
* **Utilité** :
  * Décryptage des tables de saut `switch` (`read_switch_table.js`, `read_ucc_jt.js`).
  * Lecture de la Global Offset Table et des relocations (`read_got_u2.js`, `read_full_vtable_relocs.js`).
  * Validation des tables de vtables C++ à 32/64 octets (`read_vtable_32.js`, `read_vtable_file.js`).
  * Inspection des blocs mémoires et de la mémoire VRAM (`read_context_*.js`, `inspect_memory.js`).
* **Rôle** : Garantir que les pointeurs de méthodes virtuelles appelés par l'ELF pointent bien vers des vtables compatibles et cohérentes en mémoire hôte.

### E. Les Scripts d'Application et Générateurs de Lots (`apply_lot*.js`) — ~35 fichiers
* **Utilité** : De `apply_lot9.js` jusqu'à `apply_lot25_scene.js`.
* **Rôle** : Ces scripts prenaient les métadonnées de décompilation analysées et injectaient automatiquement les signatures, prototypes et tables de mapping dans `src/runtime_u2_*.c` et `tools/u2_map/u2_engine_map.json`. Une fois le lot compilé et validé par git, ces scripts d'injection devenaient redondants.

### F. Les Validateurs de Non-Régression & Audits (`check_*.js`, `verify_*.js`) — ~30 fichiers
* **Utilité** :
  * Détection de doublons entre lots (`check_lot10_dups.js`).
  * Résolution des conflits d'adresses (`check_lot18_conflicts.js`, etc.).
  * Vérification des offsets TLS et des bindings (`check_tls_vars.js`, `check_binding_254.js`).
* **Rôle** : Contrôle qualité permanent pour s'assurer qu'un nouveau lot n'écrasait pas des hooks validés lors d'un lot antérieur.

---

## 3. Outils Essentiels Préservés en Production

À l'issue du nettoyage, les **7 outils pivots** maintenus dans `scratch/` sont :

1. **`build_msys2.ps1`** : Script d'automatisation de compilation (CMake + Ninja sous MSYS2 CLANG64).
2. **`run_test.ps1`** : Harness d'exécution automatique avec timeout, capture d'assertions et surveillance du cycle de vie de `bb-probe.exe`.
3. **`auto_test.py`** : Moteur Python de monitoring et de synchronisation des logs pour `run_test.ps1`.
4. **`u2_disasm.js`** : Désassembleur instantané en ligne de commande (`node scratch/u2_disasm.js <VA_HEX> [longueur]`), avec résolution symbolique Naughty Dog et strings rip-relatives.
5. **`status_report.js`** : Métriques globales de complétion du portage (comptage de symboles résolus, detours actifs, fonctions natives).
6. **`audit_detours.js`** : Outil d'intégrité croisée entre le code source C et `tools/u2_map/u2_engine_map.json`.
7. **`test_nd_hash.js`** : Calculateur officiel des fonctions de hachage de symboles Naughty Dog (32-bit et 64-bit).

---

## 4. Synthèse

Les ~1 100 fichiers supprimés représentent le **journal de fouille archéologique** ayant permis de passer de 0% à plus de 930 fonctions natives documentées et fonctionnelles. Leur raison d'être a été pleinement transférée dans :
* Le code source C (`src/runtime_u2_*.c`),
* La base de connaissances symboliques (`tools/u2_map/u2_engine_map.json`),
* Le registre des décisions d'architecture (`ADR.md`).
