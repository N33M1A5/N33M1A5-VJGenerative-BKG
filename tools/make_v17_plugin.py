from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
s=(ROOT/'VJGenerative.cpp').read_text().replace('#include "palettes_extra.h"','#include "palettes_extra.h"\n#include "StoryPipeline.h"')
# Load the complete Windows text APIs before FFGL suppresses their declarations.
s=s.replace('#include "FFGLSDK.h"','#include "TextAtlas.h" // Windows text APIs must precede FFGL\n#include "FFGLSDK.h"',1)
s=s.replace('P_PALETTE,','P_MOTOR_ROW_FIRST, P_MOTOR_ROW_LAST=P_MOTOR_ROW_FIRST+11,\n\tP_PALETTE,',1)
s=s.replace('P_COUNT\n','P_TEXT, P_TEXT_SIZE, P_TEXT_X, P_TEXT_Y, P_TEXT_LAYOUT,\n    P_LOOP, P_QUALITY, P_COLOR_SOURCE, P_ACTIVE_ENGINE,\n\tP_COUNT\n')
s=s.replace('    char display[32] = {};','    char display[96] = {};\n    StoryPipeline story;\n    int activeEngine=0;\n    std::string editableText="JESUS\\nREI DOS REIS\\nSENHOR DOS SENHORES";')
s=s.replace('char* GetParameterDisplay(unsigned int index) override;','''char* GetParameterDisplay(unsigned int index) override;
    FFResult SetTextParameter(unsigned int index,const char* value) override;
    char* GetTextParameter(unsigned int index) override;''')
s=s.replace('    params[P_CANVAS]=0.f;','''    params[P_LOOP]=16.f/116.f;params[P_QUALITY]=1.f;params[P_COLOR_SOURCE]=0.f;
    params[P_TEXT_SIZE]=.5f;params[P_TEXT_X]=.5f;params[P_TEXT_Y]=.5f;params[P_TEXT_LAYOUT]=1.f;
    params[P_ACTIVE_ENGINE]=0.f;
    params[P_CANVAS]=0.f;''')
