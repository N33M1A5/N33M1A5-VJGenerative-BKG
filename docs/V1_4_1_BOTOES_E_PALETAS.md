# N33M1A5 VJ Gen V1.4.1 — buttons and professional palettes

## What changed on user feedback

Only the new engine **Particle Wave** has been approved by the operator. The other 17 experimental engines have been removed from the **user-facing engine selection** and are no longer selectable. They remain unused in the shader source for future R&D, not advertised as active effects.

Two **native FFGL option controls** are registered in immediate sequence, same API that rendered the successful original six classic buttons:

1. **Motor:** Nevoa, Tunel, Lasers, Celulas, Aurora, Orbes (six original buttons in exact order).
2. **Motor Adicional:** Classicos, Particle Wave (two option buttons, not a dropdown of 24).

Selecting any classic button switches to original mode. Selecting `Particle Wave` switches to the approved procedural shader. Selecting `Classicos` returns to the selected original engine.

The FFGL host is responsible for the precise layout; the official SDK does not guarantee identical pixel-perfect UI in all Resolume versions.

## Paleta

36 color presets displayed in **alphabetical order in Portuguese**: 9 original algorithms with the exact same GLSL IDs/visuals and 27 new hand-curated 3-stop cyclical gradients.

Acido Neon, Ambar, Arco-iris, Artico, Aurora Boreal, Azul Eletrico, Brasa, Cobre, Cyberpunk, Deserto, Dourado, Esmeralda, Floresta, Fogo, Galaxia, Gelo, Holograma, Jade, Lava, Limao Neon, Magenta Laser, Mono, Neon, Oceano, Outrun, Por do sol, Rosa Quartzo, Roxo Laser, Safira, Solar, Synthwave, Tropical, Turquesa, Ultravioleta, Vaporwave, Vermelho Rubi.

The `kPaletteShaderId` mapping ensures that selecting the alphabetized nine legacy palette names still invokes their original GLSL algorithms (0..8). `Arco-iris` remains the default. Extended preset IDs (9..35) select the custom-color variant of the **same original shader**, so their extra colors are available on all six classics and Particle Wave without editing the original `shaders.h`. The RGBA A/B palette editor remains below Paleta.

## Deliverables

The Windows x64 library is `VJGenerativeV141.dll`, FFGL ID `VJ17`, display name `VJ Gen V1.4.1`. It is **separate** from stable V1.0.1 and working V1.3 and is installed via `install/INSTALAR_V1_4_1.bat` and `install/INSTALAR_V1_4_1.ps1` into `Documents/Resolume Arena/Extra Effects`. Existing versions are left intact. A backup is made only if this exact same V1.4.1 DLL already exists.

## Automated verification

- FFGL source checks ensure exact six classic names and indices plus precisely two secondary options, no 24-option dropdown.
- Immutable source fingerprint of original V1.0.1 GLSL checked.
- 36 alphabetical palette names and a permutation mapping back to the nine legacy palettes.
- glslangValidator on the original shader, original with new presets/RGBA, and expansion shader with new presets/RGBA.
- Mesa/EGL OpenGL software rendering verifies six original engines, approved Particle Wave, animation, RGBA custom palette, and new Cyberpunk palette visibly changing **both** engine families.
- Microsoft MSBuild creates a Windows x64 FFGL DLL with `plugMain` and `SetLogCallback` exports.

**Open limitation:** Only the operator's Resolume Arena can establish the exact visual host control layout, GPU driver/runtime and show performance. Please test on a spare layer before relying on it at live events.
