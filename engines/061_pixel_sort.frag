vec3 scene(){
 vec2 p=stage();float t=phase();float column=floor(p.x*(70.+density*100.));float h=hash21(vec2(column,seed));
 float displacement=.40*sin(t+h*TAU);float stretched=p.y*.13+displacement+h*.2;
 vec3 c=mix(vec3(.07,.001,.12),vec3(.015,.002,.40),smoothstep(-.4,.7,p.x));
 float streak=pow(.5+.5*sin(stretched*9.+p.x*2.),4.);c+=mix(vec3(.45,.005,.12),vec3(.025,.025,.38),h)*streak;
 c*=.5+.5*pow(h,.4);return finishLight(c);
}
