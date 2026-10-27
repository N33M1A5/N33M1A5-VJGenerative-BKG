#!/usr/bin/env python3
"""Exercise the actual C++ multipass renderer inside an EGL test host."""
import ctypes,json,sys
from pathlib import Path
import moderngl,numpy as np
ctx=moderngl.create_standalone_context(require=410,backend='egl')
lib=ctypes.CDLL(sys.argv[1] if len(sys.argv)>1 else '/tmp/vj_story_pipeline.so')
lib.story_render.argtypes=[ctypes.c_int,ctypes.c_float,ctypes.c_int,ctypes.c_int,ctypes.c_float]
assert lib.story_init()==1,'C++ shader init failed'
report={'renderer':ctx.info['GL_RENDERER'],'pipeline':'actual C++ StoryPipeline, EGL test host','resolume_tested':False,'engines':{}}
try:
 for size in [(320,180),(640,192),(180,320)]:
  tex=ctx.texture(size,4);fbo=ctx.framebuffer([tex]);fbo.use();ctx.viewport=(0,0,*size)
  for engine in range(3):
   assert lib.story_render(engine,2.,0,-1,1.)==1,('state or render',size,engine)
   rgb=np.frombuffer(fbo.read(components=4,alignment=1),dtype=np.uint8).reshape(*size[::-1],4)
   assert rgb[:,:,:3].std()>8,('black frame',size,engine)
   assert rgb[:,:,3].min()==255
   report['engines'].setdefault(str(engine),{})[str(size)]='nonblack; GL state restored'
  fbo.release();tex.release()
 tex=ctx.texture((256,144),4);fbo=ctx.framebuffer([tex]);fbo.use();ctx.viewport=(0,0,256,144)
 for engine in range(3):
  signatures=set()
  for palette in range(36):
   assert lib.story_render(engine,2.,0,palette,1.)==1
   pixels=fbo.read(components=4,alignment=1);signatures.add(pixels)
  assert len(signatures)==36,('palette collision',engine)
  for mode in range(1,4):
   assert lib.story_render(engine,2.,mode,-1,.5)==1
   rgba=np.frombuffer(fbo.read(components=4,alignment=1),np.uint8).reshape(144,256,4)
   expected=64 if mode==1 else 96
   assert abs(float(rgba[:,:,3].mean())-expected)<1,('RGBA alpha',mode)
  report['engines'][str(engine)]['palettes']=36
  report['engines'][str(engine)]['rgba_modes']=3
finally:
 lib.story_release();ctx.release()
Path('previews/gate_a/cpp_pipeline_validation.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(report,indent=2))
