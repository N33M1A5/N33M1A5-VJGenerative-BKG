vec3 scene(){
 vec2 p=stage();float t=phase();vec2 q=p*vec2(1.1,2.3);q.y+=.18*sin(t);
 float n=cloud(q,t);float billow=cloud(q*1.8+vec2(2.,-.3),t);
 float shape=n+.25*billow-.38-.12*sin(p.x*2.4+sin(t));
 float flame=smoothstep(-.12,.12,shape);float hot=pow(max(shape+.12,0.),3.)*4.;
 vec3 c=vec3(.035,.005,.002)+mix(vec3(.26,.007,.002),vec3(.72,.055,.001),flame)*flame;
 c+=vec3(.72,.26,.018)*hot*flame;c*=.55+.45*cloud(p*8.,t);
 return finishLight(c);
}
