vec3 scene(){
 vec2 p=stage();float t=phase();vec3 c=vec3(.006,.003,.016);
 for(int i=0;i<45;i++){float f=float(i);float column=mod(f,9.)-4.,row=floor(f/9.)-2.;
  vec2 center=vec2(column*.34+.06*sin(t+row),row*.40+.07*cos(t+column));
  vec2 q=p-center;float d=roundBox(q,vec2(.055,.11),.05);float blur=.035+.06*hash21(vec2(f,seed));float shape=1.-smoothstep(-blur,blur,d);
  c+=spectral(hash21(vec2(f,3.)))*shape*(.10+.35*pow(.5+.5*sin(t+f*.8),2.));
 }
 return finishLight(c);
}
