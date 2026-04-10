# Task 02: Multipass Renderer Refactor

## Goal
- Prepare renderer architecture for RT output and composition without breaking current raster rendering.

## Scope
- Introduce geometry pass, fullscreen pass, and present/composition stage.
- Add FBO and color targets for non-direct rendering.
- Move shader source to external files for maintainability.

## Affected Areas
- src/render/voxel_renderer.hpp
- src/render/voxel_renderer.cpp
- src/render/shader_program.hpp
- src/render/shader_program.cpp
- CMakeLists.txt

## Implementation Notes
- Preserve the current output parity for raster mode.
- Keep API boundaries clear between world draw and fullscreen passes.
- Add robust shader load error reporting.

## Definition of Done
- Raster mode still renders identical output.
- Fullscreen pass can present a texture target.
- Renderer lifecycle properly creates/destroys all new GL objects.

## Dependencies
- Task 01.

## Risks
- Resource lifetime bugs (FBO/texture cleanup and resize handling).
