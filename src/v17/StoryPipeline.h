#pragma once
#include "FFGLSDK.h"
#include "generated_shaders.h"
#include "engine_catalog.h"
#include "TextAtlas.h"
#include <cstdint>
#include <algorithm>

// All state touched by the multipass renderer belongs to the host on entry.
struct StoryGLState {
 GLint drawFBO,readFBO,viewport[4],program,vao,active,unpack,unpackAlignment,unpackRowLength,unpackSkipRows,unpackSkipPixels,tex[3],sampler[3];
 GLboolean blend,depth,scissor,cull,srgb,mask[4];
 StoryGLState(){
  glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING,&drawFBO);glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING,&readFBO);
  glGetIntegerv(GL_VIEWPORT,viewport);glGetIntegerv(GL_CURRENT_PROGRAM,&program);glGetIntegerv(GL_VERTEX_ARRAY_BINDING,&vao);
  glGetIntegerv(GL_ACTIVE_TEXTURE,&active);glGetIntegerv(GL_PIXEL_UNPACK_BUFFER_BINDING,&unpack);glGetIntegerv(GL_UNPACK_ALIGNMENT,&unpackAlignment);glGetIntegerv(GL_UNPACK_ROW_LENGTH,&unpackRowLength);glGetIntegerv(GL_UNPACK_SKIP_ROWS,&unpackSkipRows);glGetIntegerv(GL_UNPACK_SKIP_PIXELS,&unpackSkipPixels);
  for(int i=0;i<3;i++){glActiveTexture(GL_TEXTURE0+i);glGetIntegerv(GL_TEXTURE_BINDING_2D,&tex[i]);glGetIntegeri_v(GL_SAMPLER_BINDING,i,&sampler[i]);}
  blend=glIsEnabled(GL_BLEND);depth=glIsEnabled(GL_DEPTH_TEST);scissor=glIsEnabled(GL_SCISSOR_TEST);cull=glIsEnabled(GL_CULL_FACE);srgb=glIsEnabled(GL_FRAMEBUFFER_SRGB);glGetBooleanv(GL_COLOR_WRITEMASK,mask);
 }
 static void toggle(GLenum cap,GLboolean on){if(on)glEnable(cap);else glDisable(cap);}
 ~StoryGLState(){
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER,drawFBO);glBindFramebuffer(GL_READ_FRAMEBUFFER,readFBO);glViewport(viewport[0],viewport[1],viewport[2],viewport[3]);
  glUseProgram(program);glBindVertexArray(vao);glBindBuffer(GL_PIXEL_UNPACK_BUFFER,unpack);glPixelStorei(GL_UNPACK_ALIGNMENT,unpackAlignment);glPixelStorei(GL_UNPACK_ROW_LENGTH,unpackRowLength);glPixelStorei(GL_UNPACK_SKIP_ROWS,unpackSkipRows);glPixelStorei(GL_UNPACK_SKIP_PIXELS,unpackSkipPixels);
  for(int i=0;i<3;i++){glActiveTexture(GL_TEXTURE0+i);glBindTexture(GL_TEXTURE_2D,tex[i]);glBindSampler(i,sampler[i]);}glActiveTexture(active);
  toggle(GL_BLEND,blend);toggle(GL_DEPTH_TEST,depth);toggle(GL_SCISSOR_TEST,scissor);toggle(GL_CULL_FACE,cull);toggle(GL_FRAMEBUFFER_SRGB,srgb);glColorMask(mask[0],mask[1],mask[2],mask[3]);
 }
};
struct StoryControls {
 float time,beat,scale,intensity,hue,symmetry,seed,pulse,detail,depth,density,glow,opacity,period,contrast;
 float width,height,quality,colorSource,palette,colorMode,colorBlend,colorMix;
 float a[4],b[4];
 float orientation=0.f;
 float textSize=.5f,textX=0.f,textY=0.f,textLayout=1.f;
 const char* text=nullptr;
};
class StoryPipeline {
 struct Slot{ffglex::FFGLShader shader;int engine=-1;std::uint64_t used=0;};
 Slot cache[4];ffglex::FFGLShader blur,composite;
 std::uint64_t clock=0;bool failed[kStoryCount]={};
 GLuint textTexture=0;bool externalText=false;std::string lastText;
 GLuint fbo[3]={},texture[3]={},vao=0;
 int width=0,height=0;bool ready=false;
 void freeTargets(){glDeleteFramebuffers(3,fbo);glDeleteTextures(3,texture);for(int i=0;i<3;i++){fbo[i]=texture[i]=0;}width=height=0;}
 bool ensure(int w,int h){
  if(width==w&&height==h)return true;
  freeTargets();glBindBuffer(GL_PIXEL_UNPACK_BUFFER,0);glGenFramebuffers(3,fbo);glGenTextures(3,texture);
  glActiveTexture(GL_TEXTURE0);
  for(int i=0;i<3;i++){
   glBindTexture(GL_TEXTURE_2D,texture[i]);glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
   glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA16F,w,h,0,GL_RGBA,GL_HALF_FLOAT,nullptr);
   glBindFramebuffer(GL_FRAMEBUFFER,fbo[i]);glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,texture[i],0);
   glDrawBuffer(GL_COLOR_ATTACHMENT0);
   if(glCheckFramebufferStatus(GL_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE){freeTargets();return false;}
  }
  width=w;height=h;return true;
 }
 ffglex::FFGLShader* sceneFor(int engine){
  if(failed[engine])return nullptr;
  for(auto& slot:cache)if(slot.engine==engine){slot.used=++clock;return &slot.shader;}
  Slot* slot=&cache[0];for(auto& item:cache)if(item.engine<0||item.used<slot->used)slot=&item;
  slot->shader.FreeGLResources();slot->engine=-1;
  if(!slot->shader.Compile(kStoryVertex,kStorySources[engine])){slot->shader.FreeGLResources();failed[engine]=true;FFGLLog::LogToHost(kMotorNames[engine+7]);return nullptr;}
  slot->engine=engine;slot->used=++clock;return &slot->shader;
 }
 bool uploadText(const unsigned char* pixels){
  if(!textTexture)glGenTextures(1,&textTexture);
  glActiveTexture(GL_TEXTURE2);glBindBuffer(GL_PIXEL_UNPACK_BUFFER,0);glBindTexture(GL_TEXTURE_2D,textTexture);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
  glPixelStorei(GL_UNPACK_ALIGNMENT,1);glPixelStorei(GL_UNPACK_ROW_LENGTH,0);glPixelStorei(GL_UNPACK_SKIP_ROWS,0);glPixelStorei(GL_UNPACK_SKIP_PIXELS,0);
  glTexImage2D(GL_TEXTURE_2D,0,GL_R8,TextAtlas::W,TextAtlas::H,0,GL_RED,GL_UNSIGNED_BYTE,pixels);return true;
 }
 void draw(){glDrawArrays(GL_TRIANGLES,0,3);}
