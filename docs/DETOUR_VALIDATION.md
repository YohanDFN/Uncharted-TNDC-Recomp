# Guide de Validation et d'Audit des Détours C Natifs (Uncharted 2 PC)

> Ce document définit la méthodologie officielle et l'architecture technique permettant de tester, valider et isoler les **930+ fonctions décompilées en C natif** (`src/runtime_u2_*.c`) exécutées conjointement avec l'ELF PS4 (`eboot.elf`).

---

## 1. Contexte & Problématique

Dans `uncharted_pc`, le binaire hôte (`bb-probe.exe`) combine deux moteurs d'exécution :
1. **Le code PS4 original** (`eboot.elf`) : tournant directement sur le CPU x86-64 hôte avec redirection des accès registres/mémoire.
2. **Le runtime C natif** (`src/runtime_u2_*.c`) : réimplémentations manuelles et décompilations par lots (Lots 1 à 25) couvrant plus de 930 fonctions réparties en 5 sous-systèmes (`player`, `npc`, `scene`, `event`, `audio`).

### Le Risque des Régressions Silencieuses
Lorsqu'un detour natif est activé, il remplace du code assembleur validé par Naughty Dog. Si une fonction native présente :
* Un décalage d'offset dans une structure interne Naughty Dog (DC / Component),
* Une divergence de convention d'appel (SysV ABI vs Microsoft x64, préservation des registres callee-saved `rbx`, `r12-r15`),
* Une valeur de retour non conforme ou un écrasement de `%rax` lors du saut `write_detour_jmp64`,
* Une boucle infinie ou un débordement de pile,

