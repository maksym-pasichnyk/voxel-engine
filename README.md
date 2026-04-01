# VoxelEngine

VoxelEngine is a compact C++ voxel sandbox prototype built with SDL3, OpenGL 4.1, GLEW, and the local `mathematics` library. The current executable renders a procedurally generated block world, updates chunks around the player, and draws chunk meshes with a simple color-based shader pipeline.

## Current Status

- The active application is the runtime assembled in `src/core`, `src/platform`, `src/scene`, `src/game`, `src/world`, and `src/render`.
- CMake currently builds a single executable target: `VoxelEngine`.
- The runtime is self-contained: shaders are embedded in code and block appearance is generated procedurally in the mesher.
- There is no external asset directory in the current project layout.

## Requirements

- CMake 4.1 or newer
- A C++26-capable compiler
- OpenGL 4.1
- GLEW
- SDL3 is built from the vendored dependency in `deps/SDL-release-3.4.0`

On macOS, install GLEW before configuring the project:

```bash
brew install glew
```

## Build

Configure and build from the repository root:

```bash
cmake -S . -B cmake-build-debug -DCMAKE_BUILD_TYPE=Debug
cmake --build cmake-build-debug --target VoxelEngine
```

For a release build:

```bash
cmake -S . -B cmake-build-release -DCMAKE_BUILD_TYPE=Release
cmake --build cmake-build-release --target VoxelEngine
```

## Run

Start the executable from the chosen build directory:

```bash
./cmake-build-debug/VoxelEngine
```

The window requests an OpenGL 4.1 core profile, enables VSync, and captures the mouse in relative mode.

## Controls

- `W`, `A`, `S`, `D`: move on the horizontal plane
- `Space`: move up
- `Left Shift`: move down
- Mouse movement: rotate the camera
- `Esc`: exit the application

## Runtime Overview

The main loop is minimal and lives in `core::Application`:

1. Initialize the SDL window and OpenGL context.
2. Initialize the voxel renderer.
3. Poll SDL input events.
4. Update the FPS camera from player input.
5. Ensure nearby chunks exist around the player.
6. Render visible chunk meshes.
7. Present the backbuffer.

Chunk generation is synchronous in the current version. As the camera moves, the world allocates missing chunks, fills them with procedurally generated blocks, rebuilds the mesh, uploads GPU buffers on demand, and renders the visible set.

## Project Layout

```text
src/
  core/      Application lifecycle and main loop
  game/      Player input handling
  platform/  SDL window and OpenGL context setup
  render/    Shader compilation and voxel rendering
  scene/     FPS camera math and transforms
  world/     Chunk storage, terrain generation, and meshing

deps/
  mathematics/        Local math dependency used by the runtime
  SDL-release-3.4.0/  Vendored SDL3 source
  json/               Third-party JSON library kept in the repo

docs/
  architecture.md  Detailed technical documentation for the current runtime
```

## Rendering Notes

- Voxel geometry is meshed on a per-chunk basis.
- Faces are emitted only when the neighboring voxel is air.
- Vertex data stores position plus baked face color.
- The renderer uploads chunk VBO/VAO state lazily, only when needed.
- The projection uses a 70 degree field of view and a far plane of 512 units.

## Terrain Notes

- Chunk size is `16 x 16 x 16` voxels.
- Generated blocks are limited to `air`, `grass`, `dirt`, and `stone`.
- Terrain height is derived from a simple sine/cosine function over world X/Z coordinates.
- The application keeps chunk layers from `y = -1` through `y = 3` around the player.

## Limitations

- No persistence or save format
- No asynchronous chunk streaming
- No frustum culling; visible chunks are selected by radial distance in X/Z
- No external asset or material pipeline in the active renderer
- No automated tests are defined in the current CMake setup

## Further Reading

See `docs/architecture.md` for a subsystem-by-subsystem breakdown of the current executable.