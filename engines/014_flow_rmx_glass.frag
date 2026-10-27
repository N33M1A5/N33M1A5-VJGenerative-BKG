vec3 scene(){
 vec2 p=stage();float t=phase();vec2 q=rot(-.25+.05*sin(t))*p;
 float warp=.25*sin(q.y*1.6+t)+.12*sin(q.x*2.3-t);
 float x=q.x+warp;float flute=pow(.5+.5*cos(x*(65.+density*50.)),3.);
 vec3 behind=vec3(.015,.025,.07);
 behind+=vec3(.20,.38,.48)*spot(q,vec2(-.6,.6+.2*sin(t)),vec2(.9,1.8));
 behind+=vec3(.55,.10,.08)*spot(q,vec2(.7*cos(t),-.7),vec2(1.,1.));
 float shade=.3+.7*flute;vec3 c=behind*shade;
 c+=vec3(.08,.12,.15)*pow(flute,6.)*(.5+.5*cos(q.y+sin(t)));
 return finishLight(toning(c,p,t));
}
