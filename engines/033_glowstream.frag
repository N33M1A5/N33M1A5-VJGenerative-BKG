vec3 scene(){
 vec2 p=stage();float t=phase();vec3 c=vec3(.001,.0004,.001);
 for(int i=0;i<5;i++){float f=float(i);float center=(f-2.)*.73+.26*sin(p.y*1.3+t+f*.5);
  float width=.04+.13*pow(.5+.5*cos(p.y*2.+t+f),2.);float d=p.x-center;
  float core=exp(-pow(d/width,2.));float halo=exp(-abs(d)/(width*2.5));
  c+=mix(vec3(.4,.008,.004),vec3(1.1,.64,.30),pow(core,4.))*core*.6+vec3(.18,.008,.003)*halo*.14;
 }
 return finishLight(c);
}
