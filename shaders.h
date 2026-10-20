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
// V1.1: parametros aditivos. Com ColorMode=0, transform neutro e canvas Auto,
// os shaders originais (motores 0..5) continuam exatamente iguais.
uniform float uColorMode, uRGBAMix, uRGBABlend, uOpacity;
uniform vec4 uColorA, uColorB; // RGBA (alpha nativo)
uniform float uCanvasPreset, uCanvasFit;
uniform vec2 uCustomCanvas, uPosition, uAnchor;
uniform float uTransformScale, uRotation;
uniform float uVJDetail, uVJDepth, uVJDensity;
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

// 1 - Tunel neon
vec3 eTunnel(vec2 p,float t){
  float d=length(p)+1e-3, a=atan(p.y,p.x);
  float z=.6/d+t*1.5;
  vec2 g=abs(fract(vec2(a*1.2732+uSeed,z))-.5);
  float l=min(g.x,g.y);
  float glow=.025/(l+.025);
  return pal(z*.08+a*.05+t*.03)*glow*smoothstep(0.,.6,d)*.9;
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


// RGBA: usa os canais alpha nativos do FFGL. RGB em espaco [0,1].
vec4 rgbaCombine(float amount){
  vec4 A=clamp(uColorA,0.,1.), B=clamp(uColorB,0.,1.);
  float v=clamp(amount,0.,1.);
  int op=int(uRGBABlend+.5);
  if(op==1)return clamp(A+B*v,0.,1.);
  if(op==2)return vec4(1.-(1.-A.rgb)*(1.-B.rgb*v),mix(A.a,B.a,v));
  if(op==3)return vec4(mix(A.rgb,A.rgb*B.rgb,v),mix(A.a,A.a*B.a,v));
  return mix(A,B,v);
}
float rgbaAnimate(vec2 p,float t){
  return clamp(uRGBAMix+.32*sin(t*.35+p.x*1.3+p.y*.7+fbm(p*.75)*1.4),0.,1.);
}
vec4 chosenRGBA(vec2 p,float t){
  int mode=int(uColorMode+.5);
  if(mode<=1)return clamp(uColorA,0.,1.);
  return rgbaCombine(mode==3?rgbaAnimate(p,t):uRGBAMix);
}
vec3 colorizeVJ(vec3 col,vec2 p,float t){
  int mode=int(uColorMode+.5);
  if(mode==0)return col;
  vec3 tint=chosenRGBA(p,t).rgb;
  float luminosity=dot(max(col,vec3(0.)),vec3(.2126,.7152,.0722));
  return tint*luminosity*1.35;
}

// VJ 6: caleidoscopio de cristal — dobra radial, facetas e bordas antialias.
vec3 eCrystal(vec2 p,float t){
  float sectors=6.+floor(uVJDetail*14.);
  float sector=6.2831853/sectors;
  float a=atan(p.y,p.x)+t*(.055+.13*uVJDepth);
  a=abs(mod(a,sector)-.5*sector);
  float r=length(p);
  vec2 q=vec2(cos(a),sin(a))*r;
  float wave=fbm(q*(1.8+uVJDetail*1.8)+vec2(t*.11,-t*.09)+uSeed);
  float fac=abs(sin(a*sectors*.5+wave*2.5));
  float rad=abs(sin(r*(4.+uVJDetail*7.)-t*.47+wave*2.));
  float fwFac=max(fwidth(fac),.008), fwRad=max(fwidth(rad),.008);
  float edge=pow(1.-smoothstep(.0,fwFac*1.75,fac),3.);
  float rings=pow(1.-smoothstep(.0,fwRad*1.75,rad),3.);
  float bevel=exp(-abs(fac-.45)*17.);
  float shimmer=.5+.5*sin(q.x*3.4-q.y*2.2+t*.12);
  return pal(wave*.9+r*.16+t*.025)*(edge*.35+rings*.62+bevel*.24)
      +vec3(1.)*pow(shimmer,12.)*bevel*.18;
}

// VJ 7: onda de particulas em linhas independentes com camadas de profundidade.
vec3 eParticleWave(vec2 p,float t){
  vec3 c=vec3(0.);
  float count=5.+floor(uVJDensity*12.);
  float aa=max(fwidth(p.y),.002);
  for(int i=0;i<17;i++){
    if(float(i)>=count)break;
    float fi=float(i);
    float y=(fi-(count-1.)*.5)*.115
       +sin(p.x*(1.8+fi*.035)+t*(.24+fi*.014)+fi*.53)*(.06+.04*uVJDepth)
       +.06*sin(p.x*.53-t*.14+fi*.3);
    float line=exp(-abs(p.y-y)/(aa*1.4+.008));
    float beads=pow(.5+.5*cos(p.x*(12.+uVJDetail*22.)+fi*1.7-t*.4),18.);
    float sparkle=pow(max(0.,1.-abs(p.y-y)/(.022+aa)),3.)*beads;
    float depth=1./(1.+fi*.095);
    c+=pal(fi/count+t*.02)*(line*.21+sparkle*.64)*depth;
  }
  return c;
}

// VJ 8: camadas de luz volumetrica / cortinas com textura suave.
vec3 eCurtains(vec2 p,float t){
  vec3 c=vec3(0.);
  float vapor=fbm(p*(.65+uVJDetail*.7)+vec2(t*.017,-t*.014)+uSeed);
  for(int i=0;i<7;i++){
    float fi=float(i);
    float x=p.x+sin(p.y*(.72+fi*.12)+t*(.065+fi*.012)+fi*2.1)*(.13+.12*uVJDepth)
      +(vapor-.5)*.20;
    float center=(fi-3.)*.30;
    float width=.10+.16*uVJDensity;
    float curtain=exp(-pow(abs(x-center)/width,1.4));
    float light=curtain*(.36+.65*vapor)*(1.-.07*fi);
    c+=pal(fi*.15+t*.025)*light*.33;
  }
  return c + pal(vapor)*vapor*.06;
}

// VJ 9: grade digital, corredor com linhas AA e horizonte de profundidade.
vec3 eCyberGrid(vec2 p,float t){
  float y=p.y+.30;
  float perspective=1./max(.12,abs(y)+.12);
  float x=p.x*perspective*(.38+.45*uVJDepth);
  float z=perspective*(.85+uVJDetail)-t*(.70+.4*uVJDepth);
  vec2 grid=abs(fract(vec2(x,z)+uSeed*.15)-.5);
  float dx=max(fwidth(x),.005), dz=max(fwidth(z),.005);
  float lx=1.-smoothstep(.008,.008+dx*1.8,grid.x);
  float lz=1.-smoothstep(.008,.008+dz*1.8,grid.y);
  float line=max(lx,lz);
  float haze=exp(-abs(y)*1.4)*(.08+.12*uVJDensity);
  float glow=exp(-length(p*vec2(.55,1.5))*1.4)*.12;
  return pal(z*.015+p.x*.08+t*.025)*line*(.24+.35*clamp(perspective/4.,0.,1.))
      +pal(.3+t*.01)*(haze+glow);
}

// VJ 10: Source SOLID RGBA. Branco W independente e combinacao de A/B.
vec3 eSolidRGBA(vec2 p,float t){
  int mode=int(uColorMode+.5);
  return chosenRGBA(p,t).rgb;
}

// Coordenadas parametrizadas para output arbitrario (inclui LED 3360x1008).
// O aspect real e sempre o do viewport atual: nao redimensiona um bitmap.
float canvasAspect(float outputAR){
  int preset=int(uCanvasPreset+.5);
  if(preset==1)return 16./9.;
  if(preset==2)return 9./16.;
  if(preset==3)return 4./3.;
  if(preset==4)return 32./9.;
  if(preset==5)return max(uCustomCanvas.x,256.)/max(uCustomCanvas.y,256.);
  return outputAR;
}
vec2 transformedUV(vec2 inputUV){
  vec2 anchorOffset=uAnchor-vec2(.5);
  vec2 center=inputUV-uPosition-anchorOffset;
  float rot=(uRotation-.5)*6.2831853;
  float c=cos(rot),s=sin(rot);
  vec2 rotated=mat2(c,-s,s,c)*center;
  return rotated/exp2((uTransformScale-.5)*4.)+anchorOffset+vec2(.5);
}
vec2 getCanvasUV(vec2 inputUV,float outAspect,out float mask){
  vec2 result=transformedUV(inputUV);
  mask=1.;
  int preset=int(uCanvasPreset+.5);
  int fit=int(uCanvasFit+.5);
  if(preset!=0 && fit==1){
    float target=canvasAspect(outAspect);
    vec2 size=vec2(1.);
    if(outAspect>target)size.x=target/outAspect;
    else size.y=outAspect/target;
    result=(result-.5)/size+.5;
  } else if(preset!=0 && fit==2){
    float target=canvasAspect(outAspect);
    vec2 centered=result-.5;
    if(outAspect>target)centered.y*=target/outAspect;
    else centered.x*=outAspect/target;
    result=centered+.5;
  }
  if(preset!=0 && fit==1){
    mask=step(0.,result.x)*step(result.x,1.)*
         step(0.,result.y)*step(result.y,1.);
  }
  return result;
}

void main(){
  float ar=iResolution.x/max(iResolution.y,1.);
  float canvasMask=1.;
  vec2 canvasUV=getCanvasUV(uv,ar,canvasMask);
  vec2 p=(canvasUV-.5)*vec2(ar,1.)*2.;
  if(int(uCanvasPreset+.5)!=0 && int(uCanvasFit+.5)==3)
    p=(canvasUV-.5)*vec2(canvasAspect(ar),1.)*2.;
  if(int(uCanvasPreset+.5)!=0 && int(uCanvasFit+.5)==1)
    p=(canvasUV-.5)*vec2(canvasAspect(ar),1.)*2.;


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
  else if(e==5) col=eOrbs(p,iTime);
  else if(e==6) col=eCrystal(p,iTime);
  else if(e==7) col=eParticleWave(p,iTime);
  else if(e==8) col=eCurtains(p,iTime);
  else if(e==9) col=eCyberGrid(p,iTime);
  else col=eSolidRGBA(p,iTime);

  float layerAlpha=(e==10 || int(uColorMode+.5)!=0)?chosenRGBA(p,iTime).a:1.;
  float alpha=clamp(uOpacity,0.,1.)*canvasMask*clamp(layerAlpha,0.,1.);
  if(e==10){
    // Solid RGBA: cor exata com alpha sem tonemapping.
    fragColor=vec4(clamp(col,0.,1.)*alpha,alpha);
    return;
  }
  // Com uColorMode=0, col permanece 100% identico a V1.0.
  col=colorizeVJ(col,p,iTime);
  col*=uIntensity*2.*(1.+pulse*.6);
  col=1.-exp(-col*1.4);
  col=pow(col,vec3(mix(1.35,.75,uContrast)));
  col*=1.-.25*dot(uv-.5,uv-.5);
  col+=(hash21(gl_FragCoord.xy+iTime)-.5)/255.;
  fragColor=vec4(clamp(col,0.,1.)*alpha,alpha);
}
)GLSL";