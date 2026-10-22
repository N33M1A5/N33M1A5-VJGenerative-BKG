vec3 scene(){
 vec2 p=stage();float t=phase();float r=length(p-vec2(.35,-.1));float a=atan(p.y+.1,p.x-.35);
 float nebula=cloud(p*1.9+vec2(.2*cos(t),.1*sin(t)),t);
 float shell=exp(-pow((r-.67-.06*sin(a*3.+t))/.17,2.));
 vec3 c=vec3(.001,.005,.015)+vec3(.02,.14,.30)*shell*(.15+nebula*1.4);
 c+=vec3(.15,.43,.61)*pow(nebula,5.)*2.;
 vec2 grid=floor(p*110.);float h=hash21(grid+seed);float point=pow(max(0.,1.-length(fract(p*110.)-.5)*3.),8.);
 c+=vec3(.30,.50,.70)*point*step(.985,h)*(.4+.6*sin(t+h*50.)*sin(t+h*50.));
 c*=1.-.7*(1.-smoothstep(.55,.70,r));return finishLight(c);
}
