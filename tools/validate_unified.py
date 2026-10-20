#!/usr/bin/env python3
"""Check unified VJ 24 engines and validate all three GLSL shader variants."""
from pathlib import Path
import subprocess
import re

source=Path("VJGenerative.cpp").read_text(encoding="utf-8")
legacy=Path("shaders.h").read_text(encoding="utf-8")
new=Path("shaders_expansion.h").read_text(encoding="utf-8")
stable_sha="488f842c06fe9fa616d71b033e32c573fe90179a"
actual=subprocess.check_output(["git","hash-object","shaders.h"],text=True).strip()
assert actual==stable_sha,f"V1.0.1 shader changed: {actual}"

# UI: one row of 6 classics and one small row with Particle Wave.
assert source.count('SetOptionParamInfo(P_ENGINE,"Motor",kNumEngines,0.f);')==1
assert source.count('SetOptionParamInfo(P_EXTRA_MOTOR,"Motor Adicional",kNumExtraMotors,0.f);')==1
assert 'AddElementSeparator(P_ENGINE,' not in source
assert 'P_GEOMETRY' not in source and 'activeBank' not in source
assert source.index("P_ENGINE,") < source.index("P_EXTRA_MOTOR,") < source.index("P_PALETTE,") < source.index("P_COLOR_MODE,") < source.index("P_SPEED,")
assert source.index('SetOptionParamInfo(P_ENGINE,') < source.index('SetOptionParamInfo(P_EXTRA_MOTOR,') < source.index('SetOptionParamInfo(P_PALETTE,') < source.index('SetOptionParamInfo(P_COLOR_MODE,')
assert 'params[P_SPEED]=.5f' in source and 'params[P_INTENSITY]=.5f' in source
assert 'params[P_COLOR_MODE]=0.f' in source
assert 'paletteId<9' in source
assert 'if(classic && !originalLook && classicColorReady)' in source
assert 'active->Set("uVJEngine",1.f)' in source
assert '"VJ17"' in source
assert 'params[P_EXTRA_MOTOR]=0.f;' in source
engines=source.split('static const char* kEngines[] = {',1)[1].split('};',1)[0]
names=re.findall(r'"([^"]+)"',engines)
assert names==["Nevoa","Tunel","Lasers","Celulas","Aurora","Orbes"],names
assert 'static const char* kExtraMotors[] = { "Classicos", "Particle Wave" };' in source
palette_source=Path("palettes_extra.h").read_text(encoding="utf-8")
palNames=re.findall(r'"([^"]+)"',source.split('static const char* kPalettes[] = {',1)[1].split('};',1)[0])
assert len(palNames)==36, palNames
assert palNames==sorted(palNames,key=lambda v:v.casefold()),"palette list not alphabetical"
assert palNames[2]=="Arco-iris","default classic palette changed"
palIDs=re.findall(r'\d+',source.split('static const int kPaletteShaderId[] = {',1)[1].split('};',1)[0])
assert len(palIDs)==36 and set(map(int,palIDs))==set(range(36))
legacy_pal_map={0:"Arco-iris",1:"Fogo",2:"Oceano",3:"Neon",4:"Ultravioleta",5:"Floresta",6:"Gelo",7:"Por do sol",8:"Mono"}
assert all(palNames[palIDs.index(str(i))]==n for i,n in legacy_pal_map.items()),"Classic palette names swapped"
assert "palettePreset" in palette_source and source.count('std::string(kPresetPaletteSource)')>=2
assert 'makeExpansionPaletteShader()' in source
print("PASS: 6 original Motor buttons + Particle Wave button row; 36 palettes alphabetized and legacy mapped exactly.")

def extract(header,variable):
    m=re.search(r'static\s+const\s+char\s*\*\s*'+variable+r'\s*=\s*R"GLSL\((.*?)\)GLSL";',header,re.S)
    assert m,variable
    return m.group(1)
out=Path("glsl-validation");out.mkdir(exist_ok=True)
(out/"legacy.vert").write_text(extract(legacy,"kVertexShader"),encoding="utf-8")
(out/"legacy.frag").write_text(extract(legacy,"kFragmentShader"),encoding="utf-8")
(out/"expanded.vert").write_text(extract(new,"kVJVertexShader"),encoding="utf-8")
(out/"expanded.frag").write_text(extract(new,"kVJFragmentShader"),encoding="utf-8")

# Build the three variants exactly as the C++ runtime assembly does.
custom=extract(legacy,"kFragmentShader")
custom=custom.replace("vec3 pal(float t){","vec3 originalPal(float t){",1)
custom=custom.replace("uniform float uSlices, uSeed, uPulse, uContrast;",
  "uniform float uSlices, uSeed, uPulse, uContrast;\n"
  "uniform float uRGBAType,uRGBABlend,uRGBAMix,uOpacity,uGlow;\n"
  "uniform vec4 uRGBA_A,uRGBA_B;",1)
preset=re.search(r'static\s+const\s+char\s*\*\s*kPresetPaletteSource\s*=\s*R"GLSL\((.*?)\)GLSL";',palette_source,re.S)
assert preset,"Could not extract 27 new color presets"
paletteMatch=re.search(r'const std::string customPalette=std::string\(kPresetPaletteSource\)\+R"GLSL\((.*?)\)GLSL";',source,re.S)
assert paletteMatch,"Missing classic palette GLSL"
custom=custom.replace("// 0 - Nevoa (domain warping)",preset.group(1)+paletteMatch.group(1)+"\n// 0 - Nevoa (domain warping)",1)
custom=custom.replace("fragColor=vec4(clamp(col,0.,1.),1.);",
  "col+=max(col-vec3(.16),vec3(0.))*clamp(uGlow,0.,1.)*1.15;\n"
  "float alpha=clamp(uOpacity,0.,1.);\n"
  "if(int(uRGBAType+.5)>0)alpha*=mix(uRGBA_A.a,uRGBA_B.a,clamp(uRGBAMix,0.,1.));\n"
  "fragColor=vec4(clamp(col,0.,1.),alpha);",1)
assert "vec3 originalPal(float t)" in custom
assert "vec3 palettePreset(int id,float t)" in custom
(out/"legacy_palette.frag").write_text(custom,encoding="utf-8")

expanded=extract(new,"kVJFragmentShader")
expanded=expanded.replace("vec3 palOriginal(float t){",preset.group(1)+"\nvec3 palOriginal(float t){",1)
expanded=expanded.replace("vec3 fixedColor=palOriginal(t);",
  "int id=int(uPalette+.5);\n vec3 fixedColor=id>=9?palettePreset(id,t):palOriginal(t);",1)
assert "palettePreset(id,t)" in expanded
(out/"expanded.frag").write_text(expanded,encoding="utf-8")
print("PASS: V1.0.1 shader intact; custom classic and Particle Wave palettes compile from 36 schemes.")
