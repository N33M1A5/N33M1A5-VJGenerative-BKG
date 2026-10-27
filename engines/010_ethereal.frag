vec3 scene(){
 vec2 p=stage();float t=phase();vec2 q=rot(.35+.08*sin(t))*p;
 float warp=cloud(q*1.2,t);float fold=sin(q.x*2.7+q.y*.8+warp*3.+.5*sin(t));
 float light=exp(-pow((fold-.35)/.30,2.));
 vec3 c=vec3(.008,.012,.016)+mix(vec3(.07,.12,.20),vec3(.42,.46,.48),warp)*light*.6;
 c+=vec3(.3,.36,.45)*spot(q,vec2(.3*cos(t),.55),vec2(1.9,2.8))*.6;
 c*=.5+.5*exp(-dot(p,p)*.35);return finishLight(toning(c,p,t));
}
