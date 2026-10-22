vec3 scene(){
 vec2 p=stage();float t=phase();float r=length(p),a=atan(p.y,p.x);float angle=a+.35*sin(t)+.3*cloud(p,t);
 float bands=pow(.5+.5*cos(angle*4.+r*2.),4.);vec3 c=spectral(angle/TAU+.14*sin(t))*bands*exp(-r*.6)*.45;
 c+=vec3(.34,.08,.025)*spot(p,vec2(-.9,.5),vec2(1.,1.));return finishLight(toning(c,p,t));
}
