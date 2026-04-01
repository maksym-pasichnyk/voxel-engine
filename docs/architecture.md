# Runtime Architecture

This document describes the code that is currently compiled into the `VoxelEngine` executable.

## Build Target

The root `CMakeLists.txt` defines one application target:

- `VoxelEngine`

Compiled source files:

- `src/main.cpp`
- `src/core/application.cpp`
- `src/platform/window.cpp`
- `src/scene/fps_camera.cpp`
- `src/game/player_controller.cpp`
- `src/world/voxel_world.cpp`
- `src/world/world_generator.cpp`
- `src/world/chunk_mesher.cpp`
- `src/render/shader_program.cpp`
- `src/render/voxel_renderer.cpp`

Linked dependencies:

- `SDL3::SDL3`
- `GLEW::GLEW`
- `mathematics::mathematics`

## Execution Flow

### Entry Point

`src/main.cpp` only constructs `core::Application` and calls `run()`.

### Application Lifecycle

`core::Application` owns the main runtime objects:

- `platform::Window`
- `scene::FpsCamera`
- `game::PlayerController`
- `voxel::VoxelWorld`
- `render::VoxelRenderer`

`Application::run()` performs the following loop:

1. Initialize the window.
2. Initialize the renderer.
3. Poll SDL events until shutdown.
4. Forward mouse events to the player controller.
5. Update camera movement from keyboard state.
6. Expand the voxel world around the camera position.
7. Query the window size.
8. Render visible chunks.
9. Swap buffers.

Shutdown destroys renderer-owned GPU state first, then tears down the SDL window and GL context.

## Platform Layer

`platform::Window` is responsible for:

- Calling `SDL_Init(SDL_INIT_VIDEO)`
- Requesting an OpenGL 4.1 core profile context
- Creating a `1280 x 720` resizable SDL window
- Creating the GL context
- Enabling VSync with `SDL_GL_SetSwapInterval(1)`
- Enabling relative mouse mode
- Initializing GLEW

The class also exposes window size queries and buffer swapping. Error reporting uses `std::println(stderr, ...)` and returns failure to the caller.

## Camera and Input

### `scene::FpsCamera`

The camera stores:

- World position
- Yaw and pitch angles in degrees
- Movement speed
- Mouse sensitivity

Important behaviors:

- Default spawn position is `(0, 48, -12)`
- Pitch is clamped to `[-90, 90]`
- Movement input is interpreted in camera-local space
- The view matrix is derived by inverting the world matrix

### `game::PlayerController`

Input handling is intentionally thin:

- Mouse motion rotates the camera
- `WASD` drives planar movement
- `Space` and `Left Shift` move vertically

There is no acceleration model, gravity, collision handling, or input rebinding layer.

## World Representation

### Voxel Types

`src/world/voxel_types.hpp` defines:

- `kChunkSize = 16`
- `kChunkVolume = 4096`
- `BlockId` values: `air`, `grass`, `dirt`, `stone`
- `ChunkCoord` and its hash function
- Helpers for chunk addressing and local voxel indexing

Coordinate helpers support negative world positions by using floor division and positive modulo.

### `voxel::Chunk`

Each chunk contains:

- A fixed array of block IDs
- Generation and dirty flags
- CPU-side mesh vertex storage
- GPU mesh handles (`vao`, `vbo`, vertex count, uploaded flag)

### `voxel::VoxelWorld`

The world stores chunks in an `std::unordered_map<ChunkCoord, Chunk, ChunkCoordHasher>`.

Core responsibilities:

- Create chunks on demand
- Generate voxel content when a chunk first appears
- Rebuild meshes after generation
- Resolve block lookups across chunk boundaries
- Return nearby chunks ordered by distance to the player
- Release GL resources during shutdown

`ensure_region(player_position, radius_chunks)` populates a rectangular area in X/Z and a fixed vertical band from chunk `y = -1` to `y = 3`.

`visible_chunks(player_position, radius_chunks)` returns generated chunks inside a radial X/Z distance test and sorts them nearest-first.

## Terrain Generation

`voxel::WorldGenerator` uses a simple analytic terrain function:

```text
height(x, z) = 20 + sin(x * 0.08) * 5 + cos(z * 0.06) * 7
```

Block selection rules:

- `world_y == height`: `grass`
- `height - 3 < world_y < height`: `dirt`
- `world_y < height`: `stone`
- otherwise: `air`

This generator is deterministic, cheap, and easy to modify, but it does not currently produce caves, biomes, water, structures, or vegetation.

## Mesh Generation

`voxel::ChunkMesher` converts solid voxels into triangle lists.

Key properties:

- Each block face is predefined as two triangles
- A face is emitted only when the neighboring block is `air`
- Per-vertex attributes are `position.xyz` and `color.rgb`
- Colors are derived from block type and darkened by a simple directional shade term

The current mesher produces straightforward geometry. It does not implement greedy meshing, index buffers, ambient occlusion, LODs, or transparency separation.

## Rendering

### `render::ShaderProgram`

This wrapper owns an OpenGL program object and provides:

- Shader compilation from source strings
- Program linking
- Log output on compile or link failure
- Program binding and destruction

### `render::VoxelRenderer`

The renderer is responsible for:

- Creating the shader program from inline GLSL strings
- Enabling depth test and back-face culling
- Switching OpenGL front-face winding to clockwise
- Lazily uploading per-chunk VBO/VAO data
- Clearing and drawing all visible chunk meshes every frame

The shader pipeline is intentionally simple:

- Vertex shader transforms voxel vertices by a single `u_view_projection` matrix
- Fragment shader writes the interpolated color directly to the framebuffer

There is no texture sampling, lighting pass, shadowing, post-processing, or render graph in the active runtime.

## Runtime Data Model

The current executable does not rely on an external asset directory.

That means:

- Shader source is embedded in C++ strings
- Block appearance comes from hard-coded colors in the mesher
- Terrain generation is code-driven rather than data-driven

This keeps the runtime small and easy to build, but it also means visual content is coupled directly to code.

## Known Constraints

- Chunk generation and meshing run synchronously on the main thread
- Chunks remain resident once generated
- Visibility is distance-based rather than frustum-based
- GPU uploads happen during rendering when a mesh is first needed
- There is no serialization, editor layer, or debug UI
- There is no external asset ingestion layer
- The project currently exposes no automated test suite through CMake

## Extension Points

The current design is small enough that likely evolution paths are clear:

- Replace analytic terrain with layered noise or biome-driven generation
- Move shaders out of source and into asset files
- Add a material or texture atlas system
- Introduce chunk eviction and asynchronous streaming
- Separate simulation, asset loading, and rendering concerns further
- Add gameplay systems such as collision, physics, and interaction