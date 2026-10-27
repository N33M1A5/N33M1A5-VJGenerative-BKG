vec3 scene(){
 vec2 p=stage();float t=phase();float y=clamp(p.y*.55+.48+.025*sin(t),0.,1.);
 vec3 c=grad3(y,vec3(.003,.008,.02),vec3(.17,.018,.33),vec3(.72,.25,.83));
 float g=grain(p,t)*mix(.003,.035,detail);c+=g;
 return finishLight(max(c,0.));
}
