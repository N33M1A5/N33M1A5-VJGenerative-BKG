vec3 scene(){
 vec2 p=stage();float t=phase();vec3 c=vec3(.012,.001,.018);
 for(int i=0;i<16;i++){float f=float(i);vec2 center=vec2((hash21(vec2(f,1.))-.5)*3.5,(hash21(vec2(f,3.))-.5)*1.9);
  center+=.06*vec2(sin(t+f),cos(t+f));float size=.035+.12*hash21(vec2(f,seed));float d=length(p-center);
  vec3 color=spectral(f*.24)*vec3(.75,.6,.40);float pulseLight=.3+.7*pow(.5+.5*cos(t*2.+f),3.);
  c+=color*(exp(-d*d/(size*size))*.8+exp(-d/(size*2.))*.08)*pulseLight;
 }
 c+=vec3(.05,.002,.055)*spot(p,vec2(0,.2*sin(t)),vec2(.6,1.));return finishLight(c);
}
