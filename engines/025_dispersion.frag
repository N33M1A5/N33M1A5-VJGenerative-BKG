vec3 scene(){
 vec2 p=stage();float t=phase();vec2 o=vec2(.9+.15*cos(t),1.35);vec2 q=p-o;
 float angle=atan(q.y,q.x),r=length(q);vec3 c=vec3(.006,.007,.022);
 for(int i=0;i<45;i++){float f=float(i);float a=-2.9+hash21(vec2(f,seed))*1.7+.018*sin(t+f);
  float d=abs(sin(angle-a))*r;float streak=exp(-pow(d/(.003+.008*hash21(vec2(f,3.))),2.));
  c+=mix(vec3(.035,.07,.16),vec3(.6,.19,.07),hash21(vec2(f,5.)))*streak*.28;
 }
 return finishLight(c);
}
