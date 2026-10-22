vec3 scene(){
 vec2 p=stage();float t=phase();vec3 c=vec3(.004,.018,.05);
 c+=vec3(.42,.01,.06)*spot(p,vec2(-1.+.24*sin(t),.45),vec2(.9,1.7));
 c+=vec3(.06,.16,.12)*spot(p,vec2(.15*cos(t),-.15),vec2(1.1,1.3));
 c+=vec3(.32,.38,.02)*spot(p,vec2(1.1+.2*cos(t),.2),vec2(1.5,2.));
 c+=vec3(.16,.09,.03)*spot(p,vec2(-1.2,-.35+.15*cos(t)),vec2(1.6,1.3));
 return finishLight(toning(c,p,t));
}
