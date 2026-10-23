#include "TextAtlas.h" // Windows text APIs must precede FFGL
#include "FFGLSDK.h"
#include "shaders.h" // IMMUTABLE V1.0.1 shader
#include "shaders_expansion.h"
#include "palettes_extra.h"
#include "StoryPipeline.h"
#include <string>
#include <algorithm>
#include <cstdio>
#include <chrono>
#include <cmath>

using namespace ffglex;

enum Param
{
	P_ENGINE,
    P_EXTRA_MOTOR, // second native FFGL option row: Classic or Particle Wave
	P_MOTOR_ROW_FIRST, P_MOTOR_ROW_LAST=P_MOTOR_ROW_FIRST+11,
	P_PALETTE,
    P_COLOR_MODE, P_COLOR_BLEND, P_COLOR_MIX,
    P_A_R, P_A_G, P_A_B, P_A_ALPHA,
    P_B_R, P_B_G, P_B_B, P_B_ALPHA,
	P_SPEED,
	P_SCALE,
	P_INTENSITY,
	P_HUE,
	P_SYMMETRY,
	P_SEED,
	P_BPM,
	P_PULSE,
	P_CONTRAST,
    P_DETAIL, P_DEPTH, P_DENSITY, P_ORIENTATION, P_DIRECTION, P_GLOW, P_OPACITY,
    P_CANVAS, P_CUSTOM_W, P_CUSTOM_H, P_GPU_TEST,
	P_TEXT, P_TEXT_SIZE, P_TEXT_X, P_TEXT_Y, P_TEXT_LAYOUT,
    P_LOOP, P_QUALITY, P_COLOR_SOURCE, P_ACTIVE_ENGINE,
	P_COUNT
};

// Exact six original V1.0.1 motor names and order; native six-button FFGL row.
static const char* kEngines[] = { "Nevoa", "Tunel", "Lasers", "Celulas", "Aurora", "Orbes" };
static const char* kExtraMotors[] = { "Classicos", "Particle Wave" };
static const int kNumExtraMotors = 2;
// The original 9 palette algorithms keep IDs 0..8. Display order is alphabetical.
static const char* kPalettes[] = { "Acido Neon", "Ambar", "Arco-iris", "Artico", "Aurora Boreal", "Azul Eletrico", "Brasa", "Cobre", "Cyberpunk", "Deserto", "Dourado", "Esmeralda", "Floresta", "Fogo", "Galaxia", "Gelo", "Holograma", "Jade", "Lava", "Limao Neon", "Magenta Laser", "Mono", "Neon", "Oceano", "Outrun", "Por do sol", "Rosa Quartzo", "Roxo Laser", "Safira", "Solar", "Synthwave", "Tropical", "Turquesa", "Ultravioleta", "Vaporwave", "Vermelho Rubi" };
static const int kPaletteShaderId[] = { 9, 10, 0, 11, 12, 13, 14, 15, 16, 17, 18, 19, 5, 1, 20, 6, 21, 22, 23, 24, 25, 8, 3, 2, 26, 7, 27, 28, 29, 30, 31, 32, 33, 4, 34, 35 };
static const int kNumEngines   = sizeof(kEngines)/sizeof(kEngines[0]);
static const int kNumPalettes  = sizeof(kPalettes)/sizeof(kPalettes[0]);
static constexpr int kDefaultPaletteIndex=2; // "Arco-iris" -> original shader palette ID 0
static const char* kColorModes[] = {"Original", "RGBA A", "Paleta A+B", "A+B Animada"};
static const char* kColorBlends[] = {"Crossfade", "Add", "Screen", "Multiply"};
static const char* kCanvasModes[] = {"Auto / Output", "16:9", "9:16", "32:9", "Custom"};
static constexpr unsigned int kClassicCount = 6;

static bool replaceOnce(std::string& s, const std::string& from, const std::string& to)
{
    const auto p=s.find(from);
    if(p==std::string::npos || s.find(from,p+from.size())!=std::string::npos)return false;
    s.replace(p,from.size(),to);
    return true;
}
#include "LegacyAdjust.h"

