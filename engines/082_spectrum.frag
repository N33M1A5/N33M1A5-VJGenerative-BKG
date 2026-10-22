vec3 scene(){
 vec2 p=stage();float t=phase();vec2 q=rot(-.6+.05*sin(t))*p;
 float warp=.15*sin(q.y*2.3+t)+.10*cloud(q*2.,t);float band=pow(.5+.5*sin((q.x+warp)*8.),5.);
 vec3 c=spectral(q.x*.4+.1*sin(t))*band*.36;c+=vec3(.004,.007,.025);
 c*=.5+.5*exp(-dot(p,p)*.35);return finishLight(toning(c,p,t));
}
