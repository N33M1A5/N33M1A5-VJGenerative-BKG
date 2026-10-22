vec3 scene(){
 vec2 p=stage();float t=phase();vec3 c=mix(vec3(.57,.02,.16),vec3(.20,.002,.65),clamp(p.x*.3+.5,0.,1.));
 for(int i=0;i<12;i++){float f=float(i);float x=(f-5.5)*.22+.13*sin(p.y*4.+f*.8+t);
  float crest=.45*sin(p.y*5.+f*.9+sin(t));float d=p.x-x-crest*.2;float ribbon=exp(-pow(d/.13,4.));
  vec3 color=grad3(.5+.5*sin(f*.4+t),vec3(.85,.3,.015),vec3(.7,.025,.18),vec3(.21,.005,.64));
  c=mix(c,color,ribbon*.65);
 }
 return finishLight(c);
}
