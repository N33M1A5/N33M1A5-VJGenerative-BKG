vec3 scene(){
 vec2 p=stage();float t=phase();vec3 c=mix(vec3(.6,.018,.006),vec3(.72,.17,.05),clamp(p.y*.3+.5,0.,1.));
 c+=vec3(.20,.12,.035)*spot(p,vec2(.7*cos(t),.25),vec2(.85,1.5));
 c=mix(c,vec3(.03,.19,.20),spot(p,vec2(1.0+.25*cos(t),.55),vec2(1.4,1.8))*.8);
 c+=vec3(.13,.05,.05)*spot(p,vec2(-.7,-.6+.2*sin(t)),vec2(.8,1.5));return finishLight(toning(c,p,t));
}
