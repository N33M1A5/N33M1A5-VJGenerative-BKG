#include "FFGLSDK.h"
#include "shaders.h"
#include <chrono>
#include <cmath>

using namespace ffglex;

enum Param
{
	P_ENGINE,
	P_PALETTE,
	P_SPEED,
	P_SCALE,
	P_INTENSITY,
	P_HUE,
	P_SYMMETRY,
	P_SEED,
	P_BPM,
	P_PULSE,
	P_CONTRAST,
    // V1.1: os 11 primeiros parametros nao mudam de posicao.
    P_COLOR_MODE, P_R1, P_G1, P_B1, P_W1,
    P_R2, P_G2, P_B2, P_W2, P_RGBW_MIX, P_RGBW_BLEND,
    P_OPACITY,
    P_CANVAS_PRESET, P_CANVAS_FIT, P_CUSTOM_W, P_CUSTOM_H,
    P_POS_X, P_POS_Y, P_TRANSFORM_SCALE, P_ROTATION, P_ANCHOR_X, P_ANCHOR_Y,
    P_VJ_DETAIL, P_VJ_DEPTH, P_VJ_DENSITY,
	P_COUNT
};

static const char* kEngines[]  = { "Nevoa", "Tunel", "Lasers", "Celulas", "Aurora", "Orbes",
    "Crystal Kaleido", "Particle Waves", "Light Curtains", "Cyber Grid", "Solid RGBW" };
static const char* kPalettes[] = { "Arco-iris", "Fogo", "Oceano", "Neon", "Ultravioleta", "Floresta", "Gelo", "Por do sol", "Mono" };
static const int kNumEngines   = sizeof( kEngines ) / sizeof( kEngines[ 0 ] );
static const int kNumPalettes  = sizeof( kPalettes ) / sizeof( kPalettes[ 0 ] );
static const char* kColorModes[] = {"Original", "RGBW A", "RGBW A+B", "RGBW Animated"};
static const char* kRGBWBlends[] = {"Crossfade", "Add", "Screen", "Multiply"};
static const char* kCanvasPresets[] = {"Auto / Output", "16:9", "9:16", "4:3", "32:9", "Custom"};
static const char* kCanvasFits[] = {"Adaptive", "Fit", "Fill", "Stretch"};

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

private:
	FFGLShader shader;
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
    // RGBW: 2 conjuntos independentes, cada um com branco W.
    SetOptionParamInfo(P_COLOR_MODE, "Color Mode", 4, 0.0f);
    for(int i=0;i<4;i++) SetParamElementInfo(P_COLOR_MODE,i,kColorModes[i],(float)i);
    SetOptionParamInfo(P_RGBW_BLEND, "RGBW Combine", 4, 0.0f);
    for(int i=0;i<4;i++) SetParamElementInfo(P_RGBW_BLEND,i,kRGBWBlends[i],(float)i);
    SetParamInfof(P_R1,"A - Red",FF_TYPE_RED);
    SetParamInfof(P_G1,"A - Green",FF_TYPE_GREEN);
    SetParamInfof(P_B1,"A - Blue",FF_TYPE_BLUE);
    SetParamInfof(P_W1,"A - White",FF_TYPE_STANDARD);
    SetParamInfof(P_R2,"B - Red",FF_TYPE_RED);
    SetParamInfof(P_G2,"B - Green",FF_TYPE_GREEN);
    SetParamInfof(P_B2,"B - Blue",FF_TYPE_BLUE);
    SetParamInfof(P_W2,"B - White",FF_TYPE_STANDARD);
    SetParamInfof(P_RGBW_MIX,"RGBW Mix A/B",FF_TYPE_STANDARD);
    SetParamInfof(P_OPACITY,"Source Opacity",FF_TYPE_STANDARD);
    for(unsigned i=P_COLOR_MODE;i<=P_OPACITY;i++) SetParamGroup(i,"COLOR / RGBW");
    params[P_COLOR_MODE]=0; // ORIGINAL: garante visual identico dos motores 0-5.
    params[P_R1]=1; params[P_G1]=0; params[P_B1]=0; params[P_W1]=0;
    params[P_R2]=0; params[P_G2]=0; params[P_B2]=1; params[P_W2]=0;
    params[P_RGBW_MIX]=.5f; params[P_RGBW_BLEND]=0; params[P_OPACITY]=1;

    SetOptionParamInfo(P_CANVAS_PRESET,"Canvas Aspect",6,0);
    for(int i=0;i<6;i++) SetParamElementInfo(P_CANVAS_PRESET,i,kCanvasPresets[i],(float)i);
    SetOptionParamInfo(P_CANVAS_FIT,"Canvas Fit",4,0);
    for(int i=0;i<4;i++) SetParamElementInfo(P_CANVAS_FIT,i,kCanvasFits[i],(float)i);
    SetParamInfof(P_CUSTOM_W,"Custom W 256-8192",FF_TYPE_STANDARD);
    SetParamInfof(P_CUSTOM_H,"Custom H 256-8192",FF_TYPE_STANDARD);
    for(unsigned i=P_CANVAS_PRESET;i<=P_CUSTOM_H;i++) SetParamGroup(i,"CANVAS / RESOLUTION");
    params[P_CANVAS_PRESET]=0; params[P_CANVAS_FIT]=0;
    params[P_CUSTOM_W]=(1920.f-256.f)/7936.f; params[P_CUSTOM_H]=(1080.f-256.f)/7936.f;

    SetParamInfof(P_POS_X,"Position X",FF_TYPE_STANDARD);
    SetParamInfof(P_POS_Y,"Position Y",FF_TYPE_STANDARD);
    SetParamInfof(P_TRANSFORM_SCALE,"Transform Scale",FF_TYPE_STANDARD);
    SetParamInfof(P_ROTATION,"Rotation",FF_TYPE_STANDARD);
    SetParamInfof(P_ANCHOR_X,"Anchor X",FF_TYPE_STANDARD);
    SetParamInfof(P_ANCHOR_Y,"Anchor Y",FF_TYPE_STANDARD);
    for(unsigned i=P_POS_X;i<=P_ANCHOR_Y;i++) SetParamGroup(i,"TRANSFORM");
    params[P_POS_X]=.5f; params[P_POS_Y]=.5f; params[P_TRANSFORM_SCALE]=.5f;
    params[P_ROTATION]=.5f; params[P_ANCHOR_X]=.5f; params[P_ANCHOR_Y]=.5f;

    SetParamInfof(P_VJ_DETAIL,"VJ Detail",FF_TYPE_STANDARD);
    SetParamInfof(P_VJ_DEPTH,"VJ Depth",FF_TYPE_STANDARD);
    SetParamInfof(P_VJ_DENSITY,"VJ Density",FF_TYPE_STANDARD);
    for(unsigned i=P_VJ_DETAIL;i<=P_VJ_DENSITY;i++) SetParamGroup(i,"VJ PRO");
    params[P_VJ_DETAIL]=.5f; params[P_VJ_DEPTH]=.5f; params[P_VJ_DENSITY]=.5f;
}

