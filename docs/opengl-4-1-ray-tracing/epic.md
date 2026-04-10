# Epic: OpenGL 4.1 Ray Tracing

## Epic Summary
- Feature key: RT-GL41
- Objective: add a GL 4.1 compatible ray tracing path for voxel rendering with safe rollout through hybrid mode.
- Status: Planned
- Owner role: plan -> rendering

## Business and Technical Value
- Improves visual quality (accurate first-hit visibility, lighting effects, future shadows/reflections).
- Builds a scalable render architecture for post-processing and hybrid rendering.
- Preserves compatibility with current OpenGL 4.1 constraints.

## Scope
- In scope: voxel first-hit traversal, CPU reference tracer, GPU fragment RT pass, hybrid composition, validation and rollout gates.
- Out of scope: hardware RT APIs, full path tracing, full PBR material system, multithreaded world generation rewrite.

## Dependencies
- Existing renderer and world data flow in src/render and src/world.
- Stable camera matrix path in src/scene.
- OpenGL 4.1 context and capabilities.

## Task Index
- [Task 01 - Baseline and Render Modes](tasks/01-baseline-and-render-modes.md)
- [Task 02 - Multipass Renderer Refactor](tasks/02-multipass-renderer-refactor.md)
- [Task 03 - Ray-Friendly World Data Interface](tasks/03-ray-friendly-world-data-interface.md)
- [Task 04 - CPU Reference Ray Tracer](tasks/04-cpu-reference-ray-tracer.md)
- [Task 05 - GPU RT Fragment Pass](tasks/05-gpu-rt-fragment-pass.md)
- [Task 06 - Hybrid Lighting and Composition](tasks/06-hybrid-lighting-and-composition.md)
- [Task 07 - Performance Hardening and Streaming](tasks/07-performance-hardening-and-streaming.md)
- [Task 08 - Validation and Rollout](tasks/08-validation-and-rollout.md)

## Acceptance Criteria (Epic)
- Renderer mode switch supports Raster, RT Debug, and Hybrid.
- CPU and GPU first-hit outputs are consistent in debug scenarios.
- RT path is stable during camera motion, chunk streaming, and window resize.
- Performance targets are documented and met for the selected RT resolution scale.

## Risks
- Fragment DDA traversal cost may exceed frame budget.
- Chunk data upload bursts may introduce frame spikes.
- Coordinate space mismatches may cause incorrect hit positions.

## Open Questions
- Final RT quality/performance targets per hardware tier.
- Best voxel GPU data layout for target machines.
- Default rollout mode: hybrid-by-default or experimental toggle only.
