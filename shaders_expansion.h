#pragma once
// VJ EXPANSION renderer, deliberately isolated from the exact V1.0 GLSL.
static const char* kVJVertexShader = R"GLSL(#version 410 core
layout(location=0) in vec4 vPosition;
layout(location=1) in vec2 vUV;
out vec2 uv;
void main(){ gl_Position=vPosition; uv=vUV; }
)GLSL";
static const char* kVJFragmentShader = R"GLSL(#version 410 core
in vec2 uv;
out vec4 fragColor;
uniform vec3 iResolution;
uniform float iTime,uBeat;
uniform float uPalette,uScale,uIntensity,uHue,uSlices,uSeed,uPulse,uContrast;
uniform float uVJEngine,uDetail,uDepth,uDensity,uRGBAType,uRGBABlend,uRGBAMix,uOpacity,uCanvasMode;
uniform vec4 uRGBA_A,uRGBA_B;
uniform vec2 uCustomSize;
const float TAU=6.28318530718;
float hash21(vec2 p){p=fract(p*vec2(123.34,456.21));p+=dot(p,p+45.32);return fract(p.x*p.y);}
float n2(vec2 p){vec2 i=floor(p),f=fract(p);f=f*f*(3.-2.*f);
return mix(mix(hash21(i),hash21(i+vec2(1,0)),f.x),mix(hash21(i+vec2(0,1)),hash21(i+vec2(1,1)),f.x),f.y);}
float fbm(vec2 p){float s=0.,amp=.5;mat2 m=mat2(1.6,1.2,-1.2,1.6);
for(int j=0;j<4;j++){s+=n2(p)*amp;p=m*p;amp*=.5;}return s;}
vec3 pal(float t){
 int id=int(uPalette+.5);vec3 a=vec3(.5),b=vec3(.5),c=vec3(1.),d=vec3(0.,.33,.67);
 if(id==1){a=vec3(.5,.35,.2);b=vec3(.5,.4,.3);d=vec3(0.,.1,.2);}
 else if(id==2){a=vec3(.2,.5,.6);b=vec3(.3,.4,.4);d=vec3(0.,.25,.55);}
 else if(id==3){c=vec3(1.,1.,.5);d=vec3(.8,.9,.3);}
 else if(id==4){a=vec3(.5,.3,.6);b=vec3(.5,.3,.5);d=vec3(.7,.8,.9);}
 else if(id==5){a=vec3(.2,.4,.2);b=vec3(.3,.4,.3);d=vec3(0.,.2,.1);}
 else if(id==6){a=vec3(.6,.75,.9);b=vec3(.3,.25,.2);d=vec3(0.,.1,.2);}
 else if(id==7){a=vec3(.6,.4,.4);b=vec3(.4,.4,.4);c=vec3(1.,.8,.6);d=vec3(0.,.15,.3);}
 else if(id==8)return vec3(.5+.5*cos(TAU*(t+uHue)));
 return a+b*cos(TAU*(c*(t+uHue)+d));
}
float aaLine(float distance,float width){float edge=max(fwidth(distance),.001);return 1.-smoothstep(width,width+edge*1.75,distance);}
vec4 rgbaMix(vec2 p,float t){
 vec4 A=clamp(uRGBA_A,0.,1.),B=clamp(uRGBA_B,0.,1.);
 int typ=int(uRGBAType+.5);if(typ<=1)return A;
 float k=clamp(uRGBAMix,0.,1.);if(typ==3)k=clamp(k+.33*sin(t*.3+length(p)*1.5+fbm(p*.6)),0.,1.);
 int op=int(uRGBABlend+.5);
 if(op==1)return clamp(A+B*k,0.,1.);
 if(op==2)return vec4(1.-(1.-A.rgb)*(1.-B.rgb*k),mix(A.a,B.a,k));
 if(op==3)return vec4(mix(A.rgb,A.rgb*B.rgb,k),mix(A.a,A.a*B.a,k));
 return mix(A,B,k);
}
float aspect(){int m=int(uCanvasMode+.5);if(m==1)return 16./9.;if(m==2)return 9./16.;
 if(m==3)return 32./9.;if(m==4)return max(128.,uCustomSize.x)/max(128.,uCustomSize.y);
 return iResolution.x/max(iResolution.y,1.);}
