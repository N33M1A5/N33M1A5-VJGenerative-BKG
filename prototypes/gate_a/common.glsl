// Gate A: isolated visual prototypes. Not yet part of the FFGL DLL.
uniform vec2 resolution;
uniform float time, speed, scale, intensity, hue, symmetry, seed;
uniform float bpm, pulse, detail, depth, density, glow, opacity, period;
uniform float direction,orientation;
const float TAU=6.28318530718;
float phase(){return TAU*time*speed*direction/max(period,1.);}
float hash21(vec2 p){return fract(sin(dot(p,vec2(127.1,311.7)))*43758.5453);}
float noise2(vec2 p){
 vec2 i=floor(p),f=fract(p);f=f*f*(3.-2.*f);
 return mix(mix(hash21(i),hash21(i+vec2(1,0)),f.x),mix(hash21(i+vec2(0,1)),hash21(i+1.),f.x),f.y);
}
float fbm(vec2 p){float v=0.,a=.5;for(int i=0;i<4;i++){v+=a*noise2(p);p=mat2(1.6,1.2,-1.2,1.6)*p+3.17;a*=.5;}return v;}
mat2 rot(float a){return mat2(cos(a),-sin(a),sin(a),cos(a));}
vec3 hueShift(vec3 c,float h){
 vec3 k=normalize(vec3(1));float a=h*TAU;
 return max(vec3(0),c*cos(a)+cross(k,c)*sin(a)+k*dot(k,c)*(1.-cos(a)));
}
vec2 canvas(){vec2 p=(uv-.5)*vec2(resolution.x/resolution.y,1.)*2./mix(.65,1.65,scale);p=rot(orientation)*p;p.y/=max(.25,1.+(depth-.5)*p.y*.22);return p;}
vec3 finishLight(vec3 c){
 float beat=.5+.5*cos(TAU*time*bpm/60.);
 return hueShift(max(c,0.),hue)*mix(.35,2.2,intensity)*(1.+pulse*.55*beat);
}
