# N33M1A5 Digital Scan 3D

FFGL plugin for Resolume Arena: a single plugin intended to provide **Effect** and **Generator** modes for monochrome digital scans, point-cloud-like particles, horizontal scanlines and glitch dispersion.

## Status
Prototype integration branch. This is source code, not a tested release DLL. Windows x64 compilation and in-host validation are required before distribution.

## Build
Requires Visual Studio 2022 C++ workload and CMake 3.20+. The build fetches the official Resolume FFGL SDK from https://github.com/resolume/ffgl.

```powershell
cmake -S . -B build -A x64
cmake --build build --config Release
```

The GitHub Actions workflow builds the plugin on Windows and uploads the resulting DLL as a workflow artifact if compilation succeeds.

## Design targets
- One FFGL plugin with Effect and Generator modes
- Scanline, particle density, dispersion, direction, speed, scale, contrast, glow and palette controls
- Monochrome and user-tinted output
- Conservative real-time shader workload for live performance

## Validation checklist
- [ ] Windows x64 build succeeds
- [ ] Resolume Arena discovers plugin
- [ ] Effect mode samples and processes the input texture correctly
- [ ] Generator mode works without an input clip
- [ ] Parameters update live without stalling playback
- [ ] Alpha/output behavior verified in Arena
