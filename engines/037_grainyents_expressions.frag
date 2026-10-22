vec3 scene(){
 vec2 p=stage();float t=phase();float q=cloud(p*.55+vec2(.3*cos(t),.3*sin(t)),t);
 vec3 c=mix(vec3(.025,.045,.075),vec3(.21,.30,.20),smoothstep(.2,.8,q));
 float veil=spot(p,vec2(-.7+.45*sin(t),-.55),vec2(.75,1.));c+=vec3(.16,.14,.09)*veil;
 c+=vec3(.10,.04,.09)*spot(p,vec2(1.,.4*cos(t)),vec2(1.,1.));c+=grain(p,t)*(.035+.04*detail);
 return finishLight(max(c,0.));
}