// Make a *separate* runtime variant of the original shader for opt-in RGBA/opacity/glow.
// The original shader string is never modified; factory defaults use its original GL program.
static std::string makeClassicPaletteShader()
{
    std::string s(kFragmentShader);
    if(!replaceOnce(s,"vec3 pal(float t){","vec3 originalPal(float t){"))return {};
    if(!replaceOnce(s,"uniform float uSlices, uSeed, uPulse, uContrast;",
       "uniform float uSlices, uSeed, uPulse, uContrast;\n"
       "uniform float uRGBAType,uRGBABlend,uRGBAMix,uOpacity,uGlow;\n"
       "uniform vec4 uRGBA_A,uRGBA_B;"))return {};
    const std::string customPalette=std::string(kPresetPaletteSource)+R"GLSL(
vec3 pal(float t){
    vec3 original=originalPal(t);
    int mode=int(uRGBAType+.5);
    if(mode==0)return int(uPalette+.5)>=9?palettePreset(int(uPalette+.5),t):original;
    vec3 A=clamp(uRGBA_A.rgb,0.,1.),B=clamp(uRGBA_B.rgb,0.,1.);
    if(mode==1)return A;
    float phase=t+uHue+(mode==3?iTime*.055:0.);
    float k=clamp(.5+.5*cos(6.28318530718*phase)+uRGBAMix-.5,0.,1.);
    int blending=int(uRGBABlend+.5);
    if(blending==1)return clamp(A+B*k,0.,1.);
    if(blending==2)return 1.-(1.-A)*(1.-B*k);
    if(blending==3)return mix(A,A*B,k);
    return mix(A,B,k);
}
)GLSL";
    if(!replaceOnce(s,"// 0 - Nevoa (domain warping)",customPalette+"\n// 0 - Nevoa (domain warping)"))return {};
    if(!replaceOnce(s,"fragColor=vec4(clamp(col,0.,1.),1.);",
       "col+=max(col-vec3(.16),vec3(0.))*clamp(uGlow,0.,1.)*1.15;\n"
       "float alpha=clamp(uOpacity,0.,1.);\n"
       "if(int(uRGBAType+.5)>0)alpha*=int(uRGBAType+.5)==1?uRGBA_A.a:mix(uRGBA_A.a,uRGBA_B.a,clamp(uRGBAMix,0.,1.));\n"
       "fragColor=vec4(clamp(col,0.,1.),alpha);"))return {};
    if(!addSharedAdjustments(s,true))return {};
    return s;
}


static std::string makeExpansionPaletteShader()
{
    std::string s(kVJFragmentShader);
    if(!replaceOnce(s,"vec3 palOriginal(float t){",
       std::string(kPresetPaletteSource)+"\nvec3 palOriginal(float t){"))return {};
    if(!replaceOnce(s,"vec3 fixedColor=palOriginal(t);",
       "int id=int(uPalette+.5);\n vec3 fixedColor=id>=9?palettePreset(id,t):palOriginal(t);"))return {};
    if(!addSharedAdjustments(s,false))return {};
    return s;
}


class VJGenerative : public CFFGLPlugin
{
public:
	VJGenerative();
	~VJGenerative() {}

	FFResult InitGL( const FFGLViewportStruct* vp ) override;
	FFResult ProcessOpenGL( ProcessOpenGLStruct* pGL ) override;
	FFResult DeInitGL() override;
	FFResult SetFloatParameter( unsigned int index, float value ) override;
	float GetFloatParameter( unsigned int index ) override;
    char* GetParameterDisplay(unsigned int index) override;
    FFResult SetTextParameter(unsigned int index,const char* value) override;
    char* GetTextParameter(unsigned int index) override;

#ifdef VJ_TEST_HOST
    void testClock(std::chrono::steady_clock::time_point (*fn)()){clockNow=fn;time=0;beats=0;started=false;}
    void testTextMask(const unsigned char* pixels){story.setTestTextMask(pixels);}
#endif
private:
	FFGLShader shader; // V1.0.1 untouched
    FFGLShader classicColorShader; // built from original, new palette only
    FFGLShader expansionShader;
    bool classicColorReady=false, expansionReady=false;
    char display[96] = {};
    StoryPipeline story;
    int activeEngine=0;
    std::chrono::steady_clock::time_point (*clockNow)()=&std::chrono::steady_clock::now;
    void syncControlVisibility(bool notify);
    std::string editableText="JESUS\nREI DOS REIS\nSENHOR DOS SENHORES";
	FFGLScreenQuad quad;
	float params[ P_COUNT ] = {};
	double time       = 0.0;  // tempo acumulado (afetado pela Speed)
	double beats      = 0.0;  // batidas acumuladas (tempo real x BPM)
	std::chrono::steady_clock::time_point last;
	bool started = false;
};

