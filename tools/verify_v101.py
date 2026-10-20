#!/usr/bin/env python3
"""V1.0.1 release gate: the five non-Tunnel engines and all FFGL controls are unchanged."""
from pathlib import Path
import re
import subprocess

v100 = "458999dd09ca10acb76092af73a9515627197b22"

def original(path):
    return subprocess.check_output(["git", "show", f"{v100}:{path}"]).decode("utf-8")

baseline = original("shaders.h")
current = Path("shaders.h").read_text(encoding="utf-8")
old = """// 1 - Tunel neon
vec3 eTunnel(vec2 p,float t){
  float d=length(p)+1e-3, a=atan(p.y,p.x);
  float z=.6/d+t*1.5;
  vec2 g=abs(fract(vec2(a*1.2732+uSeed,z))-.5);
  float l=min(g.x,g.y);
  float glow=.025/(l+.025);
  return pal(z*.08+a*.05+t*.03)*glow*smoothstep(0.,.6,d)*.9;
}"""
new = """// 1 - Tunel neon (V1.0.1: correcao de costura angular)
vec3 eTunnel(vec2 p,float t){
  const float PI=3.14159265358979323846;
  const float TAU=6.28318530717958647692;
  float d=length(p)+1e-3, a=atan(p.y,p.x);
  float ang=fract((a+PI)/TAU);
  float z=.6/d+t*1.5;
  vec2 g=abs(fract(vec2(ang*8.0+uSeed,z))-.5);
  float l=min(g.x,g.y);
  float glow=.025/(l+.025);
  return pal(z*.08+sin(a)*.05+t*.03)*glow*smoothstep(0.,.6,d)*.9;
}"""
assert baseline.count(old) == 1, "V1.0 baseline mismatch"
assert current == baseline.replace(old, new), "V1.0.1 shader changed outside eTunnel()"

orig_cpp = original("VJGenerative.cpp")
cpp = Path("VJGenerative.cpp").read_text(encoding="utf-8")
metadata = [
    ('"VJGN"', '"V101"'),
    ('"VJ Generative"', '"VJ Gen V1.0.1"'),
    ('\t1, 0,\n\tFF_SOURCE,', '\t1, 1,\n\tFF_SOURCE,'),
    ('"Plugin FFGL generativo - 6 motores, 9 paletas"', '"V1.0.1: seis motores originais; Tunnel angular seam corrected"')
]
for a, b in metadata:
    assert orig_cpp.count(a) == 1, f"Missing original C++ marker: {a}"
    orig_cpp = orig_cpp.replace(a, b)
assert cpp == orig_cpp, "V1.0.1 changed original C++ behavior outside metadata"

dest = Path("glsl-validation")
dest.mkdir(exist_ok=True)
for kind, label in [("kVertexShader", "vert"), ("kFragmentShader", "frag")]:
    regex = r'static\s+const\s+char\s*\*\s*' + kind + r'\s*=\s*R"GLSL\((.*?)\)GLSL";'
    match = re.search(regex, current, re.S)
    assert match, f"Missing {kind}"
    (dest / f"v101.{label}").write_text(match.group(1), encoding="utf-8")
print("PASS: 5 legacy engines and all 11 controls unchanged; only tunnel GLSL and version metadata modified.")
