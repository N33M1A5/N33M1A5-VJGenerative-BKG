#include <FFGLSDK.h>
#include <FFGLParamRange.h>
#include <FFGLParamOption.h>
#include <FFGLScopedSamplerActivation.h>
#include <FFGLScopedTextureBinding.h>
#include <algorithm>
#include <cmath>

using namespace ffglex;
using namespace ffglqs;

static CFFGLPluginInfo PluginInfo(
    PluginFactory<class DigitalScan3D>,
    "NSD3", "N33M1A5 Digital Scan 3D",
    2, 1, 1, 0, FF_SOURCE,
    "Animated digital scan effect and abstract generator",
    "N33M1A5");

static const char* kFragment = R"(#version 410 core
uniform float time;
uniform vec2 resolution;
uniform float Modo;
uniform float Velocidade;
uniform float Densidade;
uniform float Dispersao;
uniform float Direcao;
uniform float Contraste;
uniform float Brilho;
uniform sampler2D inputTexture;
in vec2 uv;
out vec4 fragColor;
float hash(vec2 p){ p=fract(p*vec2(123.34,456.21)); p+=dot(p,p+34.345); return fract(p.x*p.y); }
void main(){
 float t=time*Velocidade*3.0;
 float aspect=resolution.x/max(resolution.y,1.0);
 vec2 shift=vec2((Direcao*2.0-1.0)*0.018*sin(t*1.7+uv.y*75.0),0.0);
 vec4 src=vec4(0.0,0.0,0.0,1.0);
 if(Modo>0.5) {
   vec2 sampleUV=clamp(uv+shift,vec2(0.001),vec2(0.999));
   src=texture(inputTexture,sampleUV);
 } else {
   vec2 q=(uv-0.5)*vec2(aspect,1.0);
   float r=length(q); float a=atan(q.y,q.x);
   float rings=0.5+0.5*sin(r*52.0-t*2.0+sin(a*7.0+t)*2.0);
   float wave=0.5+0.5*sin(q.y*88.0+sin(q.x*9.0-t)*4.0);
   vec2 cell=floor((uv+shift)*resolution/max(2.0,18.0-Densidade*14.0));
   float particles=step(1.0-Densidade*0.25,hash(cell));
   float v=max(rings*0.34,wave*0.24)+particles*0.7;
   src=vec4(vec3(clamp(v,0.0,1.0)),1.0);
 }
 float lum=dot(src.rgb,vec3(0.2126,0.7152,0.0722));
 float scan=0.78+0.22*sin(uv.y*resolution.y*1.15);
 float noise=(hash(vec2(floor(uv.y*resolution.y),floor(time*24.0)))-0.5)*Dispersao*0.25;
 float v=clamp((lum-0.5)*(Contraste*2.0)+0.5+noise,0.0,1.0)*scan;
 float edge=pow(max(v,0.0),2.0)*Brilho;
 fragColor=vec4(vec3(clamp(v+edge,0.0,1.0)),src.a);
})";

class DigitalScan3D : public ffglqs::Plugin {
public:
    DigitalScan3D() {
        SetMinInputs(0);
        SetMaxInputs(1);
        AddParam(ParamOption::Create("Modo", {
            {"Generator", 0.0f}, {"Effect", 1.0f}
        }, 0));
        AddParam(ParamRange::Create("Velocidade", 0.35f, {0.0f, 1.0f}));
        AddParam(ParamRange::Create("Densidade", 0.6f, {0.0f, 1.0f}));
        AddParam(ParamRange::Create("Dispersao", 0.4f, {0.0f, 1.0f}));
        AddParam(ParamRange::Create("Direcao", 0.5f, {0.0f, 1.0f}));
        AddParam(ParamRange::Create("Contraste", 0.7f, {0.0f, 1.0f}));
        AddParam(ParamRange::Create("Brilho", 0.2f, {0.0f, 1.0f}));
        SetFragmentShader(kFragment);
    }

    const char* GetShortName() override { return "NSD3"; }

    FFResult Render(ProcessOpenGLStruct* pGL) override {
        if (GetParamOption("Modo")->GetRealValue() > 0.5f) {
            if (!pGL || pGL->numInputTextures < 1 || !pGL->inputTextures || !pGL->inputTextures[0])
                return FF_FAIL;
            ScopedSamplerActivation sampler(0);
            Scoped2DTextureBinding texture(pGL->inputTextures[0]->Handle);
            shader.Set("inputTexture", 0);
        }
        quad.Draw();
        return FF_SUCCESS;
    }
};