le moteur subit un crash ou un blocage en aval (écran noir, freeze d'image, désynchronisation des fibres).

---

## 2. Découplage Architectural : Infrastructure vs Modules Applicatifs

Pour tester les détours un à un, le système d'injection dans [`src/runtime_u2_script.c`](file:///c:/Users/Yohan/Desktop/uncharted_pc-0.3.3/src/runtime_u2_script.c) doit distinguer strictement deux catégories :

```mermaid
flowchart TD
    Init[Démarrage bb-probe] --> Infra[Couche 1 : Infrastructure Vitale]
    Infra --> Guards[40 Patchs de Sécurité ADR-001..ADR-040]
    Infra --> SyncHooks[detour_GfxWaitEopFence / detour_Job2]
    Infra --> StateHooks[detour_UCCApp_StateMachineUpdate]
    
    Infra --> Filter{Filtrage des Détours Applicatifs}
    
    Filter -->|BB_U2_MODULES=none| PureELF[Exécution ELF PS4 pur - 0 detour applicatif]
    Filter -->|BB_U2_MODULES=player| PlayerMod[Détours Player uniquement]
    Filter -->|BB_U2_MODULES=scene| SceneMod[Détours Scene uniquement]
    Filter -->|BB_U2_DETOUR_ALLOW| Granular[Validation chirurgicale fonction par fonction]
```

### Couche 1 : L'Infrastructure Vitale (Toujours Active)
Ne doit **jamais** être désactivée par un commutateur global :
* **State Machine & Boot** : `detour_UCCApp_StateMachineUpdate` (`0x00D59A50`), `detour_IntroState_Update` (`0x00A551B0`).
* **Synchronisation GPU / CPU** : `detour_GfxWaitEopFence` (`0x00B29F10`), `detour_Job2` (`0x003F68C0`), `hook_JobDispatch` (`0x00B3DB04`).
* **Gardes Mémoire et Allocateurs** : Patchs d'architecture ADR-001 à ADR-040 (FIOS2, PlayGo, EOP tick, audio caves, bounds checks).

### Couche 2 : Les Détours Applicatifs Modulaires (Testables un à un)
Les 930 fonctions regroupées par tables dans `s_module_getters` :
1. `u2_get_player_detours` (~100 fonctions : animations, stick, mouvements de Drake)
2. `u2_get_scene_detours` (~400 fonctions : caméras, splines, rails, streaming)
3. `u2_get_event_detours` (~150 fonctions : triggers, cutscenes, événements)
4. `u2_get_npc_detours` (~200 fonctions : comportement et physique des ennemis)
5. `u2_get_audio_detours` (~80 fonctions : filtres DSP, matrices audio)

---

## 3. Protocole de Validation en 4 Paliers

### Palier 0 : La Baseline "ELF Pur"
* **Objectif** : Vérifier que le moteur démarre, avance dans ses frames et initialise le rendu Vulkan sans **aucun** détour applicatif natif.
* **Configuration** : `BB_U2_MODULES=none`
* **Critère de succès** : Le cycle de boucle `AppMainLoop` tourne de manière stable sans interruption.

### Palier 1 : Validation Granulaire par Module
Activer un seul sous-système à la fois :
```powershell
# Test du module Player seul
$env:BB_U2_MODULES="player"
powershell -File scratch/run_test.ps1 10

# Test du module Scene seul
$env:BB_U2_MODULES="scene"
powershell -File scratch/run_test.ps1 10
```
* **Critère de succès** : Le jeu conserve la même stabilité que le Palier 0. Si un crash apparaît, le sous-système défaillant est identifié immédiatement.

### Palier 2 : Diagnostic par Dichotomie (Isolation Chirurgicale)
Lorsqu'un module provoque une anomalie :
1. Diviser les adresses virtuelles du module en deux moitiés (`0x00A00000 - 0x00B00000` et `0x00B00000 - 0x00D00000`).
2. Appliquer le filtre via `BB_U2_DETOUR_RANGE=min-max`.
3. Réduire l'intervalle par deux à chaque exécution.
4. En **moins de 8 essais**, la fonction exacte causant le conflit est isolée.

### Palier 3 : Traçage Télémétrique des Appels
Pour savoir exactement quelles fonctions C natives sont invoquées par le jeu au cours de la frame :
* Chaque entrée de detour peut journaliser son premier appel :
  ```c
  [Detour Hit] 0x00A14120: ExecuteScriptCode (caller=0x003F43BE)
  ```
* En cas d'exception, le journal indique précisément le dernier detour actif avant l'anomalie.

---

## 4. Checklist de Conformité d'une Fonction C Native

Avant de valider définitivement un detour :
- [ ] **Convention ABI** : Utilise le macro `ABI` (`__attribute__((sysv_abi))` ou équivalent) pour toute fonction appelée depuis l'ELF guest.
- [ ] **Préservation des Registres** : Les registres `rbx`, `rsp`, `rbp`, `r12`, `r13`, `r14`, `r15` doivent être strictement conservés conformément à la convention SysV.
- [ ] **Contrôle des Pointeurs NULL** : Tout pointeur d'instance (`this`) ou de ressource transmis par l'ELF doit être validé (`if (!ptr || (uintptr_t)ptr < 0x10000) return ...`).
- [ ] **Alignement Mémoire** : Tout accès SIMD/AVX (`vmovaps`) doit garantir un alignement de 16 ou 32 octets sous peine d'interruption matérielle.
- [ ] **Valeur de Retour** : S'assurer que le registre de retour (`rax` ou `xmm0`) correspond fidèlement au comportement attendu par le code assembleur original.

---

## 5. Commandes de Référence

```powershell
# Compilation propre MSYS2 CLANG64
powershell -ExecutionPolicy Bypass -File scratch/build_msys2.ps1

# Test standard (10s)
powershell -ExecutionPolicy Bypass -File scratch/run_test.ps1 10

# Audit de la base de mapping
node scratch/audit_detours.js

# Rapport d'avancement
node scratch/status_report.js
```

---

## 6. Journal des Jalons Validés

* **Milestone 295/843** : 295 fonctions DC natives réécrites en C (`ABI`) - Lot 11 (batch de 20 fonctions) validé unitairement sans crash :
  - `DcSetWhiteFade` (`0x00A106A0`)
  - `DcSetBlackFade` (`0x00A10700`)
  - `DcMakeInert` (`0x009FD850`)
  - `DcDisablePlatformControlOnObject` (`0x00A04E80`)
  - `DcDisableNavBlockerObject` (`0x00A04EF0`)
  - `DcUnblockNavPolyUnderObject` (`0x00A04FC0`)
  - `DcDumpLevelsInfo` (`0x00A04900`)
  - `DcLerpReflectionPlanes` (`0x00A02AA0`)
  - `DcSlowMoEffect` (`0x00A0EE10`)
  - `DcRailSetNpcArchetype` (`0x00AB9660`)
  - `DcNpcIgnoreExplosionDamage` (`0x002DC9C0`)
  - `DcGrenadeDirectorDisable` (`0x0069BA40`)
  - `DcGrenadeDirectorIsEnabledP` (`0x0069B9C0`)
  - `DcPlayerRunSpeed` (`0x0090B180`)
  - `DcGuarantee1Bullet` (`0x0090C8F0`)
  - `DcPlayMusicStinger` (`0x009FD3F0`)
  - `DcDebugDrawLocator` (`0x009FE000`)
  - `DcNetIsGameTypeP` (`0x00667E60`)
  - `DcForceHairAnim` (`0x00906840`)
  - `DcPlayerSetCornerClimbDistScale` (`0x0090DE90`)
* **Milestone 275/843** (`d654ccc`) : 275 fonctions DC natives réécrites en C (`ABI`) - Lot 10 (batch de 20 fonctions) validé unitairement sans crash :
  - `DcSpawnParticlesAtJoint` (`0x00A00950`)
  - `DcSpawnParticlesAttached` (`0x00A00960`)
  - `DcNetHideWaveScreen` (`0x00668850`)
  - `DcSetWalkMode` (`0x0090D4B0`)
  - `DcStringFromFloat` (`0x009FDE60`)
  - `DcDebugDrawLine` (`0x009FE090`)
  - `DcPlayMovie` (`0x009FE100`)
  - `DcStartRumble` (`0x009FF3D0`)
  - `DcStopRumble` (`0x009FF370`)
  - `DcNetAllPlayersDeadP` (`0x006694B0`)
  - `DcDebugDrawString` (`0x009FDFB0`)
  - `DcSetCharacterWetMaskState` (`0x00A02690`)
  - `DcUseWaterUnitForReflection` (`0x00A02C70`)
  - `DcLerpReflectionPlaneOfWaterUnits` (`0x00A02B10`)
  - `DcRemoveCoverActionPacksFromObject` (`0x00A04DC0`)
  - `DcRegenerateCoverActionPacksFromObject` (`0x00A04E20`)
  - `DcEnableNavBlockerObject` (`0x00A04F60`)
  - `DcBlockNavPolyUnderObject` (`0x00A05030`)
  - `DcKillEntity` (`0x00A09D60`)
  - `DcNpcSetToPlayerFaction` (`0x002DC960`)
* **Milestone 255/843** (`7972c6a`) : 255 fonctions DC natives réécrites en C (`ABI`) - Lot 9 (batch de 20 fonctions) validé unitairement sans crash :
  - `DcRenderSettingsApplyDefaults` (`0x00A10760`)
  - `DcSetObjectHealth` (`0x00A04A20`)
  - `DcGetObjectHealth` (`0x00A04B10`)
  - `DcTagBoolean` (`0x00A43700`)
  - `DcTagInt32` (`0x00A43880`)
  - `DcTagFloat` (`0x00A439D0`)
  - `DcTagSymbol` (`0x00A43C90`)
  - `DcTagSpecifiedP` (`0x00A43DE0`)
  - `DcNpcDisableFlinch` (`0x002DC1D0`)
  - `DcNpcSetPerfectAim` (`0x002DC490`)
  - `DcNpcSetMovementSpeedScale` (`0x002DD030`)
  - `DcNpcTriggerAmbush` (`0x002DD0D0`)
  - `DcNpcSetLeadDrakeSpline` (`0x002DFEE0`)
  - `DcNpcSetAccuracyMultiplier` (`0x002E00F0`)
  - `DcNpcSetCommGroupId` (`0x002E0430`)
  - `DcNpcMyBattleManagerId` (`0x002E4EF0`)
  - `DcNpcEnteredActionPackP` (`0x002DFFB0`)
  - `DcNpcLowerWeapon` (`0x002DBD00`)
  - `DcNpcReloadWeapon` (`0x002DBDA0`)
  - `DcNpcFireWeapon` (`0x002DBE80`)
* **Milestone 235/843** (`b04b800`) : 235 fonctions DC natives réécrites en C (`ABI`) - Lot 8 (batch de 20 fonctions) validé unitairement sans crash :
  - `DcSay` (`0x00A01A80`)
  - `DcSetForceTension` (`0x00667EE0`)
  - `DcUnparent` (`0x00A03940`)
  - `DcDisableFrameForward` (`0x00A038A0`)
  - `DcGetObjectId` (`0x00A0BEE0`)
  - `DcCameraAbandon` (`0x00A0E5E0`)
  - `DcSplineTransferPendingP` (`0x00A25A20`)
  - `DcSetMaxCharacterWetness` (`0x00A02510`)
  - `DcSetTimeToDryPeriod` (`0x00A02780`)
  - `DcForceReflections` (`0x00A02960`)
  - `DcUseReflectionPlane` (`0x00A029A0`)
  - `DcUseVisibleWaterUnitForReflectionWithTolerance` (`0x00A02B70`)
  - `DcUseVisibleWaterUnitForReflection` (`0x00A02BC0`)
  - `DcUseReflections` (`0x00A02C10`)
  - `DcSetRainRipplesState` (`0x00A027F0`)
  - `DcSetFocusableGrappled` (`0x00A0F2C0`)
  - `DcCameraDisable` (`0x00A0E500`)
  - `DcCalcStartPhase` (`0x00A0FB70`)
  - `DcCalcStartPhaseMaxFrames` (`0x00A0FAF0`)
  - `DcCalcStartPhaseRange` (`0x00A0FA70`)
* **Milestone 215/843** (`09af430`) : 215 fonctions DC natives réécrites en C (`ABI`) - Lot 7 validé unitairement sans crash (`DcEnableGadgetry`, `DcDisableGadgetry`, `DcNetRespawnFlag`, `DcNetSpawnTerritory`, `DcNetKillTerritory`).
* **Milestone 210/843** (`0134ce9`) : 210 fonctions DC natives réécrites en C (`ABI`) - Lot 6 validé unitairement sans crash (`DcKillAllDroppedWeapons`, `DcCalcNewPhaseFromSpeed`, `DcEntityAutoSpawnOnce`, `DcPartModuleLoadedP`, `DcDrawReticle`).
* **Milestone 205/843** (`34bfb38`) : 205 fonctions DC natives réécrites en C (`ABI`), couvrant audio, player et scene avec 0 crash runtime.
* **Milestone 200/843** (`aace663`) : 200 fonctions DC natives réécrites en C (`ABI`), compilées sous Clang64 et vérifiées en direct à l'amorçage avec 0 crash.
* **Baseline Initiale** (`e80b482`) : 172 fonctions DC natives. (+28 fonctions natives validées unitairement).