VJGenerative::VJGenerative()
{
    SetMinInputs(0);
    SetMaxInputs(0);

    // Defaults FIRST. SetParamInfof() reads GetFloatParameter() during registration.
    params[P_ENGINE]=0.f; params[P_EXTRA_MOTOR]=0.f; params[P_PALETTE]=(float)kDefaultPaletteIndex;
    params[P_COLOR_MODE]=0.f; params[P_COLOR_BLEND]=0.f; params[P_COLOR_MIX]=.5f;
    params[P_A_R]=1.f; params[P_A_G]=0.f; params[P_A_B]=0.f; params[P_A_ALPHA]=1.f;
    params[P_B_R]=0.f; params[P_B_G]=0.f; params[P_B_B]=1.f; params[P_B_ALPHA]=1.f;
    params[P_SPEED]=.5f; params[P_SCALE]=.5f; params[P_INTENSITY]=.5f;
    params[P_HUE]=0.f; params[P_SYMMETRY]=0.f; params[P_SEED]=0.f;
    params[P_BPM]=.5f; params[P_PULSE]=0.f; params[P_CONTRAST]=.5f;
    params[P_DETAIL]=.5f; params[P_DEPTH]=.5f; params[P_DENSITY]=.5f;
    params[P_DIRECTION]=0.f; params[P_GLOW]=0.f; params[P_OPACITY]=1.f;
    params[P_LOOP]=16.f/116.f;params[P_QUALITY]=1.f;params[P_COLOR_SOURCE]=0.f;
    params[P_TEXT_SIZE]=.5f;params[P_TEXT_X]=.5f;params[P_TEXT_Y]=.5f;params[P_TEXT_LAYOUT]=1.f;
    params[P_ACTIVE_ENGINE]=0.f;
    params[P_CANVAS]=0.f;
    params[P_CUSTOM_W]=(1920.f-128.f)/8064.f;
    params[P_CUSTOM_H]=(1080.f-128.f)/8064.f; params[P_GPU_TEST]=0.f;

    // One native V1.0-style control, with all 82 engines as individual options.
    SetOptionParamInfo(P_ENGINE,"Motores",kTotalEngines,0.f);
    for(int i=0;i<kTotalEngines;i++){
       int id=kMotorUIOrder[i];
       SetParamElementInfo(P_ENGINE,i,kMotorLabels[id],(float)id);
    }
    SetParamGroup(P_ENGINE,"MOTORES");

    SetOptionParamInfo(P_PALETTE,"Paleta",kNumPalettes,(float)kDefaultPaletteIndex);
    for(int i=0;i<kNumPalettes;i++)
       SetParamElementInfo(P_PALETTE,i,kPalettes[i],(float)i);

    // One color section for all 82 engines, immediately after Paleta.
    SetOptionParamInfo(P_COLOR_MODE,"Modo de Cor",4,0.f);
    SetOptionParamInfo(P_COLOR_BLEND,"Mistura Cores",4,0.f);
    for(int i=0;i<4;i++){
       SetParamElementInfo(P_COLOR_MODE,i,kColorModes[i],(float)i);
       SetParamElementInfo(P_COLOR_BLEND,i,kColorBlends[i],(float)i);
    }
    SetParamInfof(P_COLOR_MIX,"Mix Cor A/B",FF_TYPE_STANDARD);
    SetParamInfof(P_A_R,"A Red",FF_TYPE_RED);
    SetParamInfof(P_A_G,"A Green",FF_TYPE_GREEN);
    SetParamInfof(P_A_B,"A Blue",FF_TYPE_BLUE);
    SetParamInfof(P_A_ALPHA,"A Alpha",FF_TYPE_ALPHA);
    SetParamInfof(P_B_R,"B Red",FF_TYPE_RED);
    SetParamInfof(P_B_G,"B Green",FF_TYPE_GREEN);
    SetParamInfof(P_B_B,"B Blue",FF_TYPE_BLUE);
    SetParamInfof(P_B_ALPHA,"B Alpha",FF_TYPE_ALPHA);
    for(unsigned i=P_COLOR_MODE;i<=P_B_ALPHA;i++)SetParamGroup(i,"PALETA PERSONALIZADA | RGBA");

    SetParamInfof(P_SPEED,"Velocidade",FF_TYPE_STANDARD);
    SetParamInfof(P_SCALE,"Escala",FF_TYPE_STANDARD);
    SetParamInfof(P_INTENSITY,"Intensidade",FF_TYPE_STANDARD);
    SetParamInfof(P_HUE,"Matiz",FF_TYPE_STANDARD);
    SetParamInfof(P_SYMMETRY,"Simetria",FF_TYPE_STANDARD);
    SetParamInfof(P_SEED,"Semente",FF_TYPE_STANDARD);
    SetParamInfof(P_BPM,"BPM (60-180)",FF_TYPE_STANDARD);
    SetParamInfof(P_PULSE,"Pulso",FF_TYPE_STANDARD);
    SetParamInfof(P_CONTRAST,"Contraste",FF_TYPE_STANDARD);

    SetParamInfof(P_DETAIL,"Detalhe",FF_TYPE_STANDARD);
    SetParamInfof(P_DEPTH,"Profundidade",FF_TYPE_STANDARD);
    SetParamInfof(P_DENSITY,"Densidade",FF_TYPE_STANDARD);
    SetParamInfof(P_ORIENTATION,"Direcao 0-360",FF_TYPE_STANDARD);
    SetParamInfof(P_DIRECTION,"Inverter Movimento",FF_TYPE_BOOLEAN);
    SetParamInfof(P_GLOW,"Realce de Luz",FF_TYPE_STANDARD);
    SetParamInfof(P_OPACITY,"Opacidade",FF_TYPE_ALPHA);
    for(unsigned i=P_DETAIL;i<=P_OPACITY;i++)SetParamGroup(i,"CONTROLES AVANCADOS");
    SetOptionParamInfo(P_CANVAS,"Canvas",5,0.f);
    for(int i=0;i<5;i++)SetParamElementInfo(P_CANVAS,i,kCanvasModes[i],(float)i);
    SetParamInfof(P_CUSTOM_W,"Largura Custom",FF_TYPE_STANDARD);
    SetParamInfof(P_CUSTOM_H,"Altura Custom",FF_TYPE_STANDARD);
    for(unsigned i=P_CANVAS;i<=P_CUSTOM_H;i++)SetParamGroup(i,"RESOLUCAO | LED");
    SetParamInfo(P_TEXT,"Texto Editavel",FF_TYPE_TEXT,editableText.c_str());
    SetParamInfof(P_TEXT_SIZE,"Tamanho Texto",FF_TYPE_STANDARD);
    SetParamInfof(P_TEXT_X,"Texto X",FF_TYPE_STANDARD);SetParamInfof(P_TEXT_Y,"Texto Y",FF_TYPE_STANDARD);
    SetOptionParamInfo(P_TEXT_LAYOUT,"Layout Texto",2,1.f);
    SetParamElementInfo(P_TEXT_LAYOUT,0,"Central",0.f);SetParamElementInfo(P_TEXT_LAYOUT,1,"Parede",1.f);
    for(unsigned i=P_TEXT;i<=P_TEXT_LAYOUT;i++)SetParamGroup(i,"TEXTO | WALLS");
    SetParamInfof(P_LOOP,"Periodo do Loop",FF_TYPE_STANDARD);
    SetOptionParamInfo(P_QUALITY,"Qualidade",3,1.f);
    SetParamElementInfo(P_QUALITY,0,"50%",0.f);SetParamElementInfo(P_QUALITY,1,"75%",1.f);SetParamElementInfo(P_QUALITY,2,"100%",2.f);
    SetOptionParamInfo(P_COLOR_SOURCE,"Cores do Motor",2,0.f);
    SetParamElementInfo(P_COLOR_SOURCE,0,"Referencia",0.f);SetParamElementInfo(P_COLOR_SOURCE,1,"Paleta",1.f);
    for(unsigned i=P_LOOP;i<=P_COLOR_SOURCE;i++)SetParamGroup(i,"RENDER");
    SetParamInfof(P_ACTIVE_ENGINE,"Engine State",FF_TYPE_STANDARD);SetParamVisibility(P_ACTIVE_ENGINE,false,false);
    SetParamInfof(P_GPU_TEST,"Teste de Shader GPU",FF_TYPE_BOOLEAN);
    SetParamGroup(P_GPU_TEST,"DIAGNOSTICO");
    // Final compact operator layout: Motors -> Palettes -> RGBA -> General.
    SetParamGroup(P_ENGINE,"MOTORES");
    SetParamGroup(P_PALETTE,"PALETAS");
    for(unsigned i=P_COLOR_MODE;i<=P_B_ALPHA;i++)SetParamGroup(i,"CONTROLE RGBA");
    for(unsigned i=P_SPEED;i<P_ACTIVE_ENGINE;i++)SetParamGroup(i,"CONTROLES GERAIS");
    syncControlVisibility(false);
}
FFResult VJGenerative::InitGL(const FFGLViewportStruct* vp)
{
    if(!shader.Compile(kVertexShader,kFragmentShader)){
       DeInitGL();
       return FF_FAIL;
    }
    std::string custom = makeClassicPaletteShader();
    classicColorReady=!custom.empty() && classicColorShader.Compile(kVertexShader,custom.c_str());
    std::string expanded=makeExpansionPaletteShader();
    expansionReady=!expanded.empty() && expansionShader.Compile(kVJVertexShader,expanded.c_str());
    if(!classicColorReady)FFGLLog::LogToHost("N33 Unified: palette shader failed; original V1 still available.");
    if(!expansionReady)FFGLLog::LogToHost("N33 Unified: expansion shader failed; original V1 still available.");
    if(!quad.Initialise()){DeInitGL();return FF_FAIL;}
    if(!story.init()){DeInitGL();return FF_FAIL;}
    started=false;
    return CFFGLPlugin::InitGL(vp);
}

