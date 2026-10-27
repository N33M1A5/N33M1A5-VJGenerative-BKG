vec3 scene(){
 vec2 p=stage();float t=phase();vec2 q=rot(.23)*p;vec3 c=vec3(.01,.005,.025);
 for(int i=0;i<18;i++){float f=float(i);vec2 center=vec2((hash21(vec2(f,2.))-.5)*4.,(hash21(vec2(f,4.))-.5)*2.);
  center+=vec2(.25*sin(t+f),.18*cos(t+f*.8));float r=.18+.28*hash21(vec2(f,seed));
  float disc=exp(-pow(length(q-center)/r,4.));c+=spectral(f*.139)*disc*.30;
 }
 return finishLight(toning(c,p,t));
}
