# Copilot Instructions: VoxelEngine

## Big Picture
- **Binary:** CMake builds a single `VoxelEngine` app; core logic lives in `src/main.cpp`.
- **Modules:** World + chunks ([src/world.hpp](../../src/world.hpp)), noise ([src/noise.hpp](../../src/noise.hpp)), geometry loading ([src/geom.hpp](../../src/geom.hpp)), image I/O ([src/image.hpp](../../src/image.hpp)), ID mapping ([src/id_map.hpp](../../src/id_map.hpp)), math ([src/math/**/*](../../src/math)).
- **Data flow:** `assets/textures.json` → load images → pack into a sprite atlas → `assets/geometry/*.json` → build per-face quads → chunk meshing → render via G-buffer → deferred lighting → screen composite.
- **Concurrency:** Terrain/mesh generation use `common::TaskGroup` workers; results are consumed on the main thread.

## Build & Run
- **Deps:** SDL3 is built from `deps/SDL-release-3.4.0`; GLEW (required) is found via `find_package(GLEW REQUIRED)`.
- **macOS prep:** `brew install glew` (SDL3 builds from the repo).
- **CMake:** From the repo root:
  ```bash
  mkdir -p cmake-build-debug
  cd cmake-build-debug
  cmake -DCMAKE_BUILD_TYPE=Debug ..
  cmake --build .
  ./VoxelEngine
  ```
- **Working dir:** Run from the build folder so relative asset paths like `assets/...` resolve.

## Assets & Blocks
- **Textures:** `assets/textures.json` maps names (e.g., `core:stone`) to PNG paths and optional `frames` for animation. Loaded via `register_textures()` in [src/main.cpp](../../src/main.cpp).
- **Geometry:** `assets/geometry/*.json` uses `elements` + `faces` with keys `front/back/left/right/top/bottom`. Parsed by `load_geometry()` in [src/geom.hpp](../../src/geom.hpp).
- **Registering:** Add entries in `register_geometries()` and `register_blocks()` in [src/main.cpp](../../src/main.cpp) using `g_block_id_map.index("namespace:name")` and `create_block(...)` (set `is_solid`, `is_transparent`, `tint`).

## World & Chunks
- **Sizes:** Chunk = 16×16×16 (`CHUNK_SIZE_*` in [src/world.hpp](../../src/world.hpp)). Height handled by 16 layers per column.
- **Coords:** Use `world_to_chunk_coords()`, `world_to_column_coords()`, `world_to_local()`; index via `chunk_block_index()`.
- **Mutation:** Use `WorldBatch` to set blocks and `apply_batch()` to commit; neighbors are marked dirty for remeshing.

## Rendering Pipeline
- **G-buffer:** Position `RGB32F`, Normal `RGB16F`, Albedo+Alpha `RGBA8` (see `ensure_gbuffer_fbo()` in [src/main.cpp](../../src/main.cpp)). Uniforms: `view`, `model`, `projection`, `g_block_atlas`, `u_write_mask`.
- **Deferred:** Uniforms `g_position`, `g_normal`, `g_albedo`, plus `u_view_position`, `u_light_direction`, `u_light_color`, `u_ambient_strength` (see shader setup in [src/main.cpp](../../src/main.cpp)).
- **Screen & Mask:** Final composite via screen quad; separate mask pass for transparency.

## Conventions
- **Handles:** Use `common::IdMap` for string→index (`core:*` naming). Never hardcode numeric IDs.
- **Math:** Use `FVector*`/`FMatrix*` types and helpers in [src/math/math.hpp](../../src/math/math.hpp).
- **Assets:** Keep new textures under `assets/textures/`; new geometry under `assets/geometry/`.
- **Paths:** Asset paths are relative to the build dir (e.g., `assets/...`).

## Controls & Debugging
- **Controls:** `1–7` switch block types; mouse to look; LMB removes; RMB places at the hit normal (see input loop in [src/main.cpp](../../src/main.cpp)).
- **Shader logs:** Compilation/link status is printed via `glGetShaderInfoLog`; check terminal if rendering fails.
- **Animated textures:** Frames assume square tiles; `update_animated_textures()` updates atlas regions each frame.

## Useful Pointers
- **Atlas:** `pack_textures()`/`update_animated_textures()` in [src/main.cpp](../../src/main.cpp).
- **Noise:** FBM layers + SIMD abstractions in [src/noise.hpp](../../src/noise.hpp) and [src/simd.hpp](../../src/simd.hpp).
- **Meshes:** Per-chunk opaque/transparency VAOs/VBOs are managed in [src/main.cpp](../../src/main.cpp) (`MeshChunk`).