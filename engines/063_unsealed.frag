vec3 scene(){
 vec2 p=stage();float t=phase();vec2 q=rot(.035*sin(t))*p;float edge=.035;
 float vertical=1.-smoothstep(.14,.14+edge,abs(q.x));vertical*=1.-smoothstep(.65,.69,abs(q.y));
 float horizontal=(1.-smoothstep(.40,.44,abs(q.x)))*(1.-smoothstep(.13,.17,abs(q.y-.18)));
 float crossShape=max(vertical,horizontal);float halo=exp(-max(boxSDF(q-vec2(0,.18),vec2(.38,.10)),0.)*13.);
 vec3 c=vec3(.001,.003,.006)+vec3(.9,1.,.83)*crossShape;
 c+=vec3(.09,.30,.45)*halo*.12;c+=vec3(.10,.14,.18)*exp(-pow((q.y+.68)/.12,2.))*(.5+.5*sin(q.x*4.+t));
 c+=vec3(.45,.06,.01)*exp(-pow((abs(q.x)-.16)/.025,2.))*vertical*.2;
 return finishLight(c);
}
