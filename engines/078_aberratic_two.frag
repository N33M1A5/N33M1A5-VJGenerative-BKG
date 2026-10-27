vec3 scene(){
 vec2 p=stage();float t=phase();vec2 q=rot(-.06)*p;float displace=.10*sin(q.y*6.+t)+.08*cloud(q*2.,t);
 float streak=cloud(vec2(q.x*.7,q.y*18.)+displace,t);float band=pow(smoothstep(.32,.75,streak),3.);
 vec3 c=vec3(.002,.002,.009)+spectral(q.y*.4+.1*sin(t))*band*.22;
 c+=vec3(.13,.09,.035)*exp(-pow((q.y+.25+.1*sin(t))/.07,2.))*pow(.5+.5*cos(q.x*1.4+t),4.);
 return finishLight(toning(c,p,t));
}
