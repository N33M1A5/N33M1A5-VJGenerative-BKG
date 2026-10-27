vec3 scene(){
 vec2 p=stage();float t=phase();float r=length(p);float a=atan(p.y,p.x)+.10*sin(t);
 vec2 q=vec2(a*8.,r*20.+.8*sin(t));vec2 f=fract(q)-.5;float dotLight=exp(-dot(f,f)*75.);
 float torus=exp(-pow((r-.7-.09*sin(a*3.+t))/.38,2.));
 float dropout=step(.48,hash21(floor(q)+seed));vec3 c=spectral(a/TAU+r*.5)*dotLight*torus*dropout*.4;
 c*=.3+.7*pow(.5+.5*sin(t*3.+floor(q.y)*.6),3.);
 c+=vec3(.002,.003,.01);return finishLight(c);
}