a=s.index('    // One short FFGL OPTION row');b=s.index('    SetOptionParamInfo(P_PALETTE',a)
s=s[:a]+'''    // Unified motor area: native FFGL option rows, all grouped together like V1.0.
    for(int row=0;row<kMotorRows;row++){
       unsigned p=row==0?P_ENGINE:(row==1?P_EXTRA_MOTOR:P_MOTOR_ROW_FIRST+row-2);
       int first=row*6, count=std::min(6,kTotalEngines-first);
       SetOptionParamInfo(p,"Motores",count,0.f);
       for(int j=0;j<count;j++){
          int id=kMotorUIOrder[first+j];
          SetParamElementInfo(p,j,kMotorLabels[id],(float)id);
       }
       SetParamGroup(p,"MOTORES");
    }

'''+s[b:]
needle='    SetParamInfof(P_GPU_TEST,"Teste de Shader GPU",FF_TYPE_BOOLEAN);'
s=s.replace(needle,'''    SetParamInfo(P_TEXT,"Texto Editavel",FF_TYPE_TEXT,editableText.c_str());
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
'''+needle)
s=s.replace('    started=false;\n    return CFFGLPlugin::InitGL(vp);','    if(!story.init()){DeInitGL();return FF_FAIL;}\n    started=false;\n    return CFFGLPlugin::InitGL(vp);')
s=s.replace('    shader.FreeGLResources();','    story.release();\n    shader.FreeGLResources();')
a=s.index('    int motor=(int)std::lround(params[P_ENGINE]);');b=s.index('    const int paletteIndex=',a)
s=s[:a]+'''    int motor=std::max(0,std::min(5,activeEngine));
    const bool particleWave=activeEngine==6;
    const bool classic=activeEngine<6;
'''+s[b:]
needle='    const bool originalLook=params[P_COLOR_MODE]<.5f'
s=s.replace(needle,'''    if(activeEngine>=7){
        StoryControls c{};c.time=(float)time;c.beat=(float)beats;c.width=width;c.height=height;
        c.scale=params[P_SCALE];c.intensity=params[P_INTENSITY];c.hue=params[P_HUE];c.symmetry=params[P_SYMMETRY];c.seed=params[P_SEED]*100.f;c.pulse=params[P_PULSE];
        c.detail=params[P_DETAIL];c.depth=params[P_DEPTH];c.density=params[P_DENSITY];c.glow=params[P_GLOW];c.opacity=params[P_OPACITY];c.contrast=params[P_CONTRAST];
        c.period=4.f+116.f*params[P_LOOP];c.quality=params[P_QUALITY];c.colorSource=params[P_COLOR_SOURCE];c.palette=(float)paletteId;
        c.colorMode=params[P_COLOR_MODE];c.colorBlend=params[P_COLOR_BLEND];c.colorMix=params[P_COLOR_MIX];
        for(int i=0;i<4;i++){c.a[i]=params[P_A_R+i];c.b[i]=params[P_B_R+i];}
        c.text=editableText.c_str();c.textSize=params[P_TEXT_SIZE];c.textX=(params[P_TEXT_X]-.5f)*3.f;c.textY=(params[P_TEXT_Y]-.5f)*2.f;c.textLayout=params[P_TEXT_LAYOUT];
        return story.render(activeEngine-7,c)?FF_SUCCESS:FF_FAIL;
    }
'''+needle)
s=s.replace('active->Set("uEngine",params[P_ENGINE]);','active->Set("uEngine",(float)motor);')
a=s.index('    if(index==P_ENGINE){',s.index('FFResult VJGenerative::SetFloatParameter'));b=s.index('    else if(index==P_COLOR_MODE',a)
s=s[:a]+'''    int row=index==P_ENGINE?0:(index==P_EXTRA_MOTOR?1:((index>=P_MOTOR_ROW_FIRST&&index<=P_MOTOR_ROW_LAST)?(int)(index-P_MOTOR_ROW_FIRST+2):-1));
    if(row>=0){
       int requested=(int)std::lround(value);
       for(int j=row*6;j<std::min(kTotalEngines,row*6+6);j++)if(kMotorUIOrder[j]==requested){activeEngine=requested;params[P_ACTIVE_ENGINE]=(float)activeEngine/(kTotalEngines-1);break;}
       return FF_SUCCESS;
    }
    if(index==P_ACTIVE_ENGINE){activeEngine=(int)std::lround(std::max(0.f,std::min(1.f,value))*(kTotalEngines-1));params[index]=(float)activeEngine/(kTotalEngines-1);return FF_SUCCESS;}
    if(index==P_QUALITY)params[index]=std::round(std::max(0.f,std::min(2.f,value)));
    else if(index==P_PALETTE){float next=std::max(0.f,std::min((float)(kNumPalettes-1),value));if(next!=params[index])params[P_COLOR_SOURCE]=1.f;params[index]=next;}
'''+s[b:]
s=s.replace('return index < P_COUNT ? params[ index ] : 0.0f;','''if(index==P_ENGINE||index==P_EXTRA_MOTOR||(index>=P_MOTOR_ROW_FIRST&&index<=P_MOTOR_ROW_LAST))return (float)activeEngine;
    return index < P_COUNT ? params[ index ] : 0.0f;''')
s=s.replace('   if(index==P_CUSTOM_W || index==P_CUSTOM_H){','''   if(index==P_ENGINE||index==P_EXTRA_MOTOR||(index>=P_MOTOR_ROW_FIRST&&index<=P_MOTOR_ROW_LAST))return const_cast<char*>(kMotorNames[activeEngine]);
   if(index==P_LOOP){std::snprintf(display,sizeof(display),"%.1f s",4.f+116.f*params[index]);return display;}
   if(index==P_CUSTOM_W || index==P_CUSTOM_H){''')
