#define VJ_TEST_HOST
#include "src/v17/VJGenerativeV17.cpp"
#include <memory>
static std::unique_ptr<VJGenerative> plugin;
static std::chrono::steady_clock::time_point nowValue;
static std::chrono::steady_clock::time_point testNow(){return nowValue;}
extern "C" int native_init(int w,int h){
 glewExperimental=GL_TRUE;glewInit();while(glGetError()!=GL_NO_ERROR){}
 plugin=std::make_unique<VJGenerative>();FFGLViewportStruct viewport{};viewport.width=w;viewport.height=h;
 return plugin->InitGL(&viewport)==FF_SUCCESS?1:0;
}
extern "C" void native_close(){if(plugin){plugin->DeInitGL();plugin.reset();}}
extern "C" void native_clock_reset(){nowValue={};plugin->testClock(testNow);}
extern "C" int native_frame(double seconds){nowValue=std::chrono::steady_clock::time_point(std::chrono::duration_cast<std::chrono::steady_clock::duration>(std::chrono::duration<double>(seconds)));ProcessOpenGLStruct gl{};
 glDisable(GL_BLEND);glDisable(GL_DEPTH_TEST);glDisable(GL_SCISSOR_TEST);glDisable(GL_CULL_FACE);glDisable(GL_FRAMEBUFFER_SRGB);glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
 return plugin->ProcessOpenGL(&gl)==FF_SUCCESS&&glGetError()==GL_NO_ERROR?1:0;
}
extern "C" int native_set(unsigned p,float value){return plugin->SetFloatParameter(p,value)==FF_SUCCESS?1:0;}
extern "C" float native_get(unsigned p){return plugin->GetFloatParameter(p);}
extern "C" unsigned native_count(){return plugin->GetNumParams();}
extern "C" const char* native_name(unsigned p){return plugin->GetParamName(p);}
extern "C" const char* native_group(unsigned p){static std::string group;group=plugin->GetParamGroup(p);return group.c_str();}
extern "C" unsigned native_visible(unsigned p){return plugin->GetParamVisibility(p);}
extern "C" unsigned native_type(unsigned p){return plugin->GetParamType(p);}
extern "C" int native_select(int engine){return native_set(P_ACTIVE_ENGINE,(float)engine/81.f);}
extern "C" int native_mask(const unsigned char* pixels){
 glPixelStorei(GL_UNPACK_ALIGNMENT,8);glPixelStorei(GL_UNPACK_ROW_LENGTH,2048);glPixelStorei(GL_UNPACK_SKIP_ROWS,3);glPixelStorei(GL_UNPACK_SKIP_PIXELS,7);
 plugin->testTextMask(pixels);GLint a,b,c,d;glGetIntegerv(GL_UNPACK_ALIGNMENT,&a);glGetIntegerv(GL_UNPACK_ROW_LENGTH,&b);glGetIntegerv(GL_UNPACK_SKIP_ROWS,&c);glGetIntegerv(GL_UNPACK_SKIP_PIXELS,&d);
 return a==8&&b==2048&&c==3&&d==7?1:0;
}
extern "C" int native_text(const char* s){return plugin->SetTextParameter(P_TEXT,s)==FF_SUCCESS?1:0;}
extern "C" const char* native_get_text(){return plugin->GetTextParameter(P_TEXT);}
