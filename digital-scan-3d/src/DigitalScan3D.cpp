#include "FFGLSDK.h"
#include <chrono>
#include <cmath>
using namespace ffglex;

static const char* kVertex = R"(#version 410 core
layout(location=0) in vec4 vPosition;
layout(location=1) in vec2 vUV;
out vec2 uv;
void main(){ gl_Position=vPosition; uv=vUV; })";

static const char* kFragment = R"(#version 410 core
uniform vec3 iResolution;
uniform float iTime;
uniform float uMode, uSpeed, uDensity, uDispersion, uDirection, uContrast, uGlow;
uniform sampler2D inputTexture;
in vec2 uv;
out vec4 fragColor;
float hash(vec2 p){ p=fract(p*vec2(123.34,456.21)); p+=dot(p,p+34.345); return fract(p.x*p.y); }
void main(){
 vec2 p=uv; float t=iTime*uSpeed; float aspect=iResolution.x/max(iResolution.y,1.0);
 p.x=(p.x-.5)*aspect+.5;
 vec2 shift=vec2(uDirection*0.025*sin(t*1.7+p.y*75.0),0.0);
 vec4 src=vec4(0.0);
 if(uMode<0.5) src=texture(inputTexture,clamp(uv+shift,vec2(0.001),vec2(0.999)));
 else {
   vec2 q=(uv-.5)*vec2(aspect,1.0);
   float r=length(q); float a=atan(q.y,q.x);
   float rings=0.5+0.5*sin(r*48.0-t*2.0+sin(a*7.0+t)*2.0);
   float wave=0.5+0.5*sin(q.y*80.0+sin(q.x*9.0-t)*4.0);
   float particles=step(1.0-uDensity*0.25,hash(floor((uv+shift)*iResolution.xy/ max(2.0,18.0-uDensity*14.0))));
   float v=max(rings*0.35,wave*0.22)+particles*0.75;
   src=vec4(vec3(v),1.0);
 }
 float lum=dot(src.rgb,vec3(.2126,.7152,.0722));
 float scan=0.78+0.22*sin(uv.y*iResolution.y*1.15);
 float noise=(hash(vec2(floor(uv.y*iResolution.y),floor(iTime*24.0)))-.5)*uDispersion*.25;
 float v=clamp((lum-.5)*uContrast+0.5+noise,0.0,1.0)*scan;
 float edge=pow(max(v,0.0),2.0)*uGlow;
 vec3 outColor=vec3(v+edge);
 fragColor=vec4(outColor,src.a);
})";

enum { P_MODE, P_SPEED, P_DENSITY, P_DISPERSION, P_DIRECTION, P_CONTRAST, P_GLOW, P_COUNT };
class DigitalScan3D : public CFFGLPlugin {
public:
 DigitalScan3D(){
   SetMinInputs(0); SetMaxInputs(1);
   SetOptionParamInfo(P_MODE,"Modo",2,0.0f);
   SetParamElementInfo(P_MODE,0,"Generator",0.0f);
   SetParamElementInfo(P_MODE,1,"Effect",1.0f);
   SetParamInfof(P_SPEED,"Velocidade",FF_TYPE_STANDARD);
   SetParamInfof(P_DENSITY,"Densidade",FF_TYPE_STANDARD);
   SetParamInfof(P_DISPERSION,"Dispersao",FF_TYPE_STANDARD);
   SetParamInfof(P_DIRECTION,"Direcao",FF_TYPE_STANDARD);
   SetParamInfof(P_CONTRAST,"Contraste",FF_TYPE_STANDARD);
   SetParamInfof(P_GLOW,"Brilho",FF_TYPE_STANDARD);
   p[P_MODE]=0; p[P_SPEED]=.35f; p[P_DENSITY]=.6f; p[P_DISPERSION]=.4f; p[P_DIRECTION]=.5f; p[P_CONTRAST]=.7f; p[P_GLOW]=.2f;
 }
 FFResult InitGL(const FFGLViewportStruct* vp) override {
   if(!shader.Compile(kVertex,kFragment) || !quad.Initialise()){ DeInitGL(); return FF_FAIL; }
   last=std::chrono::steady_clock::now(); started=true; return CFFGLPlugin::InitGL(vp);
 }
 FFResult DeInitGL() override { shader.FreeGLResources(); quad.Release(); return FF_SUCCESS; }
 FFResult ProcessOpenGL(ProcessOpenGLStruct* pGL) override {
   auto now=std::chrono::steady_clock::now(); float dt=std::chrono::duration<float>(now-last).count(); last=now; if(dt<0||dt>.25f)dt=.016f; time+=dt;
   GLint vp[4]; glGetIntegerv(GL_VIEWPORT,vp);
   ScopedShaderBinding binding(shader.GetGLID());
   shader.Set("iResolution",(float)vp[2],(float)vp[3],1.0f);
   shader.Set("iTime",time); shader.Set("uMode",p[P_MODE]); shader.Set("uSpeed",p[P_SPEED]*3.0f);
   shader.Set("uDensity",p[P_DENSITY]); shader.Set("uDispersion",p[P_DISPERSION]); shader.Set("uDirection",(p[P_DIRECTION]-.5f)*2.0f);
   shader.Set("uContrast",p[P_CONTRAST]*2.0f); shader.Set("uGlow",p[P_GLOW]);
   quad.Draw(); return FF_SUCCESS;
 }
 FFResult SetFloatParameter(unsigned int i,float v) override { if(i>=P_COUNT)return FF_FAIL; p[i]=v; return FF_SUCCESS; }
 float GetFloatParameter(unsigned int i) override { return i<P_COUNT?p[i]:0.0f; }
private:
 FFGLShader shader; FFGLScreenQuad quad; float p[P_COUNT]{}; float time=0; std::chrono::steady_clock::time_point last; bool started=false;
};
static CFFGLPluginInfo PluginInfo(PluginFactory<DigitalScan3D>,"NSD3","N33M1A5 Digital Scan 3D",2,1,0,1,FF_SOURCE,"Digital scan / particle generator and video effect","N33M1A5");
