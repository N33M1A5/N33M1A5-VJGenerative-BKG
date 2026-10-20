#!/usr/bin/env python3
"""Extract GLSL from header literals and compare frozen V1 against Git blob."""
from pathlib import Path
import re
import subprocess

expected_sha = "ae2caba1a82de984d715bfa8c4b192271818ed18"
actual_sha = subprocess.check_output(["git","hash-object","shaders_v1_original.h"], text=True).strip()
if actual_sha != expected_sha:
    raise SystemExit("LEGACY V1.0 MODIFIED! " + actual_sha + " != " + expected_sha)
out = Path("build/shaders")
out.mkdir(parents=True, exist_ok=True)
headers = [
    ("shaders_v1_original.h","kVertexShader","legacy.vert"),
    ("shaders_v1_original.h","kFragmentShader","legacy.frag"),
    ("shaders.h","kVertexShaderV11","addon.vert"),
    ("shaders.h","kFragmentShaderV11","addon.frag")
]
for filename, variable, output in headers:
    content = Path(filename).read_text(encoding="utf-8")
    pat = r"static\\s+const\\s+char\\s*\\*\\s*" + variable + r'\\s*=\\s*R"GLSL\\((.*?)\\)GLSL";'
    match = re.search(pat, content, flags=re.S)
    if not match:
        raise SystemExit("Cannot extract " + variable + " from " + filename)
    (out / output).write_text(match.group(1), encoding="utf-8")
    print("Extracted", output, "bytes", len(match.group(1)))
print("Legacy V1 GLSL remains EXACTLY identical to original Git blob.")
