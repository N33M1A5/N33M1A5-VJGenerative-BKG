vec3 scene(){
 vec2 p=stage();float t=phase();vec2 q=rot(-.43)*p;vec3 c=vec3(.008,.005,.045);
 for(int i=0;i<38;i++){float f=float(i);float y=(hash21(vec2(f,1.))-.5)*2.5;
  float x=sin(t+f*2.1)*2.4;vec2 d=q-vec2(x,y);float stripe=exp(-pow(d.y/.012,2.))*exp(-pow(d.x/.30,2.));
  vec3 color=mix(vec3(.25,.24,.7),vec3(.8,.9,.2),hash21(vec2(f,seed)));c+=color*stripe*.65;
  c+=color*exp(-pow(d.y/.05,2.)-pow(d.x/.5,2.))*.025;
 }
 return finishLight(c);
}
