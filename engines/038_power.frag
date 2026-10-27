vec3 scene(){
 vec2 p=stage();float t=phase();vec2 center=vec2(.5*sin(t),-.35+.15*cos(t));vec2 q=p-center;
 float r=length(q),a=atan(q.y,q.x);float flare=pow(.5+.5*sin(a*3.+sin(t)),8.)*exp(-r*1.3);
 vec3 c=vec3(.006,.005,.018)+vec3(.85,.34,.045)*exp(-r*r*10.);
 c+=vec3(.6,.14,.02)*flare*.25;c+=vec3(.25,.065,.014)*exp(-abs(q.y)*11.)*exp(-abs(q.x)*1.2);
 c+=vec3(.07,.04,.08)*spot(p,-center,vec2(1.3,2.));return finishLight(c);
}
