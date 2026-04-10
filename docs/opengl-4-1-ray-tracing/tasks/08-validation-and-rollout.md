# Task 08: Validation and Rollout

## Goal
- Validate correctness, stability, and performance; define release policy.

## Scope
- Compare GPU RT against CPU reference in fixed camera scenes.
- Validate chunk-boundary behavior, camera motion, and resize handling.
- Define rollout mode: experimental-only or hybrid default.

## Affected Areas
- docs/opengl-4-1-ray-tracing/epic.md
- src/render/voxel_renderer.cpp
- src/core/application.cpp

## Implementation Notes
- Use reproducible validation scenes.
- Track acceptance criteria with pass/fail evidence.
- Keep rollback path to raster mode available.

## Definition of Done
- Acceptance checklist is complete.
- Known risks and mitigations are documented.
- Rollout decision is explicit and approved.

## Dependencies
- Task 07.

## Risks
- Missing edge-case coverage can hide late regressions.
