#include "FFGLSDK.h"
#include "shaders.h" // V1.0.1 renderer, exact and unmodified
#include "shaders_expansion.h"
#include <cstdio>
#include <algorithm>
#include <chrono>
#include <cmath>

using namespace ffglex;

enum Param
{
	P_ENGINE,
	P_PALETTE,
    // Color section directly below the Palette row in the FFGL host UI.
    P_RGBA_MODE, P_RGBA_BLEND, P_RGBA_MIX,
    P_A_R, P_A_G, P_A_B, P_A_ALPHA,
    P_B_R, P_B_G, P_B_B, P_B_ALPHA,
    P_OPACITY,
    // Original six visuals and nine controls keep their names and defaults.
	P_SPEED,
	P_SCALE,
	P_INTENSITY,
	P_HUE,
	P_SYMMETRY,
	P_SEED,
	P_BPM,
	P_PULSE,
	P_CONTRAST,
    // Additional engine rows remain separate.
    P_GEOMETRY, P_LIGHT, P_DEPTH_ENGINES,
    P_DETAIL, P_VJ_DEPTH, P_DENSITY,
    P_ASPECT_MODE, P_CUSTOM_WIDTH, P_CUSTOM_HEIGHT,
	P_COUNT
};

static const char* kEngines[]  = { "Nevoa", "Tunel", "Lasers", "Celulas", "Aurora", "Orbes" };
static const char* kPalettes[] = { "Arco-iris", "Fogo", "Oceano", "Neon", "Ultravioleta", "Floresta", "Gelo", "Por do sol", "Mono" };
static const int kNumEngines   = sizeof( kEngines ) / sizeof( kEngines[ 0 ] );
static const int kNumPalettes  = sizeof( kPalettes ) / sizeof( kPalettes[ 0 ] );
static const char* kGeometry[]={"Classico V1","Crystal","Hex Mosaic","Mandala","Orbital Rings","Polygon Prism","Fractal Bloom"};
static const char* kLighting[]={"Classico V1","Light Curtains","Stage Beams","Particle Waves","Bokeh Lights","Liquid Plasma","Neon Ribbons"};
static const char* kDepth[]={"Classico V1","Cyber Grid","Kaleido Pulse","Starfield Warp","Circuit Board","Digital Rain","Solid RGBA"};
static const int kGeometryIds[]={-1,0,6,7,8,9,10};
static const int kLightingIds[]={-1,2,11,1,12,13,14};
static const int kDepthIds[]={-1,3,4,15,16,17,5};
static const char* kRGBAChoices[]={"Original Palette","RGBA A","RGBA A+B","RGBA Animated"};
static const char* kRGBABlends[]={"Crossfade","Add","Screen","Multiply"};
static const char* kCanvasModes[]={"Auto / Output","16:9","9:16","32:9","Custom"};

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

private:
	FFGLShader shader; // Official original V1.0.1 renderer
    FFGLShader expansionShader;
    bool expansionReady = false;
    int activeBank = 0; // 0=V1; 1=Geometry; 2=Light; 3=Depth
    char displayBuffer[32]={};
	FFGLScreenQuad quad;
	float params[ P_COUNT ];
	double time       = 0.0;  // tempo acumulado (afetado pela Speed)
	double beats      = 0.0;  // batidas acumuladas (tempo real x BPM)
	std::chrono::steady_clock::time_point last;
	bool started = false;
};

