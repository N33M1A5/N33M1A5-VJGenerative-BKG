// Shared material/geometry primitives. Each engine has its own scene program.
uniform sampler2D textMask;
uniform float textReady,textSize,textX,textY,textLayout;
vec2 stage(){vec2 p=canvas()+.015*vec2(sin(seed),cos(seed)-1.);p.x=mix(p.x,abs(p.x),symmetry);return rot(.08*symmetry)*p;}
float h31(vec3 p){return fract(sin(dot(p,vec3(127.1,311.7,74.7)))*43758.5453);}
float n3(vec3 p){
 vec3 i=floor(p),f=fract(p);f=f*f*(3.-2.*f);
 return mix(mix(mix(h31(i),h31(i+vec3(1,0,0)),f.x),mix(h31(i+vec3(0,1,0)),h31(i+vec3(1,1,0)),f.x),f.y),mix(mix(h31(i+vec3(0,0,1)),h31(i+vec3(1,0,1)),f.x),mix(h31(i+vec3(0,1,1)),h31(i+vec3(1,1,1)),f.x),f.y),f.z);
}
float f3(vec3 p){float s=0.,a=.5;for(int k=0;k<5;k++){s+=a*n3(p);p=mat3(1.6,1.2,0.,-1.2,1.6,0.,0.,0.,2.)*p+2.3;a*=.5;}return s;}
float cloud(vec2 p,float t){return f3(vec3(p+seed*.13,0.)+vec3(cos(t),sin(t),cos(t*2.))*.6);}
float lineAA(float d,float w){return 1.-smoothstep(w,w+max(fwidth(d),.001),abs(d));}
float boxSDF(vec2 p,vec2 b){vec2 q=abs(p)-b;return length(max(q,0.))+min(max(q.x,q.y),0.);}
float roundBox(vec2 p,vec2 b,float r){return boxSDF(p,b-vec2(r))-r;}
vec3 spectral(float x){return .52+.48*cos(TAU*(x+vec3(0.,.33,.67)));}
vec3 grad3(float x,vec3 a,vec3 b,vec3 c){x=clamp(x,0.,1.);return x<.5?mix(a,b,smoothstep(0.,.5,x)):mix(b,c,smoothstep(.5,1.,x));}
float spot(vec2 p,vec2 center,vec2 stretch){vec2 q=(p-center)*stretch;return exp(-dot(q,q));}
float grain(vec2 p,float t){vec2 pixel=floor(p*resolution.y*.5);float a=hash21(pixel+vec2(13.7,9.2));float b=hash21(pixel+vec2(71.3,4.9));return mix(a,b,.5+.5*sin(t*5.))*2.-1.;}
float star(vec2 p,float size){float r=length(p);return exp(-r*r/(size*size))+size*.025/(abs(p.x*p.y)+.001);}
vec2 repeat2(vec2 p,vec2 s){return mod(p+s*.5,s)-s*.5;}
vec3 toning(vec3 c,vec2 p,float t){return c*(.98+.02*grain(p,t)*detail);}
float typeMask(vec2 p,float t,bool wall){
 if(textReady<.5)return 0.;
 float s=mix(.65,1.9,textSize);vec2 q=(p-vec2(textX,textY))/s;
 vec2 tx=wall&&textLayout>.5?vec2(fract(q.x*.36+.5+.025*sin(t)),fract(q.y*.9+.5)):q*vec2(.36,.9)+.5;
 if((!wall||textLayout<.5)&&(any(lessThan(tx,vec2(0)))||any(greaterThan(tx,vec2(1)))))return 0.;
 return texture(textMask,tx).r;
}
