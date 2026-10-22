vec3 scene(){
 vec2 p=stage();float t=phase();vec3 c=vec3(.001);float transmission=1.;
 for(int i=0;i<14;i++){float z=float(i)/14.;vec2 q=p*vec2(1.2,1.7)+vec2(.35*sin(t+z),z*.6);
  q.x+=.22*sin(q.y*2.+t+z);float n=cloud(q*2.8,t+z);float plume=exp(-pow((p.x+.8-.7*z-.25*sin(t))/.6,2.));
  float d=smoothstep(.38,.74,n)*plume*.16;vec3 tint=mix(vec3(.29,.006,.16),vec3(.01,.29,.31),smoothstep(-.3,.7,p.x+z));
  c+=transmission*tint*d*(.4+.6*cloud(q*3.+vec2(-.1,.1),t+z));transmission*=1.-d;
 }
 return finishLight(c*4.);
}