VJGenerative::VJGenerative()
{
	SetMinInputs( 0 );
	SetMaxInputs( 0 );

	SetOptionParamInfo( P_ENGINE, "Motor", kNumEngines, 0.0f );
	for( int i = 0; i < kNumEngines; i++ )
		SetParamElementInfo( P_ENGINE, i, kEngines[ i ], (float)i );

	SetOptionParamInfo( P_PALETTE, "Paleta", kNumPalettes, 0.0f );
	for( int i = 0; i < kNumPalettes; i++ )
		SetParamElementInfo( P_PALETTE, i, kPalettes[ i ], (float)i );

    // Native color controls are intentionally registered IMMEDIATELY after Paleta.
    SetOptionParamInfo(P_RGBA_MODE,"Cor / RGBA",4,0.f);
    SetOptionParamInfo(P_RGBA_BLEND,"Mistura RGBA",4,0.f);
    for(int i=0;i<4;++i){
       SetParamElementInfo(P_RGBA_MODE,i,kRGBAChoices[i],(float)i);
       SetParamElementInfo(P_RGBA_BLEND,i,kRGBABlends[i],(float)i);
    }
    SetParamInfof(P_RGBA_MIX,"Mistura A/B",FF_TYPE_STANDARD);
    SetParamInfof(P_A_R,"A Red",FF_TYPE_RED);
    SetParamInfof(P_A_G,"A Green",FF_TYPE_GREEN);
    SetParamInfof(P_A_B,"A Blue",FF_TYPE_BLUE);
    SetParamInfof(P_A_ALPHA,"A Alpha",FF_TYPE_ALPHA);
    SetParamInfof(P_B_R,"B Red",FF_TYPE_RED);
    SetParamInfof(P_B_G,"B Green",FF_TYPE_GREEN);
    SetParamInfof(P_B_B,"B Blue",FF_TYPE_BLUE);
    SetParamInfof(P_B_ALPHA,"B Alpha",FF_TYPE_ALPHA);
    SetParamInfof(P_OPACITY,"Opacidade",FF_TYPE_ALPHA);
    for(unsigned i=P_RGBA_MODE;i<=P_OPACITY;i++)SetParamGroup(i,"CORES | RGBA");
    params[P_RGBA_MODE]=params[P_RGBA_BLEND]=0.f; params[P_RGBA_MIX]=.5f;
    params[P_A_R]=1.f;params[P_A_G]=params[P_A_B]=0.f;params[P_A_ALPHA]=1.f;
    params[P_B_R]=params[P_B_G]=0.f;params[P_B_B]=1.f;params[P_B_ALPHA]=1.f;
    params[P_OPACITY]=1.f;


	SetParamInfof( P_SPEED, "Velocidade", FF_TYPE_STANDARD );
	SetParamInfof( P_SCALE, "Escala", FF_TYPE_STANDARD );
	SetParamInfof( P_INTENSITY, "Intensidade", FF_TYPE_STANDARD );
	SetParamInfof( P_HUE, "Matiz", FF_TYPE_STANDARD );
	SetParamInfof( P_SYMMETRY, "Simetria", FF_TYPE_STANDARD );
	SetParamInfof( P_SEED, "Semente", FF_TYPE_STANDARD );
	SetParamInfof( P_BPM, "BPM (60-180)", FF_TYPE_STANDARD );
	SetParamInfof( P_PULSE, "Pulso", FF_TYPE_STANDARD );
	SetParamInfof( P_CONTRAST, "Contraste", FF_TYPE_STANDARD );

	params[ P_ENGINE ]    = 0.0f;
	params[ P_PALETTE ]   = 0.0f;
	params[ P_SPEED ]     = 0.5f;
	params[ P_SCALE ]     = 0.5f;
	params[ P_INTENSITY ] = 0.5f;
	params[ P_HUE ]       = 0.0f;
	params[ P_SYMMETRY ]  = 0.0f;
	params[ P_SEED ]      = 0.0f;
	params[ P_BPM ]       = 0.5f;
	params[ P_PULSE ]     = 0.0f;
	params[ P_CONTRAST ]  = 0.5f;

    // Separate button rows - the original six remain in Motor.
    SetOptionParamInfo(P_GEOMETRY,"MOTORES GEOMETRIA",7,0.f);
    SetOptionParamInfo(P_LIGHT,"MOTORES LUZ",7,0.f);
    SetOptionParamInfo(P_DEPTH_ENGINES,"MOTORES 3D",7,0.f);
    for(int i=0;i<7;++i){
        SetParamElementInfo(P_GEOMETRY,i,kGeometry[i],(float)i);
        SetParamElementInfo(P_LIGHT,i,kLighting[i],(float)i);
        SetParamElementInfo(P_DEPTH_ENGINES,i,kDepth[i],(float)i);
    }
    for(unsigned i=P_GEOMETRY;i<=P_DEPTH_ENGINES;i++)SetParamGroup(i,"VJ EXPANSION | MOTORES");
    params[P_GEOMETRY]=params[P_LIGHT]=params[P_DEPTH_ENGINES]=0.f;

    SetParamInfof(P_DETAIL,"Detail",FF_TYPE_STANDARD);
    SetParamInfof(P_VJ_DEPTH,"Depth",FF_TYPE_STANDARD);
    SetParamInfof(P_DENSITY,"Density",FF_TYPE_STANDARD);
    for(unsigned i=P_DETAIL;i<=P_DENSITY;i++)SetParamGroup(i,"VJ EXPANSION | AJUSTES");
    params[P_DETAIL]=params[P_VJ_DEPTH]=params[P_DENSITY]=.5f;

    SetOptionParamInfo(P_ASPECT_MODE,"Canvas",5,0.f);
    for(int i=0;i<5;i++)SetParamElementInfo(P_ASPECT_MODE,i,kCanvasModes[i],(float)i);
    SetParamInfof(P_CUSTOM_WIDTH,"Canvas Width",FF_TYPE_STANDARD);
    SetParamInfof(P_CUSTOM_HEIGHT,"Canvas Height",FF_TYPE_STANDARD);
    for(unsigned i=P_ASPECT_MODE;i<=P_CUSTOM_HEIGHT;i++)SetParamGroup(i,"VJ EXPANSION | LED CANVAS");
    params[P_ASPECT_MODE]=0.f;
    params[P_CUSTOM_WIDTH]=(1920.f-128.f)/8064.f;
    params[P_CUSTOM_HEIGHT]=(1080.f-128.f)/8064.f;
}

