vec3 scene(){
 vec2 p=stage();float t=phase();float a=atan(p.y,p.x),r=length(p);vec2 q=rot(.06*sin(t))*p;
 float n=cloud(q*2.,t),filament=cloud(q*7.+n,t);
 float shell=exp(-pow((r-.92-.15*sin(a*3.+n*4.))/.28,2.));
 vec3 c=vec3(.001,.003,.012)+vec3(.05,.045,.19)*n*.3;
 c+=mix(vec3(.11,.03,.22),vec3(.6,.19,.14),filament)*shell*pow(filament,2.)*2.;
 vec2 g=floor(p*140.);c+=vec3(.45,.60,.8)*step(.996,hash21(g+seed))*pow(max(0.,1.-length(fract(p*140.)-.5)*3.),9.);
 return finishLight(c);
}
