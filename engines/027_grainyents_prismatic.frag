vec3 scene(){
 vec2 p=stage();float t=phase();float x=p.x*.25+.1*sin(t)+.04*sin(p.y*2.+t);
 vec3 c=grad3(x+.5,vec3(.03,.10,.24),vec3(.31,.17,.23),vec3(.025,.035,.14));
 c+=vec3(.12,.10,.10)*exp(-pow((x+.1)/.25,2.));
 c+=grain(p,t)*(.018+.035*detail);return finishLight(max(c,0.));
}
