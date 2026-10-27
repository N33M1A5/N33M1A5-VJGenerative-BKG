vec3 scene(){
 vec2 p=stage();float t=phase();vec2 q=p+vec2(.10*sin(p.y*3.+t),.12*cos(p.x*2.-t));
 float f=cloud(q*2.,t)+cloud(q*4.1,t)*.20;float ink=smoothstep(.58,.61,f);
 float paper=.82+.16*grain(p,t)*detail;float edge=exp(-abs(f-.595)*90.);
 vec3 c=vec3(mix(.002,paper,ink));c+=vec3(.12)*edge;return finishLight(c);
}
