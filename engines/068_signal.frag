vec3 scene(){
 vec2 p=stage();float t=phase();vec3 c=vec3(.001,.003,.008);
 for(int layer=0;layer<4;layer++){float z=float(layer);vec2 q=p*(1.+z*.5)+vec2(.07*sin(t+z),.04*cos(t+z));
  q=rot(.18)*q;vec2 grid=q*vec2(11.,8.);vec2 id=floor(grid),f=fract(grid)-.5;
  float dotLight=exp(-dot(f,f)*150.);float hot=pow(.5+.5*sin(t+id.x*.7+id.y*.8+z),4.);
  c+=mix(vec3(.04,.12,.19),vec3(.62,.16,.014),hash21(id+seed))*dotLight*(.1+hot)/(1.+z);
 }
 return finishLight(c);
}
