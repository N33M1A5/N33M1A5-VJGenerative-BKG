#include "src/v17/StoryPipeline.h"
#include <cstring>
#include <cstdio>
static StoryPipeline pipeline;
extern "C" int catalog_init(){glewExperimental=GL_TRUE;glewInit();while(glGetError()!=GL_NO_ERROR){}return pipeline.init()?1:0;}
extern "C" void catalog_release(){pipeline.release();}
extern "C" void catalog_mask(const unsigned char* data){pipeline.setTestTextMask(data);}
static bool same(const StoryGLState& a,const StoryGLState& b){
 return a.drawFBO==b.drawFBO&&a.readFBO==b.readFBO&&a.program==b.program&&a.vao==b.vao&&a.active==b.active&&a.unpack==b.unpack&&a.blend==b.blend&&a.depth==b.depth&&a.scissor==b.scissor&&a.cull==b.cull&&a.srgb==b.srgb&&std::memcmp(a.viewport,b.viewport,sizeof(a.viewport))==0&&std::memcmp(a.tex,b.tex,sizeof(a.tex))==0&&std::memcmp(a.sampler,b.sampler,sizeof(a.sampler))==0&&std::memcmp(a.mask,b.mask,sizeof(a.mask))==0;
}
extern "C" int catalog_render(int engine,float seconds,const float* v){
 GLint vp[4];glGetIntegerv(GL_VIEWPORT,vp);StoryControls c{};
 c.time=seconds;c.beat=seconds*v[21]/60.;c.scale=v[0];c.intensity=v[1];c.hue=v[2];c.symmetry=v[3];c.seed=v[4];c.pulse=v[5];c.detail=v[6];c.depth=v[7];c.density=v[8];c.glow=v[9];c.opacity=v[10];c.palette=v[11];c.colorMode=v[12];c.colorMix=v[13];c.quality=v[14];c.contrast=v[15];c.period=v[16];c.textSize=v[17];c.textX=v[18];c.textY=v[19];c.textLayout=v[20];
 c.width=(float)vp[2];c.height=(float)vp[3];c.colorSource=c.palette<0?0.f:1.f;c.a[0]=1;c.a[3]=.5;c.b[2]=1;c.b[3]=1;
 glEnable(GL_BLEND);glEnable(GL_DEPTH_TEST);glEnable(GL_SCISSOR_TEST);glEnable(GL_CULL_FACE);glEnable(GL_FRAMEBUFFER_SRGB);glColorMask(GL_FALSE,GL_TRUE,GL_FALSE,GL_TRUE);glActiveTexture(GL_TEXTURE3);
 bool ok=false;{
  StoryGLState before;glActiveTexture(before.active);if(!pipeline.render(engine,c))return -1;StoryGLState after;ok=same(before,after);
 }
 GLenum error=glGetError();if(error!=GL_NO_ERROR){std::fprintf(stderr,"GL error %x engine %d\n",error,engine);return -2;}return ok?1:-3;
}
