vec3 scene(){
 vec2 p=stage();float t=phase();vec2 q=p+vec2(.15*sin(p.y*2.+t),.15*sin(p.x*2.-t));
 float n=cloud(q*1.7,t);float detailCloud=cloud(q*6.,t);float ridge=sin(q.y*6.+n*5.);
 vec3 c=mix(vec3(.49,.40,.23),vec3(.025,.31,.35),smoothstep(.24,.7,n));
 c=mix(c,vec3(.63,.61,.44),smoothstep(.1,.8,ridge)*.6);
 c*=.8+.2*detailCloud;c+=grain(p,t)*.012;return finishLight(max(c,0.));
}