FFResult VJGenerative::InitGL( const FFGLViewportStruct* vp )
{
	if( !shader.Compile( kVertexShader, kFragmentShader ) )
	{
		DeInitGL();
		return FF_FAIL;
	}
// The V1.0.1 shader is mandatory; expansion is optional and cannot break original.
    expansionReady=expansionShader.Compile(kVJVertexShader,kVJFragmentShader);
    if(!expansionReady)FFGLLog::LogToHost("VJ Expansion GLSL could not compile; V1.0.1 remains available");
    if( !quad.Initialise() )
	{
		DeInitGL();
		return FF_FAIL;
	}
	started = false;
	return CFFGLPlugin::InitGL( vp );
}

FFResult VJGenerative::DeInitGL()
{
	shader.FreeGLResources();
    expansionShader.FreeGLResources();
    expansionReady=false;
	quad.Release();
	return FF_SUCCESS;
}

FFResult VJGenerative::ProcessOpenGL( ProcessOpenGLStruct* pGL )
{
	auto now = std::chrono::steady_clock::now();
	if( !started )
	{
		last    = now;
		started = true;
	}
	double dt = std::chrono::duration< double >( now - last ).count();
	last      = now;
	if( dt > 0.25 ) dt = 0.25;

	// Velocidade: 0..1 -> 0..2x (0.5 = normal)
	double speed = params[ P_SPEED ] * 2.0;
	time += dt * speed;
	double bpm = 60.0 + params[ P_BPM ] * 120.0;
	beats += dt * bpm / 60.0;

	GLint vp[ 4 ];
	glGetIntegerv( GL_VIEWPORT, vp );

    const bool useExpansion=activeBank!=0 && expansionReady;
    FFGLShader& active=useExpansion?expansionShader:shader;
    ScopedShaderBinding binding(active.GetGLID());
    active.Set("iResolution",(float)vp[2],(float)vp[3],1.0f);
    active.Set("iTime",(float)time);
    active.Set("uBeat",(float)beats);
    active.Set("uEngine",params[P_ENGINE]);
    active.Set("uPalette",params[P_PALETTE]);
    active.Set("uScale",params[P_SCALE]);
    active.Set("uIntensity",params[P_INTENSITY]);
    active.Set("uHue",params[P_HUE]);
    active.Set("uSlices",params[P_SYMMETRY]);
    active.Set("uSeed",params[P_SEED]*100.f);
    active.Set("uPulse",params[P_PULSE]);
    active.Set("uContrast",params[P_CONTRAST]);
    if(useExpansion){
      int sel=(int)params[activeBank==1?P_GEOMETRY:activeBank==2?P_LIGHT:P_DEPTH_ENGINES];
      sel=std::max(1,std::min(6,sel));
      int engine=activeBank==1?kGeometryIds[sel]:(activeBank==2?kLightingIds[sel]:kDepthIds[sel]);
      active.Set("uVJEngine",(float)engine);
      active.Set("uDetail",params[P_DETAIL]);
      active.Set("uDepth",params[P_VJ_DEPTH]);
      active.Set("uDensity",params[P_DENSITY]);
      active.Set("uRGBAType",params[P_RGBA_MODE]);
      active.Set("uRGBABlend",params[P_RGBA_BLEND]);
      active.Set("uRGBAMix",params[P_RGBA_MIX]);
      active.Set("uRGBA_A",params[P_A_R],params[P_A_G],params[P_A_B],params[P_A_ALPHA]);
      active.Set("uRGBA_B",params[P_B_R],params[P_B_G],params[P_B_B],params[P_B_ALPHA]);
      active.Set("uOpacity",params[P_OPACITY]);
      active.Set("uCanvasMode",params[P_ASPECT_MODE]);
      active.Set("uCustomSize",128.f+8064.f*params[P_CUSTOM_WIDTH],128.f+8064.f*params[P_CUSTOM_HEIGHT]);
    }

	quad.Draw();
	return FF_SUCCESS;
}

