vec3 scene(){
 vec2 p=stage();float t=phase();vec3 c=vec3(.24,.25,.29);
 float trans=1.;for(int i=0;i<10;i++){float z=float(i)*.17;vec2 q=p*vec2(1.2,1.8)+vec2(.12*sin(t+z),z+.20*sin(t));
  float n=cloud(q*2.,t+z*.3);float densityCloud=smoothstep(.38,.72,n)*.18;
  float lit=.35+.65*cloud(q*2.+vec2(-.08,.12),t+z*.3);
  c+=trans*densityCloud*mix(vec3(.13,.11,.16),vec3(.47,.46,.51),lit);trans*=1.-densityCloud;
 }
 return finishLight(c*.70);
}
