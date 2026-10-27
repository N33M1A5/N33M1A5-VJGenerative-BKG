vec3 scene(){
 vec2 p=stage();float t=phase();float age=.5+.5*sin(t);vec3 c=vec3(.001);float transmission=1.;
 vec3 ro=vec3(0,0,-2.5);vec3 rd=normalize(vec3(p*.60,1.));float radius=.55+age*.9;
 for(int i=0;i<16;i++){float z=float(i)/16.;vec3 pos=ro+rd*(1.55+z*2.0);
  float turbulence=f3(pos*4.+vec3(cos(t),sin(t),0.));float body=1.-smoothstep(radius*.55,radius*1.1,length(pos)+.3*turbulence);
  float dust=body*smoothstep(.25,.75,turbulence)*.18;
  float key=.5+.5*sin(pos.x*2.4+pos.y*1.2+turbulence*2.);
  vec3 tint=grad3(key,vec3(.015,.14,.42),vec3(.32,.015,.23),vec3(.65,.10,.015));
  c+=transmission*tint*dust*(.6+.9*turbulence);transmission*=1.-dust;
 }
 vec2 px=floor(p*340.);float particles=pow(max(0.,sin(hash21(px+seed)*80.+t*3.)),35.);
 float cloudMask=exp(-dot(p,p)/(radius*radius));c+=spectral(cloud(p,t))*.10*particles*cloudMask;
 return finishLight(c*3.5);
}
