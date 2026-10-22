vec3 scene(){
 vec2 p=stage();float t=phase();vec2 q=rot(.45)*p;
 float fold=q.x+.27*sin(q.y*2.4+sin(t))+.14*sin(q.x*3.-cos(t));
 float ridge=pow(.5+.5*cos(fold*9.),10.);float n=cloud(q*2.,t);
 vec3 c=mix(vec3(.02,.05,.15),vec3(.45,.05,.009),smoothstep(.25,.75,n));
 c*=.3+.7*ridge;c+=vec3(.55,.65,.65)*pow(ridge,4.)*.65;
 c+=vec3(.03,.12,.25)*exp(-pow((fold-.2)/.08,2.));c+=grain(p,t)*.012;return finishLight(max(c,0.));
}
