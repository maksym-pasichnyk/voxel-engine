# Task 05: GPU RT Fragment Pass

## Goal
- Implement GL 4.1-compatible GPU first-hit ray traversal in a fullscreen fragment shader.

## Scope
- Reconstruct primary rays from inverse view-projection.
- Implement DDA traversal and first-hit shading on GPU.
- Support RT resolution scaling and upscale path.

## Affected Areas
- src/render/voxel_renderer.cpp
- src/render/shader_program.cpp
- src/scene/fps_camera.cpp
- src/world/voxel_world.cpp

## Implementation Notes
- Keep implementation compatible with OpenGL 4.1 limitations.
- Select and validate voxel GPU data layout (3D texture or indirection atlas).
- Expose debug mode parity with CPU reference where feasible.

## Definition of Done
- GPU RT pass renders stable first-hit output.
- Runtime resolution scaling works correctly.
- GPU output is comparable to CPU reference in debug scenarios.

## Dependencies
- Task 02, Task 03, Task 04.

## Risks
- Fragment shader traversal cost and bandwidth pressure.
