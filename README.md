# 2D Verlet Particle Physics Engine

A single-threaded 2D particle physics simulation written from scratch in **C++20** with **Raylib**, capable of simulating **10,000+ particles at 60 FPS**.

https://github.com/user-attachments/assets/f58162c1-6727-443e-ab75-2299237c066c

## Key Features

- **Verlet Integration:** 8-pass sub-stepped solver for stable particle stacking and wall damping.
- **$O(n)$ Spatial Grid:** Flat, pre-allocated 1D grid buffers mapping world coordinates to cell buckets in $O(1)$ time.
- **Zero Per-Frame Allocations:** Pre-reserved `std::vector` storage and C++20 `std::span` views prevent heap allocations during the simulation loop.
- **Timing HUD:** Real-time overlay displaying FPS, particle count, and isolated physics vs. render times (`ms`).

## Controls

- **`Left Click (Hold):`** Spawn particles at cursor
- **`SPACE`:** Toggle pendulum emitter
- **`B`:** Spawn 10,000 particles (stress test)
- **`R`:** Reset simulation

## Build & Run

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
./build/Physics2D
```

### Easter Egg (`V`)

Technically, you can use any black-and-white video, though as a legendary graphics easter egg, *Bad Apple* is more than appropriate.

https://github.com/user-attachments/assets/2190afd9-86f8-4682-a15f-d778d248f8ac

To keep the repository lightweight, video frames are excluded via `.gitignore`. To run the easter egg locally, extract frames into a `frames/` directory in `assets/` :

```bash
mkdir frames
ffmpeg -i bad_apple.mp4 -vf "fps=60" frames/frame_%04d.png
```
