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
	P_COUNT
};

static const char* kEngines[]  = { "Nevoa", "Tunel", "Lasers", "Celulas", "Aurora", "Orbes" };
static const char* kPalettes[] = { "Arco-iris", "Fogo", "Oceano", "Neon", "Ultravioleta", "Floresta", "Gelo", "Por do sol", "Mono" };
static const int kNumEngines   = sizeof( kEngines ) / sizeof( kEngines[ 0 ] );
static const int kNumPalettes  = sizeof( kPalettes ) / sizeof( kPalettes[ 0 ] );

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

	quad.Draw();
	return FF_SUCCESS;
}

FFResult VJGenerative::SetFloatParameter( unsigned int index, float value )
{
	if( index >= P_COUNT ) return FF_FAIL;
	params[ index ] = value;
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
	1, 0,
	FF_SOURCE,
	"Fundos generativos ao vivo",
	"Plugin FFGL generativo - 6 motores, 9 paletas" );