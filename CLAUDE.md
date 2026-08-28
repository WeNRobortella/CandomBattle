# CLAUDE.md

Unreal Engine C++ project. 2D side-scrolling action/platformer using PaperZD (flipbook/skeletal 2D animation) and PaperFlipbook components. Journal: [JOURNAL.md](JOURNAL.md).

## Architecture

- `Source/CandomBattle/` — gameplay C++ classes, paired with Blueprint subclasses in `Content/Blueprints/`.
- `AEnemy` (`Enemy.h/.cpp`) — base class for enemies, extends `PaperZDCharacter`. State machine via `EEnemyState` (Idle/Patrol/Chase/Dead), player detection via `USphereComponent` overlap events, `ReceiveDamage` virtual for damage handling. Concrete enemies: `Snake_Enemy`, `Spider_Enemy`, `Alligator_Enemy`, each with per-instance `FTimerHandle`-based movement timers (moved off world Tick polling) using Cos-based movement curves.
- `APlatform` (`Platform.h/.cpp`) — moving/rotating platform actor. Combines `UInterpToMovementComponent` (spline-based linear interp) and `URotatingMovementComponent` (orbit rotation), each independently toggleable (`ActivateInterpFactor`/`ActivateRotationFactor`). Sync model: `Distance` is always authoritative (from spline/geometry); an `EInterpAuthAttribute`/`ERotationAuthAttribute` enum picks whether `Period` or `Speed` drives the cycle from that distance (a "last-field-touched" auto-detect approach was considered and rejected as fragile). Orbit tangential speed: `RotationMovementSpeed = 2π·Radius / (RotationBasePeriod × RotationCycleFactor)`. `MovementPath` (`USplineComponent`) is synced from `ControlPoints` in `OnConstruction` as an editor-only path visualizer, mirroring the `RotationRadius` sphere pattern — **gotcha**: its debug-line draw depth-tests against the scene, so it can lose a depth tie and vanish when exactly coplanar with a background sprite; fixed here with a small constant Y offset, not a render/sort-priority setting. Also supports `FallingPlatform` behavior: on player contact, a delay timer triggers `SetSimulatePhysics(true)` on `BoxComp` (not root) for a gravity drop; uses `OnComponentHit` (not overlap) so the player can still stand on it; constrained to the 2D plane via `BodyInstance.DOFMode = CustomPlane` — **known open bug**: placed (non-newly-spawned) instances can retain `Default`/`SixDOF` DOFMode and still rotate under player weight; unresolved as of 2026-08-19.
- `PlayerCharacter`, `PlayerHUD`, `GameOverHUD`, `CandomBattleGameMode`, `CandomBattleGameInstance` — core gameplay/UI scaffolding.
- `ArrowProjectile`, `FallHazard` — supporting gameplay actors.

## Conventions

- Prefer per-instance `FTimerHandle` timers over `Tick()` + bool-flag polling for punctual/one-off conditions (established during Snake/Spider/Alligator enemy work).
- Movement curves use `Cos` rather than `Sin` (changed from Sin during Alligator/Spider work).

## Active focus

Branch `Platforms_Pathing`: pathing gizmo/editor workflow for `APlatform` (spline-authored movement paths, see architecture note above). `Platform.cpp/.h`, `Platform_BP`, and `Level_1.umap` are currently mid-edit and uncommitted. `BeatLength`/`BeatRate` properties were started on `Platform.h` (movement-period control) but left with no logic yet. Level 1 design target: platforms over a fall hazard, interspersed with Spider enemies, synchronized via the `Distance`-authoritative model above.

Known open bug: falling-platform rotation-under-weight issue from 2026-08-19 (see architecture note) is unresolved.

## Workflow note

Advisor-only: user writes/edits all code and content themselves; Claude reviews, analyzes, and recommends but does not modify project code files unless explicitly asked in that instance.