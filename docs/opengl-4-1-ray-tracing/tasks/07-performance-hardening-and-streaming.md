# Task 07: Performance Hardening and Streaming

## Goal
- Stabilize frame time and integrate RT data updates with chunk streaming behavior.

## Scope
- Update GPU RT resources for dirty chunks only.
- Add per-frame upload/update budget.
- Tune traversal limits and optional temporal stabilization.

## Affected Areas
- src/world/voxel_world.cpp
- src/render/voxel_renderer.cpp
- src/core/application.cpp

## Implementation Notes
- Prioritize elimination of frame-time spikes.
- Measure update cost during movement-heavy scenarios.
- Keep knobs configurable for quick hardware tuning.

## Definition of Done
- Dirty-only updates are verified.
- Upload budget prevents major frame spikes.
- Tuned presets exist for at least one quality profile.

## Dependencies
- Task 05 and Task 06.

## Risks
- Aggressive throttling can cause visible update lag.
