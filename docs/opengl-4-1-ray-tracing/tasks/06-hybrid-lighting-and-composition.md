# Task 06: Hybrid Lighting and Composition

## Goal
- Build a practical hybrid mode by combining raster and RT outputs with basic lighting.

## Scope
- Add direct sun + ambient shading in RT pass.
- Add optional shadow ray and lightweight AO approximation.
- Compose RT and raster outputs in final pass.

## Affected Areas
- src/render/voxel_renderer.cpp
- src/render/shader_program.cpp

## Implementation Notes
- Keep feature flags for expensive effects.
- Ensure visual continuity when toggling between modes.
- Maintain deterministic ordering of composition stages.

## Definition of Done
- Hybrid mode is stable and selectable.
- Lighting terms are configurable and documented.
- Composition does not break raster fallback.

## Dependencies
- Task 05.

## Risks
- Visual mismatch between raster and RT components.
