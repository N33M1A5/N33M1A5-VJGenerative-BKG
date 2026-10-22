vec3 scene(){
 vec2 p=stage();float t=phase();p.x+=.007*sin(p.y*70.+t*3.);float radius=.45+.035*sin(t);float r=length(p);
 float disc=1.-smoothstep(radius-.008,radius+.008,r);float halo=exp(-abs(r-radius)*70.);
 float scan=.88+.12*cos(p.y*620.);vec3 c=vec3(.002)+vec3(.65,.64,.55)*disc*scan;
 c+=vec3(.44,.25,.07)*halo*.35;c*=.8+.2*exp(-dot(p,p)*.8);return finishLight(c);
}
