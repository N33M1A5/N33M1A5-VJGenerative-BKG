vec3 scene(){
 vec2 p=stage();float t=phase();vec3 c=vec3(.001,.003,.006);
 for(int i=0;i<7;i++){float f=float(i);float radius=.2+f*.26+.035*sin(t+f*.8);
  vec2 center=vec2(.08*cos(t+f),.035*sin(t+f*1.3));float r=length(p-center);
  float ring=exp(-pow((r-radius)/(.014+.008*depth),2.));
  float aura=exp(-pow((r-radius)/.08,2.));float side=.12+.88*pow(.5+.5*cos(atan(p.y,p.x)-t-f*.7),3.);
  c+=vec3(.035,.25,.34)*(ring*.8+aura*.13)*side;
 }
 return finishLight(c);
}
