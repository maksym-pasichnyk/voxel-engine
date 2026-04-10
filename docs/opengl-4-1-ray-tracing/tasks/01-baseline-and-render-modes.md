# Task 01: Baseline and Render Modes

## Goal
- Define measurable targets and introduce runtime renderer mode switching.

## Scope
- Add render modes: Raster, RT Debug, Hybrid.
- Add baseline frame timing metrics collection.
- Define RT constraints: resolution scale, max traversal steps, max ray distance.

## Affected Areas
- src/core/application.cpp
- src/render/voxel_renderer.hpp
- src/render/voxel_renderer.cpp

## Implementation Notes
- Add a runtime flag/enum for active render mode.
- Keep current raster path as default to minimize regression risk.
- Add simple frame timing instrumentation for render stages.

## Definition of Done
- Mode switching works at runtime.
- Baseline metrics are recorded and documented.
- Targets for RT prototype are approved.

## Dependencies
- None (starting task).

## Risks
- Timing instrumentation overhead can skew measurements if too granular.
