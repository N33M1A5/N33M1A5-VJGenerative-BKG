vec3 scene(){
 vec2 p=stage();float t=phase();float r=length(p),a=atan(p.y,p.x);vec2 q=rot(.8*exp(-r*.7)+.15*sin(t))*p;
 for(int i=0;i<4;i++)q+=.12*vec2(sin(q.y*4.+sin(t)+float(i)),cos(q.x*4.+cos(t)-float(i)));
 float field=cloud(q*4.,t);float wave=sin(q.x*13.+q.y*8.+field*8.);float caustic=pow(.5+.5*wave,12.);
 vec3 c=mix(vec3(.005,.055,.16),vec3(.015,.34,.43),field);c+=vec3(.05,.48,.55)*caustic*.65;
 c*=.7+.3*exp(-r*.4);return finishLight(c);
}
