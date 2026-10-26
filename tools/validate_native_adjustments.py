#!/usr/bin/env python3
"""Drive the actual FFGL plugin API and inspect rendered pixels for every visible adjustment."""
from pathlib import Path
import ctypes,json,sys
import moderngl,numpy as np
from PIL import Image,ImageDraw,ImageFont
ROOT=Path(__file__).resolve().parents[1]
class NativeHost:
 def __init__(self,path):
  self.ctx=moderngl.create_standalone_context(require=410,backend='egl');self.tex=self.ctx.texture((240,135),4);self.fbo=self.ctx.framebuffer([self.tex]);self.fbo.use();self.ctx.viewport=(0,0,240,135)
  self.lib=ctypes.CDLL(str(path));lib=self.lib
  lib.native_set.argtypes=[ctypes.c_uint,ctypes.c_float];lib.native_get.argtypes=[ctypes.c_uint];lib.native_get.restype=ctypes.c_float;lib.native_frame.argtypes=[ctypes.c_double]
  for name in ['native_name','native_group']:
   getattr(lib,name).argtypes=[ctypes.c_uint];getattr(lib,name).restype=ctypes.c_char_p
  lib.native_get_text.restype=ctypes.c_char_p;lib.native_text.argtypes=[ctypes.c_char_p];lib.native_mask.argtypes=[ctypes.POINTER(ctypes.c_ubyte)]
  assert lib.native_init(240,135)==1,'plugin InitGL'
  self.names={lib.native_name(i).decode():i for i in range(lib.native_count())};self.defaults={i:lib.native_get(i) for i in range(lib.native_count()) if lib.native_group(i)==b'AJUSTES' and lib.native_type(i)!=100}
  self.text('JESUS\nAÇÃO E GRAÇA')
 def set(self,name,v):assert self.lib.native_set(self.names[name],v)==1,(name,v)
 def text(self,value):
  assert self.lib.native_text(value.encode())==1;assert self.lib.native_get_text().decode()==value
  im=Image.new('L',(1024,512));d=ImageDraw.Draw(im);font=ImageFont.truetype('/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf',82);d.multiline_text((512,24),value,font=font,anchor='ma',align='center',fill=255,spacing=20)
  data=np.flipud(np.asarray(im)).copy();assert self.lib.native_mask(data.ctypes.data_as(ctypes.POINTER(ctypes.c_ubyte)))==1,'text upload did not restore pixel store'
 def reset(self,engine):
  for i,v in self.defaults.items():assert self.lib.native_set(i,v)==1
  self.set('Cores do Motor',0.)
  assert self.lib.native_select(engine)==1
 def raw(self,t):
  self.fbo.use();self.ctx.viewport=(0,0,240,135);assert self.lib.native_frame(t)==1,'ProcessOpenGL failed'
  return np.frombuffer(self.fbo.read(components=4,alignment=1),np.uint8).reshape(135,240,4).copy()
 def sample(self,t=.2):self.lib.native_clock_reset();self.raw(0.);return self.raw(t)
 def close(self):self.lib.native_close();self.fbo.release();self.tex.release();self.ctx.release()
