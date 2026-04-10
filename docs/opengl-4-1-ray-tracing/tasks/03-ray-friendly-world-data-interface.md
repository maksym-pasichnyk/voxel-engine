# Task 03: Ray-Friendly World Data Interface

## Goal
- Expose voxel data in a traversal-friendly form independent from mesh data.

## Scope
- Create voxel sampling API suitable for ray traversal.
- Define compact material metadata used by RT shading.
- Preserve coordinate/indexing invariants across chunk boundaries.

## Affected Areas
- src/world/voxel_types.hpp
- src/world/voxel_world.hpp
- src/world/voxel_world.cpp

## Implementation Notes
- Keep chunk mesh generation decoupled from traversal storage.
- Ensure out-of-range and missing chunk behavior is deterministic.
- Validate negative world coordinate mapping.

## Definition of Done
- Traversal API can query block/material reliably.
- Cross-chunk lookups are correct at boundaries.
- Data model supports both CPU and GPU RT paths.

## Dependencies
- Task 01.

## Risks
- Hidden coupling with mesher assumptions.
