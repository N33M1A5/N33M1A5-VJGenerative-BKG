vec3 scene(){
 vec2 p=stage();float t=phase();vec2 q=p;q.x+=.035*sin(q.y*5.+t);
 float box=boxSDF(q,vec2(.59,.72));float window=1.-smoothstep(-.02,.01,box);float edge=exp(-abs(box)*70.);
 float flow=cloud(q*vec2(2.2,1.5),t);vec3 light=mix(vec3(.04,.22,.27),vec3(.62,.90,.89),smoothstep(.25,.7,flow));
 vec3 c=vec3(.001,.005,.007)+light*window*.6;c+=vec3(.06,.24,.28)*edge*.6;
 c+=vec3(.04,.13,.14)*exp(-max(box,0.)*6.)*.08;return finishLight(c);
}
