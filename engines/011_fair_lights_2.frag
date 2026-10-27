vec3 scene(){
 vec2 p=stage();float t=phase();vec3 c=vec3(.002,.002,.015);
 // Very large circles of confusion merge into a soft magenta light field.
 for(int i=0;i<9;i++){float f=float(i);vec2 center=vec2(sin(t+f*2.1)*1.7,cos(t+f*1.7)*.8);
  float d=length(p-center);float radius=.55+.35*hash21(vec2(f,seed));
  vec3 color=mix(vec3(.65,.01,.18),vec3(.15,.01,.62),hash21(vec2(f,9.)));
  c+=color*exp(-pow(d/radius,4.))*.27;
 }
 return finishLight(toning(c,p,t));
}