FFResult VJGenerative::SetFloatParameter( unsigned int index, float value )
{
	if( index >= P_COUNT ) return FF_FAIL;
    // Original 11 IDs, parameter meaning and defaults are preserved.
    if(index==P_ENGINE || index==P_PALETTE || (index>=P_SPEED && index<=P_CONTRAST)) {
       if(index==P_ENGINE)activeBank=0;
       params[index]=value;
    } else {
       if(!std::isfinite(value))return FF_FAIL;
       if(index>=P_GEOMETRY && index<=P_DEPTH_ENGINES){
          value=std::max(0.f,std::min(6.f,value));
          params[index]=value;
          int bank=(int)(index-P_GEOMETRY)+1;
          if(value>=1.f){activeBank=bank;
             for(unsigned i=P_GEOMETRY;i<=P_DEPTH_ENGINES;i++)if(i!=index)params[i]=0.f;
          }else if(activeBank==bank){activeBank=0;}
       }else if(index==P_RGBA_MODE || index==P_RGBA_BLEND || index==P_ASPECT_MODE){
           params[index]=std::max(0.f,std::min(index==P_ASPECT_MODE?4.f:3.f,value));
       }else params[index]=std::max(0.f,std::min(1.f,value));
    }
	return FF_SUCCESS;
}

float VJGenerative::GetFloatParameter( unsigned int index )
{
	return index < P_COUNT ? params[ index ] : 0.0f;
}

char* VJGenerative::GetParameterDisplay(unsigned int index)
{
    if(index==P_CUSTOM_WIDTH||index==P_CUSTOM_HEIGHT){
       std::snprintf(displayBuffer,sizeof(displayBuffer),"%u px",
                     (unsigned)std::lround(128.f+8064.f*params[index]));
       return displayBuffer;
    }
    return CFFGLPlugin::GetParameterDisplay(index);
}

static CFFGLPluginInfo PluginInfo(
	PluginFactory< VJGenerative >,
	"VJ13",
	"VJ Expanded 121",
	2, 1,
	1, 3,
	FF_SOURCE,
	"Fundos generativos ao vivo",
	"V1.2: 6 legacy motors preserved plus 18 new curated generators" );