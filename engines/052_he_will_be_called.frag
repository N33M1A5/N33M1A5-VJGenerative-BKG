vec3 scene(){
 vec2 p=stage();float t=phase();vec3 c=vec3(.002,.007,.016);float r=length(p),a=atan(p.y,p.x);
 for(int i=0;i<5;i++){float f=float(i);float radius=.26+f*.27;float ring=lineAA(r-radius,.0015);
  c+=vec3(.14,.10,.045)*ring*.25;
  for(int k=0;k<5;k++){float b=float(k)*TAU/5.+f*.74+.08*sin(t+f);vec2 center=vec2(cos(b),sin(b))*radius;
   float s=.008+.003*sin(t*2.+float(k)+f);c+=vec3(.75,.53,.24)*star(p-center,s)*.45;
  }
 }
 c+=vec3(.40,.30,.16)*lineAA(p.x,.001)*exp(-abs(p.y)*2.)*.2;return finishLight(c);
}
