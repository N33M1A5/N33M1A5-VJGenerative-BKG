vec3 scene(){
 vec2 p=stage();float t=phase();float foil=cloud(p*5.,t);float crease=pow(abs(sin((p.x+p.y)*24.+foil*18.)),16.);
 vec3 c=vec3(.035+.06*foil)+vec3(.12)*crease;
 for(int i=0;i<3;i++){float f=float(i);vec2 q=p-vec2((f-1.)*.77,.06*sin(t+f));float d=roundBox(q,vec2(.28,.67),.27);
  float mask=1.-smoothstep(-.006,.006,d);vec3 color=spectral(q.y*.3+f*.19+.08*sin(t));
  color*=.5+.5*exp(-pow((q.x+.13)/.2,2.));c=mix(c,color*.8,mask);
 }
 return finishLight(c);
}
