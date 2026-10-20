#pragma once

static const char* kVertexShader = R"GLSL(#version 410 core
layout( location = 0 ) in vec4 vPosition;
layout( location = 1 ) in vec2 vUV;
out vec2 uv;
void main(){ gl_Position = vPosition; uv = vUV; }
)GLSL";

static const char* kFragmentShader = R"GLSL(#version 410 core
uniform vec3  iResolution;
uniform float iTime;
uniform float uBeat;
uniform float uEngine, uPalette, uScale, uIntensity, uHue;
uniform float uSlices, uSeed, uPulse, uContrast;
in vec2 uv;
out vec4 fragColor;

float gZoom = 1.0;

float hash21(vec2 p){ p = fract(p*vec2(123.34,456.21)); p += dot(p,p+45.32); return fract(p.x*p.y); }
float noise(vec2 p){
  vec2 i=floor(p), f=fract(p); f=f*f*(3.-2.*f);
  return mix(mix(hash21(i),hash21(i+vec2(1,0)),f.x), mix(hash21(i+vec2(0,1)),hash21(i+vec2(1,1)),f.x), f.y);
}
float fbm(vec2 p){
  float v=0., a=.5; mat2 m=mat2(1.6,1.2,-1.2,1.6);
  for(int i=0;i<5;i++){ v+=a*noise(p); p=m*p; a*=.5; }
  return v;
}

vec3 pal(float t){
  int id=int(uPalette+.5);
  vec3 a=vec3(.5), b=vec3(.5), c=vec3(1.), d=vec3(0.,.33,.67);
  if(id==1){ a=vec3(.5,.35,.2); b=vec3(.5,.4,.3); d=vec3(0.,.1,.2); }
  else if(id==2){ a=vec3(.2,.5,.6); b=vec3(.3,.4,.4); d=vec3(0.,.25,.55); }
  else if(id==3){ c=vec3(1.,1.,.5); d=vec3(.8,.9,.3); }
  else if(id==4){ a=vec3(.5,.3,.6); b=vec3(.5,.3,.5); d=vec3(.7,.8,.9); }
  else if(id==5){ a=vec3(.2,.4,.2); b=vec3(.3,.4,.3); d=vec3(0.,.2,.1); }
  else if(id==6){ a=vec3(.6,.75,.9); b=vec3(.3,.25,.2); d=vec3(0.,.1,.2); }
  else if(id==7){ a=vec3(.6,.4,.4); b=vec3(.4,.4,.4); c=vec3(1.,.8,.6); d=vec3(0.,.15,.3); }
  else if(id==8){ return vec3(.5+.5*cos(6.28318*(t+uHue))); }
  return a+b*cos(6.28318*(c*(t+uHue)+d));
}

// 0 - Nevoa (domain warping)
vec3 eFluid(vec2 p,float t){
  vec2 q=vec2(fbm(p+t*.10+uSeed), fbm(p+vec2(5.2,1.3)-t*.08));
  vec2 r=vec2(fbm(p+3.*q+vec2(1.7,9.2)+.15*t), fbm(p+3.*q+vec2(8.3,2.8)+.12*t));
  float f=fbm(p+3.*r);
  return pal(f*1.2+length(q)*.5+t*.02)*(.25+1.3*f*f+.4*length(r));
}

// 1 - Tunel neon (V1.0.1: correcao de costura angular)
vec3 eTunnel(vec2 p,float t){
  const float PI=3.14159265358979323846;
  const float TAU=6.28318530717958647692;
  float d=length(p)+1e-3, a=atan(p.y,p.x);
  float ang=fract((a+PI)/TAU);
  float z=.6/d+t*1.5;
  vec2 g=abs(fract(vec2(ang*8.0+uSeed,z))-.5);
  float l=min(g.x,g.y);
  float glow=.025/(l+.025);
  return pal(z*.08+sin(a)*.05+t*.03)*glow*smoothstep(0.,.6,d)*.9;
}

