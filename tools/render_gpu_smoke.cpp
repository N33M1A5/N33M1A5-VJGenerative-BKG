#include <epoxy/gl.h>
#include <EGL/egl.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <cstdlib>

static const int W=128,H=72;
static std::string load(const std::string& path){
 std::ifstream f(path);if(!f)throw std::runtime_error("missing GLSL "+path);
 std::ostringstream b;b<<f.rdbuf();return b.str();
}
static GLuint compile(GLenum type,const std::string& src){
 GLuint sh=glCreateShader(type);
 const char* p=src.c_str();glShaderSource(sh,1,&p,nullptr);glCompileShader(sh);
 GLint ok=0;glGetShaderiv(sh,GL_COMPILE_STATUS,&ok);
 if(!ok){char log[16000]={};glGetShaderInfoLog(sh,sizeof(log),nullptr,log);throw std::runtime_error(log);}
 return sh;
}
static GLuint program(const char* vertex,const char* fragment){
 GLuint v=compile(GL_VERTEX_SHADER,load(vertex)),f=compile(GL_FRAGMENT_SHADER,load(fragment));
 GLuint p=glCreateProgram();glAttachShader(p,v);glAttachShader(p,f);
 glLinkProgram(p);GLint ok=0;glGetProgramiv(p,GL_LINK_STATUS,&ok);
 if(!ok){char log[16000]={};glGetProgramInfoLog(p,sizeof(log),nullptr,log);throw std::runtime_error(log);}
 glDeleteShader(v);glDeleteShader(f);return p;
}
static void set1(GLuint p,const char* name,float v){GLint l=glGetUniformLocation(p,name);if(l>=0)glUniform1f(l,v);}
static void set2(GLuint p,const char* name,float x,float y){GLint l=glGetUniformLocation(p,name);if(l>=0)glUniform2f(l,x,y);}
static void set3(GLuint p,const char* name,float x,float y,float z){GLint l=glGetUniformLocation(p,name);if(l>=0)glUniform3f(l,x,y,z);}
static void set4(GLuint p,const char* name,float x,float y,float z,float w){GLint l=glGetUniformLocation(p,name);if(l>=0)glUniform4f(l,x,y,z,w);}
static std::vector<unsigned char> draw(GLuint p,GLuint vao,int motor,int colorMode,float time){
 glUseProgram(p);
 set3(p,"iResolution",W,H,1.f);set1(p,"iTime",time);set1(p,"uBeat",1.35f);
 set1(p,"uEngine",float(motor));set1(p,"uVJEngine",float(motor));
 set1(p,"uPalette",0.f);set1(p,"uScale",.5f);set1(p,"uIntensity",.5f);
 set1(p,"uHue",0.f);set1(p,"uSlices",0.f);set1(p,"uSeed",0.f);
 set1(p,"uPulse",0.f);set1(p,"uContrast",.5f);
 set1(p,"uDetail",.5f);set1(p,"uDepth",.5f);set1(p,"uDensity",.5f);
 set1(p,"uRGBAType",float(colorMode));set1(p,"uRGBABlend",0.f);
 set1(p,"uRGBAMix",.5f);set1(p,"uOpacity",1.f);set1(p,"uGlow",0.f);
 set1(p,"uCanvasMode",0.f);set1(p,"uDiagnostic",0.f);
 set4(p,"uRGBA_A",1.f,0.f,0.f,1.f);set4(p,"uRGBA_B",0.f,0.f,1.f,1.f);
 set2(p,"uCustomSize",1920.f,1080.f);
 glViewport(0,0,W,H);glClearColor(0,0,0,1);glClear(GL_COLOR_BUFFER_BIT);
 glBindVertexArray(vao);glDrawArrays(GL_TRIANGLES,0,6);glFinish();
 GLenum e=glGetError();if(e!=GL_NO_ERROR)throw std::runtime_error("OpenGL draw error "+std::to_string(e));
 std::vector<unsigned char> pixels(W*H*4);glReadPixels(0,0,W,H,GL_RGBA,GL_UNSIGNED_BYTE,pixels.data());
 e=glGetError();if(e!=GL_NO_ERROR)throw std::runtime_error("glReadPixels error "+std::to_string(e));
 return pixels;
}
static double avgBrightness(const std::vector<unsigned char>& p){
 double v=0.;for(size_t i=0;i<p.size();i+=4)v+=double(p[i])+double(p[i+1])+double(p[i+2]);
 return v/(W*H*3.);
}
static double avgDifference(const std::vector<unsigned char>& a,const std::vector<unsigned char>& b){
 double d=0;for(size_t i=0;i<a.size();i+=4)for(int c=0;c<3;c++)d+=std::abs(int(a[i+c])-int(b[i+c]));
 return d/(W*H*3.);
}
int main(){
 try{
  EGLDisplay display=eglGetDisplay(EGL_DEFAULT_DISPLAY);
  if(display==EGL_NO_DISPLAY||!eglInitialize(display,nullptr,nullptr))throw std::runtime_error("No EGL display - set EGL_PLATFORM=surfaceless");
  if(!eglBindAPI(EGL_OPENGL_API))throw std::runtime_error("EGL OpenGL API binding failed");
  const EGLint attrs[]={EGL_SURFACE_TYPE,EGL_PBUFFER_BIT,EGL_RENDERABLE_TYPE,EGL_OPENGL_BIT,EGL_RED_SIZE,8,EGL_GREEN_SIZE,8,EGL_BLUE_SIZE,8,EGL_NONE};
  EGLConfig config;EGLint count=0;
  if(!eglChooseConfig(display,attrs,&config,1,&count)||count<1)throw std::runtime_error("No EGL OpenGL PBuffer config");
  const EGLint pbAttrs[]={EGL_WIDTH,W,EGL_HEIGHT,H,EGL_NONE};
  EGLSurface surface=eglCreatePbufferSurface(display,config,pbAttrs);
  if(surface==EGL_NO_SURFACE)throw std::runtime_error("No EGL PBuffer surface");
  const EGLint ctxAttrs[]={EGL_CONTEXT_MAJOR_VERSION,4,EGL_CONTEXT_MINOR_VERSION,1,EGL_NONE};
  EGLContext ctx=eglCreateContext(display,config,EGL_NO_CONTEXT,ctxAttrs);
  if(ctx==EGL_NO_CONTEXT)throw std::runtime_error("Unable to create OpenGL 4.1 context");
  if(!eglMakeCurrent(display,surface,surface,ctx))throw std::runtime_error("eglMakeCurrent failed");
  if(!glGetString(GL_VERSION))throw std::runtime_error("Unable to load OpenGL via EGL/libepoxy");
  glGetError();
  std::cout<<"GPU "<<(const char*)glGetString(GL_RENDERER)<<" | OpenGL "<<(const char*)glGetString(GL_VERSION)<<"\n";
  const float quad[]={
   -1.f,-1.f,0.f,1.f,0.f,0.f, 1.f,-1.f,0.f,1.f,1.f,0.f, 1.f,1.f,0.f,1.f,1.f,1.f,
   -1.f,-1.f,0.f,1.f,0.f,0.f, 1.f,1.f,0.f,1.f,1.f,1.f, -1.f,1.f,0.f,1.f,0.f,1.f };
  GLuint vao=0,vbo=0;glGenVertexArrays(1,&vao);glBindVertexArray(vao);
  glGenBuffers(1,&vbo);glBindBuffer(GL_ARRAY_BUFFER,vbo);
  glBufferData(GL_ARRAY_BUFFER,sizeof(quad),quad,GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)0);
  glEnableVertexAttribArray(1);glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)(4*sizeof(float)));
  GLuint legacy=program("glsl-validation/legacy.vert","glsl-validation/legacy.frag");
  GLuint customized=program("glsl-validation/legacy.vert","glsl-validation/legacy_palette.frag");
  GLuint expanded=program("glsl-validation/expanded.vert","glsl-validation/expanded.frag");
  for(int i=0;i<6;i++){
   auto pixels=draw(legacy,vao,i,0,5.f);
   auto result=avgBrightness(pixels);
   std::cout<<"LEGACY engine "<<i<<" average "<<result<<"\n";
   if(result<.1)throw std::runtime_error("Original motor rendered black: "+std::to_string(i));
  }
  for(int i=0;i<18;i++){
   auto pixels=draw(expanded,vao,i,0,5.f);
   auto mean=avgBrightness(pixels);
   std::cout<<"EXPANDED engine "<<(i+6)<<" average "<<mean<<"\n";
   if(mean<.12)throw std::runtime_error("New motor rendered black: "+std::to_string(i+6));
  }
  auto legacyDefault=draw(legacy,vao,0,0,5.f);
  auto legacyCustom=draw(customized,vao,0,2,5.f);
  auto customDifference=avgDifference(legacyDefault,legacyCustom);
  std::cout<<"Custom palette changes classic "<<customDifference<<"\n";
  if(customDifference<.65)throw std::runtime_error("Custom RGB palette did not change original engine colors");
  auto modernDefault=draw(expanded,vao,0,0,5.f);
  auto modernCustom=draw(expanded,vao,0,2,5.f);
  double newDifference=avgDifference(modernDefault,modernCustom);
  std::cout<<"Custom palette changes new motor "<<newDifference<<"\n";
  if(newDifference<.65)throw std::runtime_error("Custom RGB palette did not change new engine colors");
  auto first=draw(expanded,vao,1,0,5.f);
  auto later=draw(expanded,vao,1,0,6.f);
  double motion=avgDifference(first,later);
  std::cout<<"Particle wave animation after 1s: "<<motion<<"\n";
  if(motion<.20)throw std::runtime_error("New motor did not animate with Time");
  std::cout<<"PASS: 6 legacy, 18 new, user-defined palettes and animation render on OpenGL GPU.\n";
  glDeleteProgram(legacy);glDeleteProgram(customized);glDeleteProgram(expanded);
  glDeleteBuffers(1,&vbo);glDeleteVertexArrays(1,&vao);
  eglMakeCurrent(display,EGL_NO_SURFACE,EGL_NO_SURFACE,EGL_NO_CONTEXT);
  eglDestroyContext(display,ctx);eglDestroySurface(display,surface);eglTerminate(display);
  return 0;
 }catch(const std::exception& ex){std::cerr<<"GPU RENDER TEST FAILED: "<<ex.what()<<"\n";return 2;}
}
