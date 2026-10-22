vec3 scene(){
 vec2 p=stage();float t=phase();float count=18.+floor(density*18.);float col=floor((p.x+2.)*count*.25);
 float x=fract((p.x+2.)*count*.25);float height=.35+1.15*(.5+.5*sin(t+col*1.77));
 float bar=(1.-smoothstep(.70,.85,x))*smoothstep(.02,.08,x)*smoothstep(.02,.06,height-(p.y+1.));
 float side=smoothstep(.66,.73,x)*(1.-smoothstep(.88,.92,x));
 vec3 c=vec3(.001,.003,.025)+vec3(.0,.28,.52)*bar*(.4+.6*height);
 c+=vec3(.015,.025,.16)*side;c+=vec3(.0,.10,.20)*exp(-abs(height-p.y-1.)*24.)*bar;
 return finishLight(c);
}