FFResult VJGenerative::DeInitGL()
{
    story.release();
    shader.FreeGLResources();
    classicColorShader.FreeGLResources();
    expansionShader.FreeGLResources();
    classicColorReady=false;expansionReady=false;
    quad.Release();
    return FF_SUCCESS;
}

FFResult VJGenerative::ProcessOpenGL(ProcessOpenGLStruct* pGL)
{
    auto now=clockNow();
    if(!started){last=now;started=true;}
    double dt=std::chrono::duration<double>(now-last).count();
    last=now;
    if(dt>0.25)dt=0.25;
    if(dt<0.)dt=0.;
    const double speed=params[P_SPEED]*2.0;
    time+=dt*speed*(params[P_DIRECTION]>=.5f?-1.0:1.0);
    const double bpm=60.0+params[P_BPM]*120.0;
    beats+=dt*bpm/60.0;

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT,viewport);
    float width=(float)std::max(1,viewport[2]);
    float height=(float)std::max(1,viewport[3]);
    // Procedural output uses the REAL viewport in default Auto mode.
    int canvas=(int)std::lround(params[P_CANVAS]);
    if(canvas==1)width=height*(16.f/9.f);
    else if(canvas==2)width=height*(9.f/16.f);
    else if(canvas==3)width=height*(32.f/9.f);
    else if(canvas==4)width=height*(128.f+8064.f*params[P_CUSTOM_W])/(128.f+8064.f*params[P_CUSTOM_H]);

    int motor=std::max(0,std::min(5,activeEngine));
    const bool particleWave=activeEngine==6;
    const bool classic=activeEngine<6;
    const int paletteIndex=std::max(0,std::min(kNumPalettes-1,(int)std::lround(params[P_PALETTE])));
    const int paletteId=kPaletteShaderId[paletteIndex];
    if(activeEngine>=7){
        StoryControls c{};c.time=(float)time;c.beat=(float)beats;c.width=width;c.height=height;
        c.scale=params[P_SCALE];c.intensity=params[P_INTENSITY];c.hue=params[P_HUE];c.symmetry=params[P_SYMMETRY];c.seed=params[P_SEED]*100.f;c.pulse=params[P_PULSE];
        c.detail=params[P_DETAIL];c.depth=params[P_DEPTH];c.density=params[P_DENSITY];c.glow=params[P_GLOW];c.opacity=params[P_OPACITY];c.contrast=params[P_CONTRAST];
        c.orientation=params[P_ORIENTATION]*6.28318530718f;c.period=4.f+116.f*params[P_LOOP];c.quality=params[P_QUALITY];c.colorSource=params[P_COLOR_SOURCE];c.palette=(float)paletteId;
        c.colorMode=params[P_COLOR_MODE];c.colorBlend=params[P_COLOR_BLEND];c.colorMix=params[P_COLOR_MIX];
        for(int i=0;i<4;i++){c.a[i]=params[P_A_R+i];c.b[i]=params[P_B_R+i];}
        c.text=editableText.c_str();c.textSize=params[P_TEXT_SIZE];c.textX=(params[P_TEXT_X]-.5f)*3.f;c.textY=(params[P_TEXT_Y]-.5f)*2.f;c.textLayout=params[P_TEXT_LAYOUT];
        return story.render(activeEngine-7,c)?FF_SUCCESS:FF_FAIL;
    }
    const bool originalLook=params[P_COLOR_MODE]<.5f &&
                            params[P_OPACITY]>.9999f && params[P_GLOW]<.0001f &&
                            paletteId<9 && params[P_ORIENTATION]<.000001f && std::abs(params[P_DETAIL]-.5f)<.000001f && std::abs(params[P_DEPTH]-.5f)<.000001f && std::abs(params[P_DENSITY]-.5f)<.000001f;
    FFGLShader* active=&shader;
    if(classic && !originalLook && classicColorReady)active=&classicColorShader;
    else if(particleWave && expansionReady)active=&expansionShader;

    ScopedShaderBinding binding(active->GetGLID());
    active->Set("iResolution",width,height,1.f);
    active->Set("iTime",(float)(time*20.0/(4.0+116.0*params[P_LOOP])));
    active->Set("uOrientation",params[P_ORIENTATION]*6.28318530718f);
    active->Set("uGlobalDetail",params[P_DETAIL]);active->Set("uGlobalDepth",params[P_DEPTH]);active->Set("uGlobalDensity",params[P_DENSITY]);
    active->Set("uBeat",(float)beats);
    active->Set("uEngine",(float)motor);
    active->Set("uPalette",(float)paletteId);
    active->Set("uScale",params[P_SCALE]);
    active->Set("uIntensity",params[P_INTENSITY]);
    active->Set("uHue",params[P_HUE]);
    active->Set("uSlices",params[P_SYMMETRY]);
    active->Set("uSeed",params[P_SEED]*100.f);
    active->Set("uPulse",params[P_PULSE]);
    active->Set("uContrast",params[P_CONTRAST]);

    if(active==&classicColorShader || active==&expansionShader){
        active->Set("uRGBAType",params[P_COLOR_MODE]);
        active->Set("uRGBABlend",params[P_COLOR_BLEND]);
        active->Set("uRGBAMix",params[P_COLOR_MIX]);
        active->Set("uRGBA_A",params[P_A_R],params[P_A_G],params[P_A_B],params[P_A_ALPHA]);
        active->Set("uRGBA_B",params[P_B_R],params[P_B_G],params[P_B_B],params[P_B_ALPHA]);
        active->Set("uOpacity",params[P_OPACITY]);
        active->Set("uGlow",params[P_GLOW]);
    }
    if(active==&expansionShader){
        active->Set("uVJEngine",1.f); // Particle Wave is engine index 1 in expansion shader.
        active->Set("uDetail",params[P_DETAIL]);
        active->Set("uDepth",params[P_DEPTH]);
        active->Set("uDensity",params[P_DENSITY]);
        active->Set("uCanvasMode",params[P_CANVAS]);
        active->Set("uCustomSize",128.f+8064.f*params[P_CUSTOM_W],128.f+8064.f*params[P_CUSTOM_H]);
        active->Set("uDiagnostic",params[P_GPU_TEST]);
    }
    quad.Draw();
    return FF_SUCCESS;
}

