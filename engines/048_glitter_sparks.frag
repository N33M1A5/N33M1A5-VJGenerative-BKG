vec3 scene(){
 vec2 p=stage();float t=phase();vec3 c=vec3(.0005,.001,.008);
 for(int i=0;i<48;i++){float f=float(i);float h=hash21(vec2(f,seed));float a=h*TAU+t;
  vec2 center=vec2(cos(a),sin(a))*(.2+1.5*hash21(vec2(f,6.)));center.x*=1.5;
  float z=hash21(vec2(f,2.));float size=.008+.038*z;float d=length(p-center);
  float bokeh=exp(-pow(d/size,4.))*.35+exp(-pow(d/(size*.4),2.));
  float flash=pow(.5+.5*sin(t*3.+f*1.7),8.);c+=mix(vec3(.005,.025,.20),vec3(.05,.55,.75),z)*bokeh*(.2+flash);
 }
 return finishLight(c);
}
