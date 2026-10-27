vec3 scene(){
 vec2 p=stage();float t=phase();vec2 cell=floor(p/vec2(.78,.90)+.5);vec2 q=repeat2(p,vec2(.78,.90));
 float radius=.345;float r=length(q);float d=r-radius;float rim=1.-smoothstep(-.009,.005,d);
 float z=sqrt(max(radius*radius-r*r,0.))/radius;vec3 n=normalize(vec3(q/radius,z));
 vec3 light=normalize(vec3(.65*sin(t+cell.x*.4),.8*cos(t+cell.y),.12));
 float lit=pow(max(dot(n,light),0.),2.);float fres=pow(1.-z,3.);
 vec3 c=vec3(.0005,.001,.003)+rim*(vec3(.018,.07,.20)*lit+vec3(.12,.3,.6)*lit*fres*3.);
 return finishLight(c);
}
