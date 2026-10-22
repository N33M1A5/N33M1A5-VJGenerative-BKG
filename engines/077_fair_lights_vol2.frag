vec3 scene(){
 vec2 p=stage();float t=phase();vec2 q=rot(.3+.04*sin(t))*p;q.x+=.10*sin(q.y*4.+t);
 vec2 grid=q*vec2(70.,50.);vec2 f=fract(grid)-.5;float led=exp(-dot(f,f)*34.);
 float bands=.5+.5*sin(q.y*6.+.6*sin(t));float white=pow(.5+.5*sin(q.y*7.+q.x*.8+sin(t)),12.);
 vec3 c=mix(vec3(.008,.25,.45),vec3(.55,.008,.20),bands)*(.3+.7*led);
 c+=vec3(.7,.85,.85)*white*(.4+.6*led);return finishLight(c);
}
