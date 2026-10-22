vec3 scene(){
 vec2 p=stage();float t=phase();float x=p.x+.10*sin(p.y*5.+sin(t));
 float contour=sin((p.y+.14*sin(x*2.+t))*20.+cos(t));
 float cut=floor((contour*.5+.5)*6.)/6.;
 vec3 c=mix(vec3(.50,.003,.025),vec3(.01,.055,.20),smoothstep(-1.2,1.2,x));
 c*=.65+.35*cut;return finishLight(toning(c,p,t));
}