FFResult VJGenerative::SetFloatParameter( unsigned int index, float value )
{
	if( index >= P_COUNT ) return FF_FAIL;
    if(!std::isfinite(value))return FF_FAIL;
    if(index==P_ENGINE){
       int requested=(int)std::lround(value);
       if(requested>=0&&requested<kTotalEngines){
          if(activeEngine!=requested){activeEngine=requested;syncControlVisibility(true);}
          params[P_ACTIVE_ENGINE]=(float)activeEngine/(kTotalEngines-1);
       }
       return FF_SUCCESS;
    }
    if(index==P_ACTIVE_ENGINE){activeEngine=(int)std::lround(std::max(0.f,std::min(1.f,value))*(kTotalEngines-1));params[index]=(float)activeEngine/(kTotalEngines-1);syncControlVisibility(true);return FF_SUCCESS;}
    if(index==P_QUALITY)params[index]=std::round(std::max(0.f,std::min(2.f,value)));
    else if(index==P_PALETTE){float next=std::max(0.f,std::min((float)(kNumPalettes-1),value));if(next!=params[index])params[P_COLOR_SOURCE]=1.f;params[index]=next;}
    else if(index==P_COLOR_MODE || index==P_COLOR_BLEND)params[index]=std::max(0.f,std::min(3.f,value));
    else if(index==P_CANVAS){params[index]=std::max(0.f,std::min(4.f,value));syncControlVisibility(true);}
    else params[index]=std::max(0.f,std::min(1.f,value));
	return FF_SUCCESS;
}

