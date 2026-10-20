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

assert source.count("AddElementSeparator(P_ENGINE,")==3
for n in (6,12,18):assert f"AddElementSeparator(P_ENGINE,{n});" in source
assert 'SetOptionParamInfo(P_ENGINE,"Motor",kNumEngines,0.f);' in source
assert 'P_GEOMETRY' not in source and 'activeBank' not in source
assert 'P_ENGINE' in source and 'P_PALETTE' in source
assert source.index("P_ENGINE,")<source.index("P_PALETTE,")<source.index("P_COLOR_MODE,")<source.index("P_SPEED,")
assert source.index('SetOptionParamInfo(P_ENGINE,')<source.index('SetOptionParamInfo(P_PALETTE,')<source.index('SetOptionParamInfo(P_COLOR_MODE,')<source.index('SetParamInfof(P_SPEED,')
assert 'params[P_SPEED]=.5f' in source and 'params[P_INTENSITY]=.5f' in source
assert 'params[P_COLOR_MODE]=0.f' in source
assert 'originalLook=params[P_COLOR_MODE]<.5f' in source
assert 'if(classic && !originalLook && classicColorReady)' in source
assert 'active->Set("uVJEngine",(float)(motor-6))' in source
assert 'AddElementSeparator' in source and '"VJ15"' in source
assert source.count('SetParamElementInfo(P_ENGINE,i,kEngines[i],(float)i);')==1
engines=source.split('static const char* kEngines[] = {',1)[1].split('};',1)[0]
names=re.findall(r'"([^"]+)"',engines)
assert len(names)==24, f"Expected 24 motor names, found {len(names)}"
assert names[:6]==["Nevoa","Tunel","Lasers","Celulas","Aurora","Orbes"]
assert 'inverted' not in source
assert 'GetParameterDisplay' in source
print("OK original SHA, V1-0-1 visual modes, defaults and 24 contiguous Motor options")

def extract(header,variable):
    m=re.search(r'static\s+const\s+char\s*\*\s*'+variable+r'\s*=\s*R"GLSL\((.*?)\)GLSL";',header,re.S)
    assert m,variable
    return m.group(1)
out=Path("glsl-validation");out.mkdir(exist_ok=True)
(out/"legacy.vert").write_text(extract(legacy,"kVertexShader"),encoding="utf-8")
(out/"legacy.frag").write_text(extract(legacy,"kFragmentShader"),encoding="utf-8")
(out/"expanded.vert").write_text(extract(new,"kVJVertexShader"),encoding="utf-8")
(out/"expanded.frag").write_text(extract(new,"kVJFragmentShader"),encoding="utf-8")

# Rebuild the same opt-in shader source that makeClassicPaletteShader builds.
custom=extract(legacy,"kFragmentShader")
custom=custom.replace("vec3 pal(float t){","vec3 originalPal(float t){",1)
custom=custom.replace("uniform float uSlices, uSeed, uPulse, uContrast;",
  "uniform float uSlices, uSeed, uPulse, uContrast;\n"
  "uniform float uRGBAType,uRGBABlend,uRGBAMix,uOpacity,uGlow;\n"
  "uniform vec4 uRGBA_A,uRGBA_B;",1)
paletteMatch=re.search(r'const std::string customPalette=R"GLSL\((.*?)\)GLSL";',source,re.S)
assert paletteMatch,"Missing generated palette GLSL"
custom=custom.replace("// 0 - Nevoa (domain warping)",paletteMatch.group(1)+"\n// 0 - Nevoa (domain warping)",1)
custom=custom.replace("fragColor=vec4(clamp(col,0.,1.),1.);",
  "col+=max(col-vec3(.16),vec3(0.))*clamp(uGlow,0.,1.)*1.15;\n"
  "float alpha=clamp(uOpacity,0.,1.);\n"
  "if(int(uRGBAType+.5)>0)alpha*=mix(uRGBA_A.a,uRGBA_B.a,clamp(uRGBAMix,0.,1.));\n"
  "fragColor=vec4(clamp(col,0.,1.),alpha);",1)
assert "vec3 originalPal(float t)" in custom and "vec3 pal(float t)" in custom
assert "uniform vec4 uRGBA_A,uRGBA_B;" in custom
(out/"legacy_palette.frag").write_text(custom,encoding="utf-8")
print("OK GLSL output generated for original, custom palette and 18 new motors")