FFResult VJGenerative::InitGL( const FFGLViewportStruct* vp )
{
	if( !shader.Compile( kVertexShader, kFragmentShader ) )
	{
		DeInitGL();
		return FF_FAIL;
	}
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

	ScopedShaderBinding binding( shader.GetGLID() );
	shader.Set( "iResolution", (float)vp[ 2 ], (float)vp[ 3 ], 1.0f );
	shader.Set( "iTime", (float)time );
	shader.Set( "uBeat", (float)beats );
	shader.Set( "uEngine", params[ P_ENGINE ] );
	shader.Set( "uPalette", params[ P_PALETTE ] );
	shader.Set( "uScale", params[ P_SCALE ] );
	shader.Set( "uIntensity", params[ P_INTENSITY ] );
	shader.Set( "uHue", params[ P_HUE ] );
	shader.Set( "uSlices", params[ P_SYMMETRY ] );
	shader.Set( "uSeed", params[ P_SEED ] * 100.0f );
	shader.Set( "uPulse", params[ P_PULSE ] );
	shader.Set( "uContrast", params[ P_CONTRAST ] );
    shader.Set("uColorMode",params[P_COLOR_MODE]);
    shader.Set("uColorA",params[P_R1],params[P_G1],params[P_B1],params[P_W1]);
    shader.Set("uColorB",params[P_R2],params[P_G2],params[P_B2],params[P_W2]);
    shader.Set("uRGBWMix",params[P_RGBW_MIX]);
    shader.Set("uRGBWBlend",params[P_RGBW_BLEND]);
    shader.Set("uOpacity",params[P_OPACITY]);
    shader.Set("uCanvasPreset",params[P_CANVAS_PRESET]);
    shader.Set("uCanvasFit",params[P_CANVAS_FIT]);
    shader.Set("uCustomCanvas",256.f+7936.f*params[P_CUSTOM_W],256.f+7936.f*params[P_CUSTOM_H]);
    shader.Set("uPosition",params[P_POS_X],params[P_POS_Y]);
    shader.Set("uTransformScale",params[P_TRANSFORM_SCALE]);
    shader.Set("uRotation",params[P_ROTATION]);
    shader.Set("uAnchor",params[P_ANCHOR_X],params[P_ANCHOR_Y]);
    shader.Set("uVJDetail",params[P_VJ_DETAIL]);
    shader.Set("uVJDepth",params[P_VJ_DEPTH]);
    shader.Set("uVJDensity",params[P_VJ_DENSITY]);

	quad.Draw();
	return FF_SUCCESS;
}

FFResult VJGenerative::SetFloatParameter( unsigned int index, float value )
{
	if( index >= P_COUNT ) return FF_FAIL;
	params[ index ] = std::fmax(0.0f,std::fmin(1.0f,value));
    // FFGL option values are discrete IDs rather than 0-1 normalized numbers.
    if(index==P_ENGINE) params[index]=std::fmax(0.f,std::fmin((float)(kNumEngines-1),value));
    else if(index==P_PALETTE) params[index]=std::fmax(0.f,std::fmin((float)(kNumPalettes-1),value));
    else if(index==P_COLOR_MODE) params[index]=std::fmax(0.f,std::fmin(3.f,value));
    else if(index==P_RGBW_BLEND) params[index]=std::fmax(0.f,std::fmin(3.f,value));
    else if(index==P_CANVAS_PRESET) params[index]=std::fmax(0.f,std::fmin(5.f,value));
    else if(index==P_CANVAS_FIT) params[index]=std::fmax(0.f,std::fmin(3.f,value));
	return FF_SUCCESS;
}

float VJGenerative::GetFloatParameter( unsigned int index )
{
	return index < P_COUNT ? params[ index ] : 0.0f;
}

static CFFGLPluginInfo PluginInfo(
	PluginFactory< VJGenerative >,
	"VJGN",
	"VJ Generative",
	2, 1,
	1, 1,
	FF_SOURCE,
	"Fundos generativos ao vivo",
	"V1.1 RGBW + custom canvas + 11 engines. Original 6 preserved" );