float VJGenerative::GetFloatParameter( unsigned int index )
{
	if(index==P_ENGINE)return (float)activeEngine;
    return index < P_COUNT ? params[ index ] : 0.0f;
}

char* VJGenerative::GetParameterDisplay(unsigned int index)
{
   if(index==P_ENGINE)return const_cast<char*>(kMotorNames[activeEngine]);
   if(index==P_ORIENTATION){std::snprintf(display,sizeof(display),"%.0f graus",360.f*params[index]);return display;}
   if(index==P_LOOP){std::snprintf(display,sizeof(display),"%.1f s",4.f+116.f*params[index]);return display;}
   if(index==P_CUSTOM_W || index==P_CUSTOM_H){
      const unsigned px=(unsigned)std::lround(128.f+8064.f*params[index]);
      std::snprintf(display,sizeof(display),"%u px",px);
      return display;
   }
   if(index==P_GPU_TEST){
      return (char*)(expansionReady&&story.isReady()?"GLSL OK":"GLSL ERRO");
   }
   return CFFGLPlugin::GetParameterDisplay(index);
}

void VJGenerative::syncControlVisibility(bool notify){
 SetParamVisibility(P_QUALITY,activeEngine>=7,notify);SetParamVisibility(P_COLOR_SOURCE,activeEngine>=7,notify);
 for(unsigned i=P_TEXT;i<=P_TEXT_LAYOUT;i++)SetParamVisibility(i,activeEngine==21,notify);
 SetParamVisibility(P_CUSTOM_W,params[P_CANVAS]>=3.5f,notify);SetParamVisibility(P_CUSTOM_H,params[P_CANVAS]>=3.5f,notify);
 SetParamVisibility(P_GPU_TEST,false,notify);
}

FFResult VJGenerative::SetTextParameter(unsigned int index,const char* value){
 if(index!=P_TEXT||!value)return FF_FAIL;std::string incoming(value);if(incoming.size()>4096)return FF_FAIL;editableText=std::move(incoming);return FF_SUCCESS;
}
char* VJGenerative::GetTextParameter(unsigned int index){return index==P_TEXT?const_cast<char*>(editableText.c_str()):nullptr;}

static CFFGLPluginInfo PluginInfo(
	PluginFactory< VJGenerative >,
	"VJ82",
	"VJ Gen V1.7 82",
	2, 1,
	1, 7,
	FF_SOURCE,
	"Fundos generativos ao vivo",
	"82 engines; native button rows; 36 palettes; RGBA; editable UTF-8 text" );