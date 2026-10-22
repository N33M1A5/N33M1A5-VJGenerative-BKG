vec3 scene(){
 vec2 p=stage();float t=phase();float n=6.+floor(density*8.);vec2 cell=floor(p*n);vec2 f=fract(p*n)-.5;
 float id=hash21(cell+seed);float on=smoothstep(.84,.995,.5+.5*sin(t*2.+id*TAU));
 float d=boxSDF(f,vec2(.43));float tile=1.-smoothstep(-.02,.025,d);
 float hot=pow(on,4.);vec3 color=mix(vec3(.10,.018,.008),vec3(1.1,.76,.51),hot);
 vec3 c=color*tile*on;c+=vec3(.16,.05,.015)*exp(-max(d,0.)*10.)*hot*.25;
 return finishLight(c);
}
