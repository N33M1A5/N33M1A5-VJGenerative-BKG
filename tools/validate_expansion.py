#!/usr/bin/env python3
"""CI guard: preserve V1.0.1 core, validate VJ expansion renderer separately."""
from pathlib import Path
import re
import subprocess

stable_sha = "488f842c06fe9fa616d71b033e32c573fe90179a"
actual = subprocess.check_output(["git","hash-object","shaders.h"],text=True).strip()
if actual != stable_sha:
    raise SystemExit(f"ERROR: V1.0.1 GLSL modified! Expected {stable_sha}, got {actual}")

cpp=Path("VJGenerative.cpp").read_text(encoding="utf-8")
names='"Nevoa", "Tunel", "Lasers", "Celulas", "Aurora", "Orbes"'
assert names in cpp,"V1.0.1 original 6 motor names or order changed."
assert 'SetOptionParamInfo( P_ENGINE, "Motor", kNumEngines, 0.0f )' in cpp
assert 'params[ P_CONTRAST ]  = 0.5f;' in cpp
assert cpp.index("P_PALETTE,") < cpp.index("P_RGBA_MODE,") < cpp.index("P_SPEED,")
assert cpp.index("P_CONTRAST,") < cpp.index("P_GEOMETRY,")
assert cpp.index('SetOptionParamInfo( P_PALETTE,') < cpp.index('SetOptionParamInfo(P_RGBA_MODE,') < cpp.index('SetParamInfof( P_SPEED,')
assert 'SetParamGroup(i,"CORES | RGBA")' in cpp
assert 'if(index==P_ENGINE || index==P_PALETTE || (index>=P_SPEED && index<=P_CONTRAST))' in cpp
assert '#include "shaders.h"' in cpp
assert 'FFGLShader shader;' in cpp
assert 'shader.Compile( kVertexShader, kFragmentShader )' in cpp
assert 'expansionShader.Compile(kVJVertexShader,kVJFragmentShader)' in cpp
assert 'activeBank!=0 && expansionReady' in cpp
assert '"VJ13"' in cpp,"V1.2 must not hijack the V1.0.1 host unique plugin id"
print("PASS: Original V1.0.1 engine GLSL and six-engine selector protected.")

destination=Path("glsl-validation")
destination.mkdir(exist_ok=True)
for header,names in (
    ("shaders.h",(("kVertexShader","legacy.vert"),("kFragmentShader","legacy.frag"))),
    ("shaders_expansion.h",(("kVJVertexShader","expanded.vert"),("kVJFragmentShader","expanded.frag"))),
):
    source=Path(header).read_text(encoding="utf-8")
    for variable,filename in names:
        pattern=r"static\s+const\s+char\s*\*\s*"+variable+r'\s*=\s*R"GLSL\((.*?)\)GLSL";'
        match=re.search(pattern,source,re.S)
        assert match,f"Missing shader literal {variable}"
        (destination/filename).write_text(match.group(1),encoding="utf-8")
        print(filename,len(match.group(1)),"chars")
for i in range(18):
    assert f"col={['crystal','particles','curtains','cyber','kaleido'][i]}(" in source if i<5 else (f"if(id=={i})" in source),f"Missing engine {i}"
print("PASS: expansion dispatch covers 18 engines, GLSL extracted.")
