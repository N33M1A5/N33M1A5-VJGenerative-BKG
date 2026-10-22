vec3 scene(){
 vec2 p=stage();float t=phase();vec2 q=rot(.35+.04*sin(t))*p;
 float y=q.y+.20*sin(q.x*1.1+t);vec3 c=vec3(.008,.006,.033);
 c+=vec3(.015,.36,.34)*exp(-pow((y-.48)/.35,2.));
 c+=vec3(.48,.33,.09)*exp(-pow((y+.38)/.13,2.));
 c+=vec3(.12,.03,.17)*exp(-pow((y+.60)/.32,2.));return finishLight(toning(c,p,t));
}
