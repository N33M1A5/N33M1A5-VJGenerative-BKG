#!/usr/bin/env python3
"""Render actual GLSL 410 prototypes using EGL; no reference textures required.

pip install -r prototypes/gate_a/requirements.txt
python tools/render_gate_a.py --video
"""
from pathlib import Path
import argparse, io, json, subprocess, time as clock
import moderngl
import numpy as np
from PIL import Image, ImageDraw, ImageFont

ROOT=Path(__file__).resolve().parents[1]
SRC=ROOT/'prototypes/gate_a'
ENGINES={'stained_glass':'Stained Glass Sacred Bubble Gum','spotlight':'Spotlight 1','flow_dark_glass':'Flow Dark Glass'}
DEFAULTS=dict(time=0.,speed=1.,scale=.5,intensity=.5,hue=0.,symmetry=0.,seed=0.,bpm=120.,pulse=0.,detail=.5,depth=.5,density=.5,glow=.35,opacity=1.,period=20.,direction=1.)

class Renderer:
 def __init__(self,w,h):
  self.w,self.h=w,h
  self.ctx=moderngl.create_standalone_context(require=410,backend='egl')
  self.programs={};self.vaos={};self.textures=[];self.fbos=[]
  vertex=(SRC/'fullscreen.vert').read_text()
  common=(SRC/'common.glsl').read_text()
  for name in ENGINES:
   source='#version 410 core\nin vec2 uv;\nout vec4 fragColor;\n'+common+(SRC/(name+'.frag')).read_text()+'\nvoid main(){fragColor=vec4(scene(),1.);}\n'
   self.programs[name]=self.ctx.program(vertex_shader=vertex,fragment_shader=source)
  for name in ['bloom','composite']:
   self.programs[name]=self.ctx.program(vertex_shader=vertex,fragment_shader=(SRC/(name+'.frag')).read_text())
  for name,p in self.programs.items(): self.vaos[name]=self.ctx.vertex_array(p,[])
  for i in range(4):
   tex=self.ctx.texture((w,h),4,dtype='f2' if i<3 else 'f1')
   tex.filter=(moderngl.LINEAR,moderngl.LINEAR);tex.repeat_x=False;tex.repeat_y=False
   self.textures.append(tex);self.fbos.append(self.ctx.framebuffer([tex]))
 def set(self,name,values):
  p=self.programs[name]
  for k,v in values.items():
   if k in p:p[k].value=v
 def draw(self,name,target):
  self.fbos[target].use();self.ctx.viewport=(0,0,self.w,self.h)
  self.vaos[name].render(mode=moderngl.TRIANGLES,vertices=3)
 def frame(self,name,t,**overrides):
  params={**DEFAULTS,'time':float(t),**overrides}
  self.set(name,{**params,'resolution':(float(self.w),float(self.h))})
  self.draw(name,0)
  self.textures[0].use(0);self.set('bloom',dict(source=0,axis=(3./self.w,0.)))
  self.draw('bloom',1)
  self.textures[1].use(0);self.set('bloom',dict(source=0,axis=(0.,3./self.h)))
  self.draw('bloom',2)
  self.textures[0].use(0);self.textures[2].use(1)
  self.set('composite',dict(source=0,bloom=1,glow=params['glow'],opacity=params['opacity']))
  self.draw('composite',3)
  data=self.fbos[3].read(components=4,alignment=1)
  return np.frombuffer(data,np.uint8).reshape(self.h,self.w,4)[::-1].copy()
 def release(self):
  for v in self.vaos.values():v.release()
  for p in self.programs.values():p.release()
  for f in self.fbos:f.release()
  for t in self.textures:t.release()
  self.ctx.release()

def diff(a,b):return float(np.abs(a[:,:,:3].astype(float)-b[:,:,:3]).mean())
def save_png(image,path):
 data=io.BytesIO();image.save(data,format='PNG');path.write_bytes(data.getvalue())
 with Image.open(path) as saved:saved.load()
