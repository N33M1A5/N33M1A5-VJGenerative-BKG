vec3 scene(){
 vec2 p=stage();float t=phase();vec2 grid=p*vec2(6.,5.);float row=floor(grid.y);grid.x+=.18*sin(row*.8+t);
 vec2 id=floor(grid),q=fract(grid)-.5;float x=q.x+.18*q.y;
 float panel=(1.-smoothstep(.42,.48,abs(x)))*(1.-smoothstep(.42,.49,abs(q.y)));
 float normal=sin(x*3.14);float face=pow(.5+.5*cos(x*3.14),3.);
 float card=pow(.5+.5*sin(normal*4.+id.x*.5+id.y*.4+sin(t)),8.);
 vec3 reflected=mix(vec3(.015,.05,.10),vec3(.52,.24,.09),.5+.5*sin(id.x*.5+id.y*.7+t));
 vec3 c=vec3(.0005,.001,.003)+reflected*panel*(.025+.45*card)*face;
 float bevel=exp(-pow((x+.36)/.025,2.))*.6+exp(-pow((q.y+.35)/.035,2.))*.2;
 c+=vec3(.16,.25,.36)*bevel*panel*card;c*=.7+.3*face;return finishLight(c);
}
