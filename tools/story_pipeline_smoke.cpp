#include "src/v16/StoryPipeline.h"
#include <cstring>
#include <cstdio>
static StoryPipeline pipeline;
extern "C" int story_init(){glewExperimental=GL_TRUE;glewInit();while(glGetError()!=GL_NO_ERROR){}return pipeline.init()?1:0;}
extern "C" void story_release(){pipeline.release();}
static bool same(const StoryGLState& a,const StoryGLState& b){
 return a.drawFBO==b.drawFBO&&a.readFBO==b.readFBO&&a.program==b.program&&a.vao==b.vao&&a.active==b.active&&a.unpack==b.unpack&&a.blend==b.blend&&a.depth==b.depth&&a.scissor==b.scissor&&a.cull==b.cull&&a.srgb==b.srgb&&std::memcmp(a.viewport,b.viewport,sizeof(a.viewport))==0&&std::memcmp(a.tex,b.tex,sizeof(a.tex))==0&&std::memcmp(a.sampler,b.sampler,sizeof(a.sampler))==0&&std::memcmp(a.mask,b.mask,sizeof(a.mask))==0;
}
extern "C" int story_render(int engine,float seconds,int mode,int palette,float alpha){
 GLint vp[4];glGetIntegerv(GL_VIEWPORT,vp);
 StoryControls c{};c.time=seconds;c.beat=seconds*2;c.scale=.5;c.intensity=.5;c.detail=.5;c.depth=.5;c.density=.5;c.glow=.35;c.opacity=alpha;c.period=20;c.contrast=.5;
 c.width=(float)vp[2];c.height=(float)vp[3];c.quality=2;c.colorMode=(float)mode;c.palette=(float)palette;c.colorSource=palette<0?0.f:1.f;c.colorMix=.5;
 c.a[0]=1;c.a[3]=.5;c.b[2]=1;c.b[3]=1;
 glEnable(GL_BLEND);glEnable(GL_DEPTH_TEST);glEnable(GL_SCISSOR_TEST);glEnable(GL_CULL_FACE);glEnable(GL_FRAMEBUFFER_SRGB);glColorMask(GL_FALSE,GL_TRUE,GL_FALSE,GL_TRUE);
 glActiveTexture(GL_TEXTURE3);
 bool ok=false;
 {
  StoryGLState before;
  // Capturing itself switches active texture; restore it for a fair comparison.
  glActiveTexture(before.active);
  if(!pipeline.render(engine,c))return -1;
  StoryGLState after;ok=same(before,after);
 }
 GLenum error=glGetError();if(error!=GL_NO_ERROR){std::fprintf(stderr,"GL error %x\n",error);return -2;}
 return ok?1:-3;
}
