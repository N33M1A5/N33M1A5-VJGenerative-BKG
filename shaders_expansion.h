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
 if(int(uRGBAType+.5)>0){float lum=dot(max(col,vec3(0.)),vec3(.2126,.7152,.0722));col=rgba.rgb*lum*1.35;alpha*=rgba.a;}
 col*=uIntensity*2.*(1.+beat*.6);
 col=1.-exp(-col*1.4);
 col=pow(max(col,vec3(0.)),vec3(mix(1.35,.75,uContrast)));
 col*=1.-.25*dot(uv-.5,uv-.5);
 col+=(hash21(gl_FragCoord.xy+iTime)-.5)/255.;
 fragColor=vec4(clamp(col,0.,1.),alpha);
}
)GLSL";