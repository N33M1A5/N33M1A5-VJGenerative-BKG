vec3 scene(){
 vec2 p=stage();float t=phase();vec2 q=p;q.x+=.18*sin(p.y*1.9+t);
 float index=floor((q.x+2.)*4.);float x=fract((q.x+2.)*4.)-.5;
 float face=pow(max(cos(x*3.14159),0.),7.);float y=q.y+.12*sin(t+index);
 float bend=sin(y*4.+index*.6+sin(t));
 vec3 c=vec3(.001,.001,.018)+vec3(.045,.025,.26)*face*(.1+.9*pow(abs(bend),8.));
 c+=vec3(.05,.12,.40)*exp(-pow((x+.24)/.045,2.))*pow(.5+.5*cos(y*2.+t+index),6.);
 return finishLight(c);
}