s=s.replace('(expansionReady?"GPU OK":"GPU ERRO")','(expansionReady&&story.isReady()?"GLSL OK":"GLSL ERRO")')
needle='static CFFGLPluginInfo PluginInfo('
s=s.replace(needle,'''FFResult VJGenerative::SetTextParameter(unsigned int index,const char* value){
 if(index!=P_TEXT||!value)return FF_FAIL;std::string incoming(value);if(incoming.size()>4096)return FF_FAIL;editableText=std::move(incoming);return FF_SUCCESS;
}
char* VJGenerative::GetTextParameter(unsigned int index){return index==P_TEXT?const_cast<char*>(editableText.c_str()):nullptr;}

'''+needle)
s=s.replace('"VJ17"','"VJ82"').replace('"VJ Gen V1.4.1"','"VJ Gen V1.7 82"').replace('\t1, 7,','\t1, 7,').replace('"V1.4.1: original 6 motor buttons, Particle Wave button row, 36 sorted palettes"','"82 engines; native button rows; 36 palettes; RGBA; editable UTF-8 text"')
s=s.replace('ALL 24 engines','all 82 engines').replace('class VJGenerative','class VJGenerative')
# Shared adjustment controls and deterministic clock seam for the native test host.
s=s.replace('P_DETAIL, P_DEPTH, P_DENSITY, P_DIRECTION','P_DETAIL, P_DEPTH, P_DENSITY, P_ORIENTATION, P_DIRECTION')
s=s.replace('    SetParamInfof(P_DIRECTION,"Inverter Movimento",FF_TYPE_BOOLEAN);','    SetParamInfof(P_ORIENTATION,"Direcao 0-360",FF_TYPE_STANDARD);\n    SetParamInfof(P_DIRECTION,"Inverter Movimento",FF_TYPE_BOOLEAN);')
s=s.replace('    SetParamGroup(P_GPU_TEST,"DIAGNOSTICO");','    SetParamGroup(P_GPU_TEST,"DIAGNOSTICO");\n    // Final compact operator layout: Motors -> Palettes -> RGBA -> General.\n    SetParamGroup(P_ENGINE,"MOTORES");\n    SetParamGroup(P_PALETTE,"PALETAS");\n    for(unsigned i=P_COLOR_MODE;i<=P_B_ALPHA;i++)SetParamGroup(i,"CONTROLE RGBA");\n    for(unsigned i=P_SPEED;i<P_ACTIVE_ENGINE;i++)SetParamGroup(i,"CONTROLES GERAIS");\n    syncControlVisibility(false);')
s=s.replace('    int activeEngine=0;','    int activeEngine=0;\n    std::chrono::steady_clock::time_point (*clockNow)()=&std::chrono::steady_clock::now;\n    void syncControlVisibility(bool notify);')
s=s.replace('private:\n','''#ifdef VJ_TEST_HOST
    void testClock(std::chrono::steady_clock::time_point (*fn)()){clockNow=fn;time=0;beats=0;started=false;}
    void testTextMask(const unsigned char* pixels){story.setTestTextMask(pixels);}
#endif
private:
''',1)
s=s.replace('auto now=std::chrono::steady_clock::now();','auto now=clockNow();')
s=s.replace('// Make a *separate* runtime variant','#include "LegacyAdjust.h"\n\n// Make a *separate* runtime variant',1)
for name,classic in [('makeClassicPaletteShader','true'),('makeExpansionPaletteShader','false')]:
 start=s.index('static std::string '+name);end=s.index('    return s;',start)
 s=s[:end]+'    if(!addSharedAdjustments(s,'+classic+'))return {};\n'+s[end:]
