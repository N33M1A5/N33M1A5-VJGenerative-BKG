#!/usr/bin/env python3
from pathlib import Path
import argparse,subprocess,xml.etree.ElementTree as ET
ROOT=Path(__file__).resolve().parents[1]
p=argparse.ArgumentParser();p.add_argument('--sdk',required=True);p.add_argument('--out',default='/tmp');args=p.parse_args();sdk=Path(args.sdk).resolve();out=Path(args.out);out.mkdir(exist_ok=True,parents=True)
ns={'m':'http://schemas.microsoft.com/developer/msbuild/2003'};tree=ET.parse(ROOT/'build/windows/VJGenerativeV1.vcxproj')
sources=[(sdk/'build/windows'/x.attrib['Include'].replace('\\','/')).resolve() for x in tree.findall('.//m:ClCompile[@Include]',ns) if 'plugins' not in x.attrib['Include']]
flags=['-std=c++17','-O1','-DGLEW_NO_GLU','-I'+str(ROOT),'-I'+str(sdk/'source/lib'),'-I'+str(sdk/'deps/glew-2.1.0/include')]
libs=['-l:libGLEW.so.2.2','-l:libGL.so.1']
# Compile SDK once and reuse the objects for the actual FFGL interface test and render host.
objects=[]
for source in sources:
 obj=out/(source.stem+'.o');objects.append(str(obj))
 if not obj.exists() or source.stat().st_mtime>obj.stat().st_mtime:subprocess.run(['g++',*flags,'-fPIC','-c',str(source),'-o',str(obj)],check=True)
subprocess.run(['g++',*flags,str(ROOT/'tools/plugin_contract_smoke.cpp'),*objects,*libs,'-o',str(out/'vj_plugin_contract')],check=True)
subprocess.run([str(out/'vj_plugin_contract')],check=True)
subprocess.run(['g++',*flags,'-shared','-fPIC',str(ROOT/'tools/native_plugin_bridge.cpp'),*objects,*libs,'-o',str(out/'vj_native_plugin.so')],check=True)