def delta(a,b):return float(np.abs(a[:,:,:3].astype(float)-b[:,:,:3]).mean())
def main():
 host=NativeHost(sys.argv[1] if len(sys.argv)>1 else '/tmp/vj-native/vj_native_plugin.so');assert all(host.lib.native_group(i)==b'AJUSTES' for i in host.defaults), 'adjustment grouping';report={'renderer':host.ctx.info['GL_RENDERER'],'actual_ffgl_api':True,'resolume_ui_tested':False,'engines':[]};print('PARAMETERS',host.names,flush=True)
 try:
  cases=[('Velocidade',.9),('Escala',.8),('Intensidade',.85),('Matiz',.24),('Simetria',.67),('Semente',.137),('Contraste',.85),('Detalhe',.9),('Profundidade',.85),('Densidade',.9),('Direcao 0-360',.19),('Realce de Luz',.8),('Periodo do Loop',.7)]
  for engine in range(int(__import__("os").environ.get("VJ_TEST_START","0")),82):
   host.reset(engine);base=host.sample();assert base[:,:,:3].std()>.5,('black',engine)
   responses={}
   for name,value in cases:
    host.reset(engine);host.set(name,value);responses[name]=delta(base,host.sample());assert responses[name]>.003,(engine,name,'inactive',responses[name])
   host.reset(engine);host.set('Pulso',.8);pulsed=host.sample();responses['Pulso']=delta(base,pulsed);assert responses['Pulso']>.003
   host.set('BPM (60-180)',.2);responses['BPM']=delta(pulsed,host.sample());assert responses['BPM']>.003
   # Reverse retraces a known timeline; Speed=0 then holds exactly that state.
   host.reset(engine);host.lib.native_clock_reset();host.raw(0.);a=host.raw(.1);b=host.raw(.2);host.set('Inverter Movimento',1.);back=host.raw(.3)
   assert delta(a,back)<.15,(engine,'reverse does not retrace',delta(a,back));host.set('Velocidade',0.);paused=host.raw(.4);assert delta(back,paused)<.15,(engine,'pause')
   responses['Inverter Movimento']='timeline retraced';responses['Velocidade zero']='paused'
   host.reset(engine);host.set('Opacidade',.25);alpha=host.sample()[:,:,3];assert abs(float(alpha.mean())-64)<1
   # Test palettes and every RGBA channel via public setters, including A-only alpha.
   host.reset(engine);host.set('Paleta',10.);palette=host.sample();assert delta(base,palette)>.01
   host.set('Modo de Cor',1.);host.set('A Alpha',.5);host.set('Opacidade',.5);a=host.sample();assert abs(float(a[:,:,3].mean())-64)<1,(engine,'A alpha')
   # Hue is already validated above against each engine's rendered output.
   # In RGBA override modes, the selected A/B colors replace engine colorization,
   # so hue is not expected to shift those explicit user-selected colors.
   host.set('Modo de Cor',3.);host.set('B Alpha',1.);host.set('Mix Cor A/B',.5)
   for name in ['A Red','A Green','A Blue']:
    before=host.sample();original=host.lib.native_get(host.names[name]);host.set(name,1.-original);assert delta(before,host.sample())>.003,(engine,name);host.set(name,original)
   host.set('Modo de Cor',2.);host.set('Mix Cor A/B',.5)
   for name in ['B Red','B Green','B Blue']:
    before=host.sample();original=host.lib.native_get(host.names[name]);host.set(name,1.-original);assert delta(before,host.sample())>.003,(engine,name);host.set(name,original)
   before=host.sample();host.set('B Alpha',.1);assert float(host.sample()[:,:,3].mean())<float(before[:,:,3].mean())-10,(engine,'B alpha')
   before=host.sample();host.set('Mix Cor A/B',.1);assert delta(before,host.sample())>.003,(engine,'mix')
   for op in [0,1,2,3]:host.set('Mistura Cores',float(op));assert host.sample()[:,:,:3].std()>.1
   host.reset(engine)
   for canvas in [1.,2.,3.]:
    host.set('Canvas',canvas);image=host.sample()
    if canvas==1.:assert image[:,:,:3].std()>.5,(engine,'16:9 canvas produced invalid output')
    else:assert delta(base,image)>.003,(engine,'canvas option',canvas)
   host.set('Canvas',4.);before=host.sample();host.set('Largura Custom',.45);assert delta(before,host.sample())>.003,(engine,'custom width');before=host.sample();host.set('Altura Custom',.4);assert delta(before,host.sample())>.003,(engine,'custom height')
   # Qualidade and reference colors only apply to the 75 multipass engines.
   if engine>=7:
    assert host.lib.native_visible(host.names['Qualidade'])==1
    host.reset(engine);host.set('Qualidade',0.);low=host.sample();host.set('Qualidade',2.);high=host.sample();assert delta(low,high)>.003,(engine,'quality')
    host.set('Cores do Motor',1.);assert delta(high,host.sample())>.003,(engine,'color source')
   else:assert host.lib.native_visible(host.names['Qualidade'])==0
   report['engines'].append({'id':engine+1,'adjustments':responses,'rgba_channels_tested':8,'blend_operations_tested':4,'canvas_custom_tested':True});print('PASS adjustments',engine+1,flush=True)
  host.reset(21)
  for name,value in [('Tamanho Texto',.9),('Texto X',.7),('Texto Y',.7),('Layout Texto',0.)]:
   host.reset(21);before=host.sample();host.set(name,value);assert delta(before,host.sample())>.003,('text adjustment',name)
  before=host.sample();host.text('CRIAÇÃO\nVITÓRIA');assert delta(before,host.sample())>.003,'text editing'
  report['text_editable_utf8']=True;report['single_adjustment_group']=True;report['text_pixel_store_restored']=True
  (ROOT/'previews/v17/native_adjustments.json').write_text(json.dumps(report,ensure_ascii=False,indent=2)+'\n')
 finally:host.close()
if __name__=='__main__':main()
