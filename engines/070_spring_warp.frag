vec3 scene(){
 vec2 p=stage();float t=phase();vec2 q=p+vec2(.18*sin(p.y*2.+t),.16*cos(p.x*2.-t));
 float field=sin(q.y*2.+.6*sin(q.x*1.2+sin(t)));
 vec3 c=mix(vec3(.14,.001,.58),vec3(.40,.008,.40),.5+.5*field);
 c+=vec3(.02,.15,.18)*spot(q,vec2(.8,.65+.1*cos(t)),vec2(1.,2.));
 c+=vec3(.12,.015,.32)*spot(q,vec2(-1.,-.5),vec2(.8,1.6));return finishLight(toning(c,p,t));
}
