# Directives de Développement & Protocole Rapide (Agent Antigravity)

Ce document résume le flux de travail optimisé pour aller droit au but sans perte de temps.

---

## 1. Règle d'or : Zéro Décompilation Massive à l'Aveugle
* **La baseline fonctionnelle de référence est `e80b482`**.
* Ne JAMAIS injecter de lots de fonctions sans tester immédiatement le jeu.
* L'ancien état (Lots 1 à 25) est archivé sur la branche distante `dev_archive_lots`. Il sert de référence de consultation.
* Chaque fonction C réécrite doit être testée et validée unitairement avec `run_test.ps1 6` avant tout commit.

---

## 2. Commandes d'Action Rapide

### Compilation (MSYS2 CLANG64)
```powershell
powershell -ExecutionPolicy Bypass -File scratch/build_msys2.ps1
```

### Test Runtime Immédiat (6 secondes)
> Nécessite `BypassSandbox: true` pour accéder aux fichiers du jeu dans `Desktop\Uncharted\CUSA02343`.
```powershell
powershell -ExecutionPolicy Bypass -File scratch/run_test.ps1 6
```

### Désassemblage d'une Routine ELF PS4
```powershell
node scratch/u2_disasm.js <offset_hex> <longueur_octets>
# Exemple: node scratch/u2_disasm.js 0x00A127D0 40
```

### Rapport de Statut des Fonctions DC
```powershell
node scratch/status_report.js
```

---

## 3. Protocole d'Intégration d'un Nouveau Détour
1. **Consulter l'archive ou désassembler** : Examiner l'implémentation originale dans `scratch/u2_disasm.js` ou dans `dev_archive_lots`.
2. **Implémenter dans le bon module** :
   * `src/runtime_u2_player.c` : Fonctions joueur, armes, mouvements.
   * `src/runtime_u2_npc.c` : IA, ennemis, patrouilles.
   * `src/runtime_u2_scene.c` : Acteurs de scène, caméras, physique, décors.
   * `src/runtime_u2_event.c` : Événements, déclencheurs, cinématiques.
   * `src/runtime_u2_audio.c` : Audio, dialogues, effets sonores.
3. **Respecter l'ABI** : `ABI` (`__attribute__((sysv_abi))`) obligatoire.
4. **Compiler** : `scratch/build_msys2.ps1`.
5. **Tester en direct** : `scratch/run_test.ps1 6` (Doit afficher `STOPPED_AFTER_TIMEOUT` sans crash).
6. **Committer** : Commit atomique ciblé sur `dev`.