s=s.replace('if(int(uRGBAType+.5)>0)alpha*=mix(uRGBA_A.a,uRGBA_B.a,clamp(uRGBAMix,0.,1.));','if(int(uRGBAType+.5)>0)alpha*=int(uRGBAType+.5)==1?uRGBA_A.a:mix(uRGBA_A.a,uRGBA_B.a,clamp(uRGBAMix,0.,1.));')
s=s.replace('paletteId<9;','paletteId<9 && params[P_ORIENTATION]<.000001f && std::abs(params[P_DETAIL]-.5f)<.000001f && std::abs(params[P_DEPTH]-.5f)<.000001f && std::abs(params[P_DENSITY]-.5f)<.000001f;')
s=s.replace('c.period=4.f+116.f*params[P_LOOP];','c.orientation=params[P_ORIENTATION]*6.28318530718f;c.period=4.f+116.f*params[P_LOOP];')
s=s.replace('active->Set("iTime",(float)time);','active->Set("iTime",(float)(time*20.0/(4.0+116.0*params[P_LOOP])));\n    active->Set("uOrientation",params[P_ORIENTATION]*6.28318530718f);\n    active->Set("uGlobalDetail",params[P_DETAIL]);active->Set("uGlobalDepth",params[P_DEPTH]);active->Set("uGlobalDensity",params[P_DENSITY]);')
s=s.replace('activeEngine=requested;params[P_ACTIVE_ENGINE]', 'if(activeEngine!=requested){activeEngine=requested;syncControlVisibility(true);}params[P_ACTIVE_ENGINE]')
s=s.replace('if(index==P_ACTIVE_ENGINE){activeEngine=(int)', 'if(index==P_ACTIVE_ENGINE){activeEngine=(int)')
s=s.replace('params[index]=(float)activeEngine/(kTotalEngines-1);return FF_SUCCESS;', 'params[index]=(float)activeEngine/(kTotalEngines-1);syncControlVisibility(true);return FF_SUCCESS;')
s=s.replace('else if(index==P_CANVAS)params[index]=std::max(0.f,std::min(4.f,value));','else if(index==P_CANVAS){params[index]=std::max(0.f,std::min(4.f,value));syncControlVisibility(true);}')
s=s.replace('   if(index==P_LOOP){','   if(index==P_ORIENTATION){std::snprintf(display,sizeof(display),"%.0f graus",360.f*params[index]);return display;}\n   if(index==P_LOOP){')
pos=s.index('FFResult VJGenerative::SetTextParameter')
s=s[:pos]+'''void VJGenerative::syncControlVisibility(bool notify){
 SetParamVisibility(P_QUALITY,activeEngine>=7,notify);SetParamVisibility(P_COLOR_SOURCE,activeEngine>=7,notify);
 for(unsigned i=P_TEXT;i<=P_TEXT_LAYOUT;i++)SetParamVisibility(i,activeEngine==21,notify);
 SetParamVisibility(P_CUSTOM_W,params[P_CANVAS]>=3.5f,notify);SetParamVisibility(P_CUSTOM_H,params[P_CANVAS]>=3.5f,notify);
 SetParamVisibility(P_GPU_TEST,false,notify);
}

'''+s[pos:]

# The single P_ENGINE option control is created directly above; no extra replacement needed.
s=s.replace('''    int row=index==P_ENGINE?0:(index==P_EXTRA_MOTOR?1:((index>=P_MOTOR_ROW_FIRST&&index<=P_MOTOR_ROW_LAST)?(int)(index-P_MOTOR_ROW_FIRST+2):-1));
    if(row>=0){
       int requested=(int)std::lround(value);
       for(int j=row*6;j<std::min(kTotalEngines,row*6+6);j++)if(kMotorUIOrder[j]==requested){if(activeEngine!=requested){activeEngine=requested;syncControlVisibility(true);}params[P_ACTIVE_ENGINE]=(float)activeEngine/(kTotalEngines-1);break;}
       return FF_SUCCESS;
    }
''','''    if(index==P_ENGINE){
       int requested=(int)std::lround(value);
       if(requested>=0&&requested<kTotalEngines){
          if(activeEngine!=requested){activeEngine=requested;syncControlVisibility(true);}
          params[P_ACTIVE_ENGINE]=(float)activeEngine/(kTotalEngines-1);
       }
       return FF_SUCCESS;
    }
''')
s=s.replace('''if(index==P_ENGINE||index==P_EXTRA_MOTOR||(index>=P_MOTOR_ROW_FIRST&&index<=P_MOTOR_ROW_LAST))return (float)activeEngine;''','''if(index==P_ENGINE)return (float)activeEngine;''')
s=s.replace('''if(index==P_ENGINE||index==P_EXTRA_MOTOR||(index>=P_MOTOR_ROW_FIRST&&index<=P_MOTOR_ROW_LAST))return const_cast<char*>(kMotorNames[activeEngine]);''','''if(index==P_ENGINE)return const_cast<char*>(kMotorNames[activeEngine]);''')

(ROOT/'src/v17/VJGenerativeV17.cpp').write_text(s)