def validate(out):
 r=Renderer(640,360);report={'renderer':r.ctx.info['GL_RENDERER'],'gl_version':r.ctx.info['GL_VERSION'],'engines':{},'claim':'OpenGL rendering validation; not an RTX/Resolume performance certification.'}
 images=[]
 for name,title in ENGINES.items():
  a=r.frame(name,0);b=r.frame(name,5);c=r.frame(name,10)
  assert a[:,:,:3].std()>8,(name,'flat or black')
  motion=diff(a,b);assert motion>1,(name,'static')
  endpoint=diff(a,r.frame(name,20));assert endpoint<.15,(name,'loop endpoint')
  wrap=diff(r.frame(name,20.-1./30.),a)
  adjacent=diff(a,r.frame(name,1./30.))
  assert wrap<max(2.,adjacent*3.),(name,'loop jump')
  assert diff(a,r.frame(name,5,speed=0))==0,(name,'pause')
  alpha=r.frame(name,0,opacity=.25)[:,:,3];assert abs(float(alpha.mean())-64)<1
  responses={}
  for control,val in dict(scale=.8,intensity=.9,hue=.2,symmetry=.7,seed=27.,detail=.9,depth=.9,density=.9,glow=.9,speed=.6,direction=-1.).items():
   responses[control]=diff(b,r.frame(name,5,**{control:val}))
   assert responses[control]>.01,(name,control,'inactive')
  responses['pulse']=diff(r.frame(name,.21),r.frame(name,.21,pulse=.8))
  responses['bpm_with_pulse']=diff(r.frame(name,.21,pulse=.8),r.frame(name,.21,pulse=.8,bpm=85.))
  assert responses['pulse']>.01 and responses['bpm_with_pulse']>.01
  for t,img in [(0,a),(5,b),(10,c)]:save_png(Image.fromarray(img),out/f'{name}_{t:02d}.png')
  images.append(b)
  report['engines'][name]={'motion_mean_delta_5s':motion,'loop_endpoint_delta':endpoint,'wrap_delta':wrap,'adjacent_delta':adjacent,'controls':responses,'alpha_25_percent':float(alpha.mean())}
 for i in range(3):
  for j in range(i):assert diff(images[i],images[j])>10,'Indistinct engines'
 r.release()
 for w,h in [(1920,1080),(3360,1008),(3840,2160),(1080,1920)]:
  r=Renderer(w,h)
  for name in ENGINES:
   start=clock.perf_counter();img=r.frame(name,5)
   assert img[:,:,:3].std()>8
   report['engines'][name][f'{w}x{h}']={'render_and_readback_seconds':round(clock.perf_counter()-start,3),'nonblack':True}
   if w==3360:save_png(Image.fromarray(img),out/f'{name}_LED_3360x1008.png')
  r.release()
 (out/'validation.json').write_text(json.dumps(report,indent=2)+'\n')
 return report

def previews(out):
 r=Renderer(960,540)
 for name in ENGINES:
  command=['ffmpeg','-y','-loglevel','error','-f','rawvideo','-pixel_format','rgba','-video_size','960x540','-framerate','30','-i','-','-an','-c:v','libx264','-preset','fast','-crf','19','-pix_fmt','yuv420p','-movflags','+faststart',str(out/f'{name}_20s.mp4')]
  proc=subprocess.Popen(command,stdin=subprocess.PIPE)
  try:
   for f in range(600):proc.stdin.write(r.frame(name,f/30.).tobytes())
  finally:proc.stdin.close()
  if proc.wait():raise RuntimeError('ffmpeg failed')
  print('Video ready:',name,flush=True)
 r.release()

def contact(out):
 fontpath='/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf'
 try:font=ImageFont.truetype(fontpath,21);small=ImageFont.truetype(fontpath,14)
 except OSError:font=small=ImageFont.load_default()
 canvas=Image.new('RGB',(1280,1190),'#101016');d=ImageDraw.Draw(canvas)
 d.text((28,20),'VJ GENERATIVE  /  V1.6  /  PROVAS VISUAIS',font=font,fill='#e7e7f0')
 d.text((28,55),'Renderizações dos shaders · protótipos para avaliação · integração V1.6 LAB',font=small,fill='#a0a0b0')
 for row,(name,title) in enumerate(ENGINES.items()):
  y=100+row*360
  d.text((28,y),title,font=font,fill='white')
  for col,t in enumerate([0,5,10]):
   im=Image.open(out/f'{name}_{t:02d}.png').convert('RGB').resize((400,225))
   canvas.paste(im,(28+col*418,y+40));d.text((28+col*418,y+274),f'{t:02d} s',font=small,fill='#a0a0b0')
 save_png(canvas,out/'VJ_GENERATIVE_Gate_A.png')

if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('--video',action='store_true');p.add_argument('--out',default='previews/gate_a');args=p.parse_args()
 out=ROOT/args.out;out.mkdir(parents=True,exist_ok=True)
 report=validate(out);contact(out);print(json.dumps(report,indent=2),flush=True)
 if args.video:previews(out)
