vec3 scene(){
 vec2 p=stage();float t=phase();vec3 c=vec3(.015,.001,.003);
 // Rising flame tongues are sampled through a shallow volume, not a flat recolor.
 for(int i=0;i<12;i++){float z=float(i)/12.;vec2 q=p*vec2(1.1+z,1.8)+vec2(.3*sin(t+z),z*.8);
  q.x+=.25*sin(q.y*3.-sin(t)+z*8.);float n=cloud(q*2.,t+z*.3);
  float fire=smoothstep(.47,.72,n-.08*p.y);float hot=pow(fire,3.);
  c+=mix(vec3(.18,.004,.001),vec3(.85,.16,.005),hot)*fire*.23;
  c+=vec3(.65,.32,.04)*pow(fire,8.)*.1;
 }
 return finishLight(c);
}
