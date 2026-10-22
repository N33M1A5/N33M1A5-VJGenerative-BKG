vec3 scene(){
 vec2 p=stage();float t=phase();vec2 q=rot(-.40)*p;
 for(int i=0;i<5;i++)q+=.14*vec2(sin(q.y*2.1+float(i)+sin(t)),sin(q.x*1.9-float(i)+cos(t)));
 float height=cloud(q*2.,t);float strata=sin((q.x*.7+q.y*.15+height*1.9)*60.);
 float veins=pow(.5+.5*strata,8.);vec3 c=mix(vec3(.018,.03,.07),vec3(.24,.32,.48),height);
 c+=vec3(.25,.34,.48)*veins*.8;c*=.45+.55*cloud(q*10.,t);return finishLight(c);
}