public:
 bool init(){
  StoryGLState state;
  ready=blur.Compile(kStoryVertex,kStoryBloom)&&composite.Compile(kStoryVertex,kStoryComposite);
  if(ready)glGenVertexArrays(1,&vao);return ready;
 }
 bool isReady()const{return ready;}
 void renderSize(int& w,int& h)const{w=width;h=height;}
 void release(){for(auto& slot:cache){slot.shader.FreeGLResources();slot.engine=-1;slot.used=0;}blur.FreeGLResources();composite.FreeGLResources();freeTargets();if(vao)glDeleteVertexArrays(1,&vao);if(textTexture)glDeleteTextures(1,&textTexture);textTexture=0;lastText.clear();externalText=false;vao=0;ready=false;clock=0;std::fill_n(failed,kStoryCount,false);}
 void setTestTextMask(const unsigned char* pixels){StoryGLState state;uploadText(pixels);externalText=true;}
 bool render(int engine,const StoryControls& c){
  if(!ready||engine<0||engine>=kStoryCount)return false;
  StoryGLState state;
  auto* selected=sceneFor(engine);if(!selected)return false;
  if(engine==14&&!externalText){
   std::string text=c.text?c.text:"";
   if(text!=lastText||!textTexture){TextAtlas atlas;if(!atlas.build(text))return false;uploadText(atlas.pixels.data());lastText=text;}
  }
  float quality=c.quality<.5f?.5f:(c.quality<1.5f?.75f:1.f);
  GLint limit=0;glGetIntegerv(GL_MAX_TEXTURE_SIZE,&limit);
  int w=std::max(1,(int)(state.viewport[2]*quality)),h=std::max(1,(int)(state.viewport[3]*quality));
  float fit=std::min(1.f,(float)limit/(float)std::max(w,h));w=std::max(1,(int)(w*fit));h=std::max(1,(int)(h*fit));
  if(!ensure(w,h))return false;
  glDisable(GL_BLEND);glDisable(GL_DEPTH_TEST);glDisable(GL_SCISSOR_TEST);glDisable(GL_CULL_FACE);glDisable(GL_FRAMEBUFFER_SRGB);glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
  glBindVertexArray(vao);glBindSampler(0,0);glBindSampler(1,0);glBindSampler(2,0);glViewport(0,0,w,h);
  auto& scene=*selected;glUseProgram(scene.GetGLID());glBindFramebuffer(GL_FRAMEBUFFER,fbo[0]);
  scene.Set("resolution",c.width,c.height);scene.Set("time",c.time);scene.Set("speed",1.f);scene.Set("direction",1.f);
  scene.Set("orientation",c.orientation);scene.Set("scale",c.scale);scene.Set("intensity",c.intensity);scene.Set("hue",c.colorSource<.5f&&c.colorMode<.5f?c.hue:0.f);
  scene.Set("symmetry",c.symmetry);scene.Set("seed",c.seed);scene.Set("pulse",0.f);scene.Set("bpm",120.f);
  scene.Set("detail",c.detail);scene.Set("depth",c.depth);scene.Set("density",c.density);scene.Set("period",c.period);
  scene.Set("uHostBeat",c.beat);scene.Set("uHostPulse",c.pulse);scene.Set("uPalette",c.palette);scene.Set("uHue",c.hue);scene.Set("uColorSource",c.colorSource);
  scene.Set("uRGBAType",c.colorMode);scene.Set("uRGBABlend",c.colorBlend);scene.Set("uRGBAMix",c.colorMix);
  scene.Set("uRGBA_A",c.a[0],c.a[1],c.a[2],c.a[3]);scene.Set("uRGBA_B",c.b[0],c.b[1],c.b[2],c.b[3]);
  scene.Set("textMask",2);scene.Set("textReady",textTexture?1.f:0.f);scene.Set("textSize",c.textSize);scene.Set("textX",c.textX);scene.Set("textY",c.textY);scene.Set("textLayout",c.textLayout);
  glActiveTexture(GL_TEXTURE2);glBindTexture(GL_TEXTURE_2D,textTexture);draw();
  glUseProgram(blur.GetGLID());blur.Set("source",0);glActiveTexture(GL_TEXTURE0);glBindTexture(GL_TEXTURE_2D,texture[0]);glBindFramebuffer(GL_FRAMEBUFFER,fbo[1]);blur.Set("axis",3.f/w,0.f);draw();
  glBindTexture(GL_TEXTURE_2D,texture[1]);glBindFramebuffer(GL_FRAMEBUFFER,fbo[2]);blur.Set("axis",0.f,3.f/h);draw();
  glUseProgram(composite.GetGLID());composite.Set("source",0);composite.Set("bloom",1);composite.Set("glow",c.glow);composite.Set("contrast",c.contrast);composite.Set("detailAmount",c.detail);composite.Set("coverage",c.density);
  float alpha=c.opacity;if(c.colorMode>.5f)alpha*=c.colorMode<1.5f?c.a[3]:c.a[3]*(1.f-c.colorMix)+c.b[3]*c.colorMix;
  composite.Set("opacity",alpha);
  glActiveTexture(GL_TEXTURE0);glBindTexture(GL_TEXTURE_2D,texture[0]);glActiveTexture(GL_TEXTURE1);glBindTexture(GL_TEXTURE_2D,texture[2]);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER,state.drawFBO);glViewport(state.viewport[0],state.viewport[1],state.viewport[2],state.viewport[3]);draw();return true;
 }
};
