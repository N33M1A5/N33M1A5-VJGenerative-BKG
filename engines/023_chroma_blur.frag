vec3 scene(){
 vec2 p=stage();float t=phase();vec3 c=vec3(.008,.006,.025);
 for(int i=0;i<10;i++){float f=float(i);float x=(hash21(vec2(f,1.))- .5)*3.8+.15*sin(t+f);
  float y=.5*sin(t+f*2.);float w=.10+.30*hash21(vec2(f,seed));float v=spot(p,vec2(x,y),vec2(1./w,1.0));
  c+=spectral(f*.14+.03*sin(t))*v*.26;
 }
 return finishLight(toning(c,p,t));
}
