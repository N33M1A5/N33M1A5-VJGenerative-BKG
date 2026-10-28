# N33M1A5 Digital Scan 3D

FFGL **Effect** plugin for Resolume Arena. Applies animated monochrome digital scanning, scanlines, contrast shaping, glow and horizontal digital tearing to an input texture.

## Status
Development branch. A successful Windows x64 build proves compilation only; it does not prove that the plugin has been validated inside Resolume Arena. Do not treat this as a tested release until in-host testing is complete.

## Build
Requires Visual Studio 2022 C++ workload, CMake 3.20+, and vcpkg with GLEW installed for the x64-windows triplet. Set `VCPKG_ROOT` to the vcpkg root.

```powershell
cmake -S . -B build -A x64
cmake --build build --config Release
```

The GitHub Actions workflow builds the plugin on Windows and uploads the resulting DLL as a workflow artifact if compilation succeeds.

## Current controls
- Velocidade: animation rate
- Densidade: scanline frequency/strength
- Dispersao: digital tearing/noise amount
- Direcao: left/right displacement direction
- Contraste: monochrome signal contrast
- Brilho: highlight/glow amount

## Validation checklist
- [ ] Windows x64 build succeeds
- [ ] Resolume Arena discovers plugin
- [ ] Effect mode samples and processes the input texture correctly
- [ ] Each parameter visibly affects the output
- [ ] Parameters update live without stalling playback
- [ ] Alpha/output behavior verified in Arena

Generator mode is not implemented in this branch. The plugin is currently an input-texture Effect only.