// 2 - Lasers de palco
vec3 eLasers(vec2 p,float t){
  vec3 col=vec3(0.);
  float ar=iResolution.x/iResolution.y*gZoom;
  for(int i=0;i<12;i++){
    float fi=float(i);
    vec2 o=vec2((fi/11.-.5)*2.*ar,-gZoom*1.1);
    float ang=1.5708+sin(t*.5+fi*1.7+uSeed)*.75;
    vec2 dir=vec2(cos(ang),sin(ang));
    vec2 w=p-o;
    float dd=abs(w.x*dir.y-w.y*dir.x);
    float along=dot(w,dir);
    col+=pal(fi/12.+t*.05)*(.006/(dd+.006))*step(0.,along)*exp(-along*.25);
  }
  return col*.8+pal(.3)*.03*fbm(p*1.5+t*.1);
}

// 3 - Celulas (voronoi)
vec3 eCells(vec2 p,float t){
  p*=1.5;
  vec2 ip=floor(p), fp=fract(p);
  float d1=8., d2=8., id=0.;
  for(int j=-1;j<=1;j++) for(int i=-1;i<=1;i++){
    vec2 g=vec2(i,j), cell=ip+g;
    vec2 o=vec2(hash21(cell+uSeed),hash21(cell+37.1+uSeed));
    o=.5+.5*sin(t*.8+6.2831*o);
    float d=length(g+o-fp);
    if(d<d1){ d2=d1; d1=d; id=hash21(cell); } else if(d<d2) d2=d;
  }
  float edge=d2-d1;
  vec3 c=pal(id+t*.03)*(.35+.65*smoothstep(0.,1.,1.-d1));
  c+=pal(id+.5)*(.03/(edge+.03))*.6;
  return c*.9;
}

// 4 - Aurora
vec3 eAurora(vec2 p,float t){
  vec3 col=vec3(0.);
  for(int i=0;i<6;i++){
    float fi=float(i);
    float y=sin(p.x*(.9+fi*.35)+t*(.35+fi*.07)+fi*2.+uSeed)*.28+sin(p.x*2.3-t*.4+fi)*.1+(fi-2.5)*.22;
    col+=pal(fi*.17+p.x*.08+t*.03)*(.02/(abs(p.y-y)+.025));
  }
  return col*.7+pal(p.y*.2+.5)*.06;
}

// 5 - Orbes de luz
vec3 eOrbs(vec2 p,float t){
  vec3 c=vec3(0.);
  for(int i=0;i<9;i++){
    float fi=float(i);
    vec2 o=vec2(sin(t*.4+fi*1.9+uSeed)*1.2, cos(t*.33+fi*2.7))*vec2(1.3,.8)*.9;
    float d=length(p-o);
    c+=pal(fi*.11+t*.02)*.05/(d*d+.06);
  }
  return c;
}

void main(){
  float ar=iResolution.x/iResolution.y;
  vec2 p=(uv-.5)*vec2(ar,1.)*2.;

  float beatPhase=fract(uBeat);
  float pulse=pow(1.-beatPhase,3.)*uPulse;

  gZoom=mix(.5,3.,uScale)*(1.+pulse*.05);
  p*=gZoom;

  int n=int(uSlices*12.+.5);
  if(n>=2){
    float seg=6.28318/float(n);
    float a=atan(p.y,p.x);
    a=abs(mod(a,seg)-seg*.5);
    p=length(p)*vec2(cos(a),sin(a));
  }

  int e=int(uEngine+.5);
  vec3 col;
  if(e==0) col=eFluid(p,iTime);
  else if(e==1) col=eTunnel(p,iTime);
  else if(e==2) col=eLasers(p,iTime);
  else if(e==3) col=eCells(p,iTime);
  else if(e==4) col=eAurora(p,iTime);
  else col=eOrbs(p,iTime);

  col*=uIntensity*2.*(1.+pulse*.6);
  col=1.-exp(-col*1.4);
  col=pow(col,vec3(mix(1.35,.75,uContrast)));
  col*=1.-.25*dot(uv-.5,uv-.5);
  col+=(hash21(gl_FragCoord.xy+iTime)-.5)/255.;
  fragColor=vec4(clamp(col,0.,1.),1.);
}
)GLSL";