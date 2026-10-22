vec3 scene(){
 vec2 p=stage();float t=phase();vec3 c=vec3(.004,.002,.021);
 for(int i=0;i<5;i++){float f=float(i);float y=(f-2.)*.31+.15*sin(p.x*1.7+t+f*.9);
  float stripe=exp(-pow((p.y-y)/(.045+.04*depth),2.));
  c+=spectral(f*.18+p.x*.08+.08*sin(t))*stripe*.48;
 }
 c*=.4+.6*exp(-abs(p.x)*.25);return finishLight(c);
}
