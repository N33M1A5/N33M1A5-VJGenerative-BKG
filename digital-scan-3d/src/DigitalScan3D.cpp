#include <FFGLSDK.h>
#include <FFGLParamRange.h>
#include <FFGLParamOption.h>
#include <FFGLScopedSamplerActivation.h>
#include <FFGLScopedTextureBinding.h>
#include <FFGLScopedShaderBinding.h>
#include <algorithm>
#include <cmath>

using namespace ffglex;
using namespace ffglqs;

static CFFGLPluginInfo PluginInfo(
    PluginFactory<class DigitalScan3D>,
    "NSD3", "N33M1A5 Digital Scan 3D",
    2, 1, 1, 0, FF_EFFECT,
    "Animated digital scan video effect",
    "N33M1A5");

static const char* kFragment = R"(#version 410 core
uniform sampler2D inputTexture;
uniform vec2 maxUV;
in vec2 i_uv;
out vec4 fragColor;
float hash(vec2 p){ p=fract(p*vec2(123.34,456.21)); p+=dot(p,p+34.345); return fract(p.x*p.y); }
void main(){
 vec2 uv=clamp(i_uv,vec2(0.0),maxUV);
 float t=time*Velocidade*3.0;
 vec2 shift=vec2((Direcao*2.0-1.0)*0.018*sin(t*1.7+uv.y*75.0),0.0);
 vec4 src=texture(inputTexture,clamp(uv+shift,vec2(0.001),max(maxUV,vec2(0.001))));
 float lum=dot(src.rgb,vec3(0.2126,0.7152,0.0722));
 float lineCount=max(resolution.y*(0.35+ Densidade*1.65),1.0);\n float scan=1.0-(0.04+0.18*Densidade)*(0.5+0.5*sin(uv.y*lineCount*6.2831853));
 float noise=(hash(vec2(floor(uv.y*lineCount),floor(time*24.0)))-0.5)*Dispersao*0.25;
 float v=clamp((lum-0.5)*(Contraste*2.0)+0.5+noise,0.0,1.0)*scan;
 float edge=pow(max(v,0.0),2.0)*Brilho;
 fragColor=vec4(vec3(clamp(v+edge,0.0,1.0)),src.a);
}
})";

class DigitalScan3D : public ffglqs::Plugin {
public:
    DigitalScan3D() {
        SetMinInputs(1);
        SetMaxInputs(1);
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
        if (!pGL || pGL->numInputTextures < 1 || !pGL->inputTextures || !pGL->inputTextures[0])
            return FF_FAIL;
        ScopedShaderBinding shaderBinding(shader.GetGLID());
        ScopedSamplerActivation sampler(0);
        Scoped2DTextureBinding texture(pGL->inputTextures[0]->Handle);
        shader.Set("inputTexture", 0);
        const FFGLTexCoords maxCoords = GetMaxGLTexCoords(*pGL->inputTextures[0]);
        shader.Set("maxUV", maxCoords.s, maxCoords.t);
        quad.Draw();
        return FF_SUCCESS;
    }
};

