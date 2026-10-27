vec3 scene(){
 vec2 p=stage();float t=phase();float row=floor((p.y+1.)*9.);float y=fract((p.y+1.)*9.);
 float curve=.12*sin(p.x*2.2+sin(t)+row*.45);float v=fract(y+curve);
 float face=smoothstep(.08,.14,v)*(1.-smoothstep(.66,.94,v));
 float light=pow(.5+.5*sin(p.x*5.+cos(t)+row*.8),3.);
 vec3 c=vec3(.012,.001,.0)+vec3(1.,.24,.002)*face*(.05+.9*light);
 c+=vec3(.7,.09,.001)*exp(-pow((v-.25)/.07,2.))*light;
 return finishLight(c);
}
