vec3 scene(){
 vec2 p=stage();float t=phase();vec3 c=vec3(.001,.0002,.004);
 for(int burst=0;burst<3;burst++){float b=float(burst);float age=.5+.5*sin(t+b*TAU/3.);float radius=.12+age*1.2;
  vec2 center=vec2(.45*sin(b*2.7),.25*cos(b*1.9));
  for(int i=0;i<30;i++){float f=float(i);float a=f*TAU/30.+.12*sin(t+b)+.2*hash21(vec2(f,b));
   vec2 dir=vec2(cos(a),sin(a));vec2 q=p-center-dir*radius;float along=dot(q,dir),across=dot(q,vec2(-dir.y,dir.x));
   across+=.018*sin(along*34.+f+sin(t));
   float trail=exp(-pow(across/.012,2.))*exp(-pow(along/(.08+.20*age),2.));
   vec3 color=mix(vec3(.44,.006,.17),vec3(.13,.05,.65),hash21(vec2(f,seed)));
   c+=color*trail*(.3+.7*sin(age*3.14159))*1.7;
   c+=color*exp(-pow(across/.05,2.)-pow(along/.22,2.))*.08;
  }
 }
 return finishLight(c);
}
