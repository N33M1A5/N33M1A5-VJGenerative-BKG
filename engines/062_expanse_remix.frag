vec3 scene(){
 vec2 p=stage();float t=phase();vec2 q=p;q.y+=.18*sin(p.x*2.4+t)+.08*sin(p.x*5.-t);
 vec2 grid=q*vec2(45.,29.);vec2 cell=floor(grid),f=fract(grid)-.5;float dotLight=exp(-dot(f,f)*70.);
 float water=.5+.5*sin(q.y*5.+.8*cloud(q*2.,t));vec3 c=mix(vec3(.005,.02,.11),vec3(.32,.003,.07),water)*dotLight;
 c*=.25+.75*pow(.5+.5*cos(q.x*1.8+q.y*2.+t),2.);return finishLight(c*2.);
}
