#!/usr/bin/env python3
"""Render all 75 expansion engines through the actual C++ pipeline in an EGL host."""
from pathlib import Path
import argparse,ctypes,json,subprocess,time,hashlib,io,tempfile
import moderngl,numpy as np
from PIL import Image,ImageDraw,ImageFont
ROOT=Path(__file__).resolve().parents[1]
REFS=json.loads((ROOT/'docs/REFERENCIAS_STORYLOOP.json').read_text())['references']
DEFAULT=[.5,.5,0.,0.,0.,0.,.5,.5,.5,.35,1.,-1.,0.,.5,2.,.5,20.,.5,0.,0.,1.,120.]
class Host:
 def __init__(self,library):
  self.ctx=moderngl.create_standalone_context(require=410,backend='egl');self.lib=ctypes.CDLL(str(library));self.lib.catalog_render.argtypes=[ctypes.c_int,ctypes.c_float,ctypes.POINTER(ctypes.c_float)];self.lib.catalog_mask.argtypes=[ctypes.POINTER(ctypes.c_ubyte)];self.size=None
  assert self.lib.catalog_init()==1,'pipeline initialization'
  self.mask('JESUS\nREI DOS REIS\nAÇÃO E GRAÇA')
 def mask(self,text):
  im=Image.new('L',(1024,512));d=ImageDraw.Draw(im);font=ImageFont.truetype('/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf',82)
  d.multiline_text((512,24),text,font=font,anchor='ma',align='center',fill=255,spacing=20)
  pixels=np.flipud(np.asarray(im)).copy();self.lib.catalog_mask(pixels.ctypes.data_as(ctypes.POINTER(ctypes.c_ubyte)))
 def frame(self,e,t,size=(320,180),changes=None):
  if self.size!=size:
   if self.size:self.fbo.release();self.tex.release()
   self.tex=self.ctx.texture(size,4);self.fbo=self.ctx.framebuffer([self.tex]);self.size=size
  self.fbo.use();self.ctx.viewport=(0,0,*size);values=DEFAULT.copy()
  if changes:
   for i,v in changes.items():values[i]=v
  a=(ctypes.c_float*len(values))(*values)
  result=self.lib.catalog_render(e,float(t),a);assert result==1,(e,'C++ render/state',result)
  return np.frombuffer(self.fbo.read(components=4,alignment=1),np.uint8).reshape(size[1],size[0],4)[::-1].copy()
 def close(self):
  self.lib.catalog_release()
  if self.size:self.fbo.release();self.tex.release()
  self.ctx.release()
def delta(a,b):return float(np.abs(a[:,:,:3].astype(float)-b[:,:,:3]).mean())
def png(im,path):
 b=io.BytesIO();im.save(b,format='PNG');path.write_bytes(b.getvalue())