vec3 crystal(vec2 p,float t){
 float a=atan(p.y,p.x)+t*(.06+.1*uDepth),r=length(p);
 float seg=TAU/(5.+floor(uDetail*13.));a=abs(mod(a,seg)-seg*.5);
 vec2 q=r*vec2(cos(a),sin(a));
 float n=fbm(q*(2.+2.*uDensity)+uSeed*.1+vec2(t*.06,-t*.04));
 float g=abs(sin(a*(9.+uDetail*19.)+n*3.)),rr=abs(sin(r*(6.+uDetail*10.)-t*.5+n*2.));
 float facets=pow(.5+.5*cos(n*12.+r*4.),8.);
 return pal(n*.8+r*.1+t*.02)*(aaLine(g,.10)*.65+aaLine(rr,.08)*.8+facets*.26);
}
vec3 particles(vec2 p,float t){
 vec3 v=vec3(0.);int layers=5+int(uDensity*11.);
 for(int i=0;i<16;i++){if(i>=layers)break;float fi=float(i);
 float y=(fi-float(layers-1)*.5)*.15+sin(p.x*(1.4+fi*.07)+t*(.22+fi*.018)+fi*.64)*(.08+.08*uDepth);
 float d=abs(p.y-y),pt=pow(.5+.5*cos(p.x*(10.+uDetail*24.)-t*.38+fi*1.3),18.);
 v+=pal(fi/float(layers)+t*.018)*(aaLine(d,.008)*(.12+.7*pt)+exp(-d*30.)*.05)*(.9-fi*.025);}
 return v;
}
vec3 curtains(vec2 p,float t){
 vec3 v=vec3(0.);float f=fbm(p*(.7+uDetail)+vec2(t*.022,-t*.016)+uSeed*.1);
 for(int i=0;i<7;i++){float fi=float(i);
 float x=p.x-(fi-3.)*.33+sin(p.y*(1.+fi*.13)+t*(.09+fi*.01)+fi*2.)*(.10+.16*uDepth)+(f-.5)*.3;
 float stripe=exp(-pow(abs(x)/(.08+.14*uDensity),1.4));
 v+=pal(fi*.15+t*.02)*stripe*(.2+.65*f)*.30;}
 return v+pal(f+t*.02)*f*.05;
}
vec3 cyber(vec2 p,float t){
 float horizon=p.y+.3,depth=1./max(.14,abs(horizon)+.14);
 float x=p.x*depth*(.38+.48*uDepth),z=depth*(.8+uDetail)-t*(.5+.6*uDepth);
 vec2 cell=abs(fract(vec2(x,z)+uSeed*.1)-.5);
 vec2 deriv=max(fwidth(vec2(x,z)),vec2(.004));
 float gx=1.-smoothstep(.01,.01+deriv.x*1.7,cell.x);
 float gz=1.-smoothstep(.01,.01+deriv.y*1.7,cell.y);
 float light=exp(-abs(horizon)*4.)*(.1+.24*uDensity);
 return pal(z*.014+t*.025)*max(gx,gz)*(.23+.42*min(depth*.14,1.))+pal(t*.03+.3)*light;
}
vec3 kaleido(vec2 p,float t){
 float r=length(p),sector=TAU/(5.+floor(uDetail*9.));
 float a=abs(mod(atan(p.y,p.x)+t*.07,sector)-sector*.5);
 float pulse=.5+.5*cos(uBeat*TAU);
 float g=abs(sin(a*(12.+floor(uDetail*15.))+r*4.+t*.17));
 float rr=abs(sin(r*(5.+uDensity*10.)-t*(.4+.26*uDepth)+pulse));
 float aura=pow(.5+.5*cos(a*23.-r*4.-t*.3),10.);
 return pal(r*.18+a*.8+t*.04)*(aaLine(g,.065)*.67+aaLine(rr,.07)*.73+aura*.2)*(1.+pulse*.4);
}

