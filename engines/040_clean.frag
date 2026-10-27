vec3 scene(){
 vec2 p=stage();float t=phase();float warp=.13*sin(p.y*2.+sin(t))+.1*cos(p.x*2.-cos(t));
 float f=sin(p.x*.9+p.y*.7+warp);float rim=exp(-pow((f-.12)/.2,2.));
 vec3 c=vec3(.35,.37,.33)+vec3(.30,.34,.31)*rim;
 c+=vec3(.05,.015,.025)*spot(p,vec2(.5*cos(t),.5),vec2(1.,1.5));
 c+=vec3(.015,.05,.06)*spot(p,vec2(-.8,-.3*sin(t)),vec2(1.,1.));return finishLight(c);
}
