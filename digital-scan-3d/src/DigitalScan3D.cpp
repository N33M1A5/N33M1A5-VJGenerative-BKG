#include <FFGLSDK.h>
#include <FFGLParamRange.h>
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

static const char* kFragment = R"(
uniform sampler2D inputTexture;
uniform vec2 maxUV;
in vec2 i_uv;

float hash(vec2 p) {
    p = fract(p * vec2(123.34, 456.21));
    p += dot(p, p + 34.345);
    return fract(p.x * p.y);
}

void main() {
    vec2 uv = clamp(i_uv, vec2(0.0), maxUV);
    float t = time * max(Velocidade, 0.01) * 3.0;

    // Direction is an actual signed scan displacement; 0.0 moves left, 1.0 moves right.
    float dir = (Direcao < 0.5) ? -1.0 : 1.0;
    float wave = sin(t * 1.7 + uv.y * 75.0);
    vec2 shiftedUV = clamp(uv + vec2(dir * wave * 0.035 * Dispersao, 0.0),
                           vec2(0.001), max(maxUV, vec2(0.001)));

    vec4 src = texture(inputTexture, shiftedUV);
    float lum = dot(src.rgb, vec3(0.2126, 0.7152, 0.0722));

    float lineCount = max(resolution.y * (0.5 + Densidade * 2.5), 1.0);
    float lineWave = 0.5 + 0.5 * sin(uv.y * lineCount * 6.2831853);
    float scan = 1.0 - (0.08 + 0.32 * Densidade) * lineWave;

    float row = floor(uv.y * lineCount);
    float noise = (hash(vec2(row, floor(time * (12.0 + Velocidade * 48.0)))) - 0.5)
                  * Dispersao * 0.5;

    float contrast = max(Contraste * 3.0, 0.05);
    float signal = clamp((lum - 0.5) * contrast + 0.5 + noise, 0.0, 1.0);
    signal *= scan;

    // A small animated digital tear makes the effect visible even on flat footage.
    float tear = step(0.985, hash(vec2(row, floor(time * 8.0))))
                 * Dispersao * 0.35;
    signal = clamp(signal + tear, 0.0, 1.0);

    float glow = pow(signal, 2.0) * Brilho;
    vec3 result = vec3(clamp(signal + glow, 0.0, 1.0));
    fragColor = vec4(result, src.a);
}
)";

class DigitalScan3D : public ffglqs::Plugin {
public:
    DigitalScan3D() {
        SetMinInputs(1);
        SetMaxInputs(1);
        AddParam(ParamRange::Create("Velocidade", 0.5f, {0.0f, 1.0f}));
        AddParam(ParamRange::Create("Densidade", 0.6f, {0.0f, 1.0f}));
        AddParam(ParamRange::Create("Dispersao", 0.6f, {0.0f, 1.0f}));
        AddParam(ParamRange::Create("Direcao", 1.0f, {0.0f, 1.0f}));
        AddParam(ParamRange::Create("Contraste", 0.7f, {0.0f, 1.0f}));
        AddParam(ParamRange::Create("Brilho", 0.35f, {0.0f, 1.0f}));
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