// 6 - Hex Mosaic: crystalline hexagonal tiles with independently lit edges.
vec3 hexMosaic(vec2 p,float t){
 p*=2.5+uDensity*3.;p.x*=.57735027;
 vec2 r=vec2(p.x+p.y*.5,p.y);
 vec2 g=abs(fract(r)-.5);
 float a=max(g.x,g.y),b=abs(g.x-g.y);
 float borders=aaLine(abs(a-.39),.018)+aaLine(abs(b-.23),.02)*.3;
 float cell=hash21(floor(r)+uSeed);
 float flicker=.72+.28*sin(t*.15+cell*TAU);
 return pal(cell*.7+t*.025)*(.14+.46*borders)*flicker;
}
// 7 - Sacred Mandala: geometry that forms concentric petals.
vec3 mandala(vec2 p,float t){
 float r=length(p),a=atan(p.y,p.x);
 float petals=6.+floor(uDetail*18.);
 float shape=abs(cos(a*petals*.5+t*.07))*r;
 float ribs=abs(sin(shape*(10.+uDensity*12.)-t*.3));
 float circles=abs(sin(r*(9.+uDepth*12.)+t*.27));
 float l=aaLine(ribs,.09)*.72+aaLine(circles,.06)*.6;
 float glow=exp(-r*1.7)*(.12+.23*pow(abs(sin(a*petals*.5)),8.));
 return pal(r*.35+a*.09+t*.03)*(l+glow);
}
// 8 - Orbit Rings: independently rotating concentric rings.
vec3 orbitRings(vec2 p,float t){
 vec3 c=vec3(0.);
 for(int i=0;i<9;i++){
  float k=float(i),radius=.22+k*(.12+uDensity*.018);
  vec2 center=vec2(.10*sin(t*(.11+.013*k)+k),.07*cos(t*(.13+.02*k)+k));
  float d=abs(length(p-center)-radius);
  float arc=.2+.8*pow(.5+.5*cos(atan(p.y-center.y,p.x-center.x)*(3.+mod(k,4.))-t*(.35+.04*k)+k),3.);
  c+=pal(k*.13+t*.024)*aaLine(d,.012+.014*uDetail)*arc*.47;
 }
 return c;
}
// 9 - Polygon Prism: hard edged shifting facet mesh.
vec3 polygonPrism(vec2 p,float t){
 p*=2.1+uDensity*2.;
 vec2 tile=floor(p),q=fract(p)-.5;
 float side=3.+floor(uDetail*7.);
 float angle=atan(q.y,q.x)+t*.08;
 float sector=TAU/side;
 float rad=length(q)*cos(mod(angle,sector)-sector*.5);
 float boundary=aaLine(abs(rad-(.18+.06*sin(t*.2+hash21(tile)*TAU))),.012);
 float rib=aaLine(abs(sin(angle*side*.5+rad*12.)),.055);
 float seed=hash21(tile+uSeed);
 return pal(seed*.8+rad+t*.025)*(boundary*.7+rib*.16+.03);
}
// 10 - Fractal Bloom: soft branching luminous structures.
vec3 fractalBloom(vec2 p,float t){
 vec2 q=p*(1.4+uDensity*1.3);
 vec2 warp=vec2(fbm(q+vec2(t*.08,uSeed)),fbm(q+vec2(4.,7.)-t*.06));
 float n=fbm(q+2.7*warp);
 float petal=abs(sin(atan(q.y,q.x)*(4.+floor(uDetail*8.))+length(q)*5.-t*.32+n*4.));
 float core=exp(-length(q)*2.);
 return pal(n*.9+t*.026)*(pow(1.-petal,5.)*.65+n*n*.62+core*.45);
}
// 11 - Stage Beams: atmospheric performance beams from several moving fixtures.
vec3 stageBeams(vec2 p,float t){
 vec3 c=vec3(0.);
 for(int i=0;i<12;i++){
  float fi=float(i);
  vec2 source=vec2((fi/11.-.5)*4.,-1.45);
  vec2 v=p-source;
  float angle=1.570796+.73*sin(t*(.24+.009*fi)+fi*1.73+uSeed);
  vec2 dir=vec2(cos(angle),sin(angle));
  float perp=abs(v.x*dir.y-v.y*dir.x),along=dot(v,dir);
  float aa=max(fwidth(perp),.004);
  float width=.018+.04*uDetail;
  float beam=exp(-perp/(width+aa)) *step(0.,along)*exp(-along*(.18+.14*(1.-uDepth)));
  c+=pal(fi*.083+t*.02)*beam*.38;
 }
 return c;
}
// 12 - Lens Bokeh: defocused depth-layer floating light disks.
vec3 bokeh(vec2 p,float t){
 vec3 c=vec3(0.);
 for(int j=-2;j<=2;j++)for(int i=-3;i<=3;i++){
  vec2 id=vec2(float(i),float(j));
  float seed=hash21(id+uSeed);
  float depth=mix(.65,2.1,seed);
  vec2 q=(p-id*.52-vec2(sin(t*.07+seed*7.),cos(t*.075+seed*9.))*.13);
  float d=length(q)*depth;
  float radius=.055+.10*seed*uDensity+.03;
  float disk=1.-smoothstep(radius*.65,radius+max(fwidth(d),.006),d);
  float halo=exp(-d*d/(radius*radius*7.))*.08;
  c+=pal(seed+t*.02)*(disk*.4+halo);
 }
 return c;
}
// 13 - Liquid Plasma: animated domain warped colored fluid.
vec3 liquidPlasma(vec2 p,float t){
 vec2 q=p*(1.+uDensity*2.);
 vec2 w=vec2(fbm(q+vec2(t*.08,.2)),fbm(q+vec2(9.,2.)-t*.075));
 vec2 v=vec2(fbm(q+2.5*w+vec2(t*.13,0.)),fbm(q+2.5*w+vec2(0.,t*.1)));
 float f=fbm(q+v*(2.5+uDepth*3.));
 float ridges=pow(abs(sin((f+w.x)*TAU)),7.)*uDetail*.45;
 return pal(f+w.y*.6+t*.025)*(.20+1.45*f*f+ridges);
}
// 14 - Neon Ribbons: glossy flowing lines organized in depth.
vec3 ribbons(vec2 p,float t){
 vec3 c=vec3(0.);
 for(int i=0;i<10;i++){
  float k=float(i);
  float offs=(k-4.5)*(.19+.03*uDensity);
  float wave=sin(p.x*(1.3+k*.12)+t*(.23+.02*k)+k*.86)*(.09+.12*uDepth);
  float y=offs+wave+sin(p.x*3.5-t*.14+k)*.045;
  float dist=abs(p.y-y);
  float core=aaLine(dist,.009+.011*uDetail);
  float halo=exp(-dist*17.)*.13;
  c+=pal(k*.105+p.x*.02+t*.015)*(core*.45+halo*.15);
 }
 return c;
}
// 15 - Starfield Warp: continuous depth-perspective particles.
vec3 starfield(vec2 p,float t){
 vec3 c=vec3(0.);
 for(int layer=0;layer<4;layer++){
  float k=float(layer);
  float z=fract(k*.25+t*(.08+.09*uDepth));
  float depth=.10+z*2.4;
  vec2 grid=p*depth*(5.+uDensity*10.);
  vec2 cell=floor(grid),pos=fract(grid)-.5;
  float rnd=hash21(cell+uSeed+k*31.);
  vec2 offset=vec2(hash21(cell+8.1+k),hash21(cell+19.1+k))-.5;
  float d=length(pos-offset*.6);
  float star=aaLine(d,.012+.03*rnd*uDetail)*step(.60,rnd);
  c+=pal(rnd+k*.2+t*.018)*star*(.35+z*.3);
 }
 return c;
}
// 16 - Circuit Board: orthogonal tracks, nodes and scrolling signal pulses.
vec3 circuitBoard(vec2 p,float t){
 p*=3.+uDensity*4.;
 vec2 id=floor(p),f=fract(p)-.5;
 float seed=hash21(id+uSeed);
 float h=aaLine(abs(f.y),.017)*step(.26,seed);
 float v=aaLine(abs(f.x),.017)*step(seed,.74);
 float node=aaLine(length(f),.06)*step(.5,seed);
 float signal=pow(.5+.5*cos((p.x+p.y)*(2.+uDetail*8.)-t*1.6+seed*TAU),10.);
 return pal(seed+t*.025)*(h*.35+v*.36+node*.55+signal*(h+v)*.4);
}
// 17 - Digital Rain: narrow animated bit columns, vertical depth parallax.
vec3 digitalRain(vec2 p,float t){
 vec2 q=p*vec2(11.+uDensity*17.,7.+uDetail*12.);
 vec2 cell=floor(q),f=fract(q);
 float seed=hash21(vec2(cell.x+uSeed,12.));
 float speed=.35+seed*.9+uDepth*.3;
 float scan=fract(f.y-t*speed+seed);
 float digit=hash21(cell+vec2(0.,floor(t*.8*speed)));
 float body=step(.48,digit)*step(.12,f.x)*step(f.x,.84)*step(.13,f.y)*step(f.y,.9);
 float head=exp(-scan*7.);
 float flicker=.35+.65*hash21(cell+floor(t*2.));
 return pal(seed*.55+t*.02)*(body*head*flicker*.7);
}

