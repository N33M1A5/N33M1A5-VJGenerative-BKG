#!/usr/bin/env python3
"""V1.7 source/catalog sanity checks for the unified 82-engine build."""
from pathlib import Path
import re, json
root=Path(__file__).resolve().parents[1]
catalog=(root/"src/v17/engine_catalog.h").read_text(encoding="utf-8")
source=(root/"src/v17/VJGenerativeV17.cpp").read_text(encoding="utf-8")
generator=(root/"tools/make_v17_plugin.py").read_text(encoding="utf-8")
assert "kTotalEngines=82,kMotorRows=1" in catalog, "catalog must expose 82 engines and one unified control"
names=re.search(r"kMotorNames\[\]=\{(.*?)\};",catalog,re.S)
assert names, "engine names not found"
decoded=re.findall(r'"((?:\\.|[^"\\])*)"',names.group(1))
assert len(decoded)==82, f"expected 82 engines, got {len(decoded)}"
assert 'SetOptionParamInfo(P_ENGINE,"Motores",kTotalEngines,0.f)' in source, "native source lacks unified selector"
assert 'SetOptionParamInfo(P_ENGINE,"Motores",kTotalEngines,0.f)' in generator, "generator does not preserve unified selector"
assert "for(int row=0;row<kMotorRows;row++)" not in source, "legacy multi-row selector remains"
assert "for(int row=0;row<kMotorRows;row++)" not in generator, "generator still emits multi-row selector"
assert 'SetParamGroup(P_ENGINE,"MOTORES")' in source
manifest=json.loads((root/"docs/ENGINE_SOURCES.json").read_text(encoding="utf-8"))
assert len(manifest)==75, f"expected 75 source-backed engines, got {len(manifest)}"
print("PASS: 82 engine names, single native engine selector, generator consistency, source manifest.")