def sheets(out):
 font=ImageFont.truetype('/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf',13)
 for page in range(4):
  canvas=Image.new('RGB',(1280,790),'#111118');d=ImageDraw.Draw(canvas);d.text((14,10),f'VJ GENERATIVE / 82 MOTORES / EXPANSÃO {page+1}/4 — RENDERS REAIS',font=font,fill='white')
  for i,r in enumerate(REFS[page*20:(page+1)*20]):
   x=(i%5)*256;y=(i//5)*185+40;path=out/f"{r['engine_id']:02d}_05.png"
   if path.exists():canvas.paste(Image.open(path).convert('RGB').resize((250,141)),(x+3,y))
   d.text((x+5,y+145),f"{r['engine_id']:02d} {r['name']}"[:32],font=font,fill='#dddded')
  png(canvas,out/f'catalogo_{page+1}.png')
def validate(host,out,full=True):
 report={'renderer':host.ctx.info['GL_RENDERER'],'pipeline':'actual C++/EGL','resolume_tested':False,'expansion_engines':75,'total_engines':82,'engines':[]};signatures=[]
 for e,r in enumerate(REFS):
  start=time.perf_counter();a=host.frame(e,0);b=host.frame(e,5);c=host.frame(e,10)
  assert a[:,:,:3].std()>.8,(r['name'],'flat/black')
  motion=delta(a,b);assert motion>.015,(r['name'],'no motion')
  loop=delta(a,host.frame(e,20));assert loop<.7,(r['name'],'loop endpoint',loop)
  responses={}
  for name,index,value in [('scale',0,.8),('intensity',1,.9),('hue',2,.25),('symmetry',3,.7)]:
   responses[name]=delta(b,host.frame(e,5,changes={index:value}))
  responses['pulse']=delta(host.frame(e,.21),host.frame(e,.21,changes={5:.8}))
  responses['bpm']=delta(host.frame(e,.21,changes={5:.8}),host.frame(e,.21,changes={5:.8,21:85.}))
  if full:
   for name,d in responses.items():assert d>.005,(r['name'],name,'inactive',d)
   pals=set()
   for palette in range(36):pals.add(hashlib.sha256(host.frame(e,5,size=(160,90),changes={11:palette}).tobytes()).hexdigest())
   assert len(pals)==36,(r['name'],'palette collision')
   for mode in [1,2,3]:
    rgba=host.frame(e,5,size=(160,90),changes={12:mode,10:.5});alpha=float(rgba[:,:,3].mean());assert abs(alpha-(64 if mode==1 else 96))<1,(r['name'],'RGBA alpha',mode,alpha)
   # Actual viewport shape changes, not just an image resize.
   for size in [(640,192),(180,320)]:assert host.frame(e,5,size=size)[:,:,:3].std()>.8
  for sec,im in [(0,a),(5,b),(10,c)]:png(Image.fromarray(im),out/f"{r['engine_id']:02d}_{sec:02d}.png")
  signatures.append(hashlib.sha256(b.tobytes()).hexdigest())
  report['engines'].append({'id':r['engine_id'],'name':r['name'],'motion_delta':motion,'loop_endpoint_delta':loop,'responses':responses,'palettes_checked':36 if full else 0,'rgba_modes_checked':3 if full else 0,'viewport_shapes_checked':3 if full else 1,'seconds':round(time.perf_counter()-start,3)})
  print('Validated',r['engine_id'],r['name'],round(motion,3),flush=True)
 assert len(set(signatures))==75,'Duplicate outputs'
 (out/'validation.json').write_text(json.dumps(report,ensure_ascii=False,indent=2)+'\n');sheets(out)
 return report
def movies(host,out,seconds=12,fps=20,selected=None):
 for e,r in enumerate(REFS):
  if selected and r['engine_id'] not in selected:continue
  path=out/f"{r['engine_id']:02d}_preview.mp4"
  temporary=Path(tempfile.gettempdir())/f"vj_catalog_{r['engine_id']:02d}.mp4"
  command=['ffmpeg','-y','-loglevel','error','-f','rawvideo','-pixel_format','rgba','-video_size','640x360','-framerate',str(fps),'-i','-','-an','-c:v','libx264','-preset','fast','-crf','19','-pix_fmt','yuv420p','-movflags','+faststart',str(temporary)]
  proc=subprocess.Popen(command,stdin=subprocess.PIPE)
  try:
   for f in range(int(seconds*fps)):proc.stdin.write(host.frame(e,f/fps,size=(640,360)).tobytes())
  finally:proc.stdin.close()
  assert proc.wait()==0;path.write_bytes(temporary.read_bytes());temporary.unlink();print('Preview ready',r['engine_id'],flush=True)
if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('--library',default='/tmp/vj_catalog_pipeline.so');p.add_argument('--out',default='previews/v17');p.add_argument('--quick',action='store_true');p.add_argument('--video',action='store_true');p.add_argument('--movies-only',action='store_true');p.add_argument('--seconds',type=int,default=12);p.add_argument('--ids');args=p.parse_args();out=ROOT/args.out;out.mkdir(exist_ok=True,parents=True)
 host=Host(args.library)
 try:
  if not args.movies_only:validate(host,out,not args.quick)
  if args.video or args.movies_only:movies(host,out,args.seconds,selected=set(map(int,args.ids.split(','))) if args.ids else None)
 finally:host.close()