void main(){
 float ar=aspect();
 vec2 p=(uv-.5)*vec2(ar,1.)*2.;
 float beat=pow(1.-fract(uBeat),3.)*uPulse;
 p*=mix(.5,3.,uScale)*(1.+beat*.05);
 int sym=int(uSlices*12.+.5);
 if(sym>=2){float seg=TAU/float(sym);float a=abs(mod(atan(p.y,p.x),seg)-seg*.5);p=length(p)*vec2(cos(a),sin(a));}
 int id=int(uVJEngine+.5);
 vec4 rgba=rgbaMix(p,iTime);
 float alpha=clamp(uOpacity,0.,1.);
 if(id==5){fragColor=vec4(rgba.rgb,alpha*rgba.a);return;}
 vec3 col=vec3(0.);
 if(id==0)col=crystal(p,iTime);
 else if(id==1)col=particles(p,iTime);
 else if(id==2)col=curtains(p,iTime);
 else if(id==3)col=cyber(p,iTime);
 else if(id==4)col=kaleido(p,iTime);
 else if(id==6)col=hexMosaic(p,iTime);
 else if(id==7)col=mandala(p,iTime);
 else if(id==8)col=orbitRings(p,iTime);
 else if(id==9)col=polygonPrism(p,iTime);
 else if(id==10)col=fractalBloom(p,iTime);
 else if(id==11)col=stageBeams(p,iTime);
 else if(id==12)col=bokeh(p,iTime);
 else if(id==13)col=liquidPlasma(p,iTime);
 else if(id==14)col=ribbons(p,iTime);
 else if(id==15)col=starfield(p,iTime);
 else if(id==16)col=circuitBoard(p,iTime);
 else if(id==17)col=digitalRain(p,iTime);
 if(int(uRGBAType+.5)>0){float lum=dot(max(col,vec3(0.)),vec3(.2126,.7152,.0722));col=rgba.rgb*lum*1.35;alpha*=rgba.a;}
 col*=uIntensity*2.*(1.+beat*.6);
 col=1.-exp(-col*1.4);
 col=pow(max(col,vec3(0.)),vec3(mix(1.35,.75,uContrast)));
 col*=1.-.25*dot(uv-.5,uv-.5);
 col+=(hash21(gl_FragCoord.xy+iTime)-.5)/255.;
 fragColor=vec4(clamp(col,0.,1.),alpha);
}
)GLSL";