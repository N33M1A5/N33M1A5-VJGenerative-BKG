vec3 scene(){
 vec2 p=stage();float t=phase();vec2 pixel=floor(p*mix(180.,600.,detail));float h=hash21(pixel+seed);
 float fine=.5+.5*sin(h*100.+sin(t*53.)*31.);float coarse=cloud(p*7.,t);
 float ink=smoothstep(.20,.82,mix(fine,coarse,.30));
 float fibers=.5+.5*sin(p.x*780.+cloud(p*4.,t)*12.);float l=.15+.50*ink+.12*fibers;
 return finishLight(vec3(l));
}
