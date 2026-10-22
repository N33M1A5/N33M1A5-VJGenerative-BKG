vec3 scene(){
 vec2 p=stage();float t=phase();vec2 q=rot(-.035)*p;q.x+=.06*sin(t);
 float letters=typeMask(q,t,true);float channel=.25+.75*pow(.5+.5*cos(q.y*4.+q.x+t),3.);
 vec3 c=vec3(.003,.001,.009)+mix(vec3(.15,.025,.34),vec3(.83,.78,.9),channel)*letters;
 c+=vec3(.08,.015,.18)*letters*cloud(q*3.,t)*depth;
 float scan=.9+.1*cos(q.y*200.);return finishLight(c*scan);
}
