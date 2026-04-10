# Task 04: CPU Reference Ray Tracer

## Goal
- Implement a correctness-first reference for voxel ray traversal.

## Scope
- Add CPU DDA first-hit traversal over voxel grid.
- Render low-resolution RT buffer and display as fullscreen texture.
- Add debug outputs: distance, normal, voxel id, traversal steps.

## Affected Areas
- src/render/voxel_renderer.cpp
- src/scene/fps_camera.cpp
- src/world/voxel_world.cpp

## Implementation Notes
- Use deterministic camera ray reconstruction from current view/projection.
- Prioritize correctness and diagnostics over speed.
- Keep debug visualizations switchable at runtime.

## Definition of Done
- CPU RT image is stable and visually correct.
- Debug outputs are available and interpretable.
- Results can be used as GPU reference baseline.

## Dependencies
- Task 02 and Task 03.

## Risks
- CPU reference may become too slow at high resolutions (use fixed low-res output).
