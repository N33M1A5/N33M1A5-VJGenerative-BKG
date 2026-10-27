vec3 scene(){
 vec2 p=stage();float t=phase();vec3 c=vec3(.001,.001,.008);
 for(int i=0;i<24;i++){float f=float(i);float x=(f-11.5)*.14;float y=.2*sin(t+f*.8);
  float facet=abs(p.x-x)/(.025+.025*hash21(vec2(f,seed)))+abs(p.y-y)/(.4+.65*hash21(vec2(f,3.)));
  float shape=pow(max(0.,1.-facet),.55);float light=pow(.5+.5*sin(t+f*.67),4.);
  c+=mix(vec3(.018,.02,.11),vec3(.25,.015,.40),light)*shape*(.1+light);
 }
 return finishLight(c);
}
