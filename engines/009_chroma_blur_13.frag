vec3 scene(){
 vec2 p=stage();float t=phase();vec3 c=vec3(.012,.018,.023);
 float band=exp(-pow((p.y-.12-.15*sin(t))/.44,2.));
 c+=vec3(.70,.035,.13)*band*(.5+.5*cos(p.x*1.2+t));
 c+=vec3(.13,.5,.43)*spot(p,vec2(.75*cos(t),-.17),vec2(2.,1.25));
 c+=vec3(.68,.37,.04)*spot(p,vec2(-.7+.2*sin(t),.5),vec2(1.1,2.7));
 float streak=.5+.5*cos(p.x*9.+.8*sin(t+p.y));c+=vec3(.04,.11,.10)*streak*exp(-pow((p.y+.6)/.35,2.));
 c*=.85+.15*cloud(p*.7,t);return finishLight(toning(c,p,t));
}
