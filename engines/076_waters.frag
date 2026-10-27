vec3 scene(){
 vec2 p=stage();float t=phase();float perspective=1./max(.22,p.y+1.35);vec2 q=vec2(p.x*perspective, perspective);
 float ripples=sin(q.y*20.+sin(t)+sin(q.x*4.))*sin(q.x*7.+cos(t));
 float shine=pow(.5+.5*ripples,18.);float reflection=exp(-pow((p.x-.25*sin(t))/.5,2.));
 vec3 c=vec3(.001,.002,.012)+vec3(.015,.01,.08)*(.5+.5*cloud(q,t));
 c+=mix(vec3(.03,.015,.18),vec3(.35,.035,.20),reflection)*shine*reflection*.5;return finishLight(c);
}
