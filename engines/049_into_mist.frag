float pine(vec2 p,float h){
 float trunk=1.-smoothstep(.005,.016,abs(p.x));trunk*=step(0.,p.y)*step(p.y,h);
 float tree=0.;for(int k=0;k<9;k++){float f=float(k);float y=h*(.12+f*.09);float width=h*(.32-f*.033);
  float d=max(abs(p.x)/max(width,.01)+(p.y-y)/(h*.20)-1.,max(y-p.y,p.y-y-h*.20));tree=max(tree,1.-smoothstep(0.,.02,d));}
 return max(tree,trunk*.7);
}
vec3 scene(){
 vec2 p=stage();float t=phase();vec3 c=vec3(.30,.32,.40);
 for(int layer=0;layer<5;layer++){float l=float(layer);float depthFade=.25+l*.13;
  float drift=.025*sin(t+l);vec2 q=p+vec2(drift,0.);float spacing=.36-l*.035;float id=floor((q.x+3.)/spacing);float x=mod(q.x+3.,spacing)-spacing*.5;
  float base=-.70-l*.035+.20*noise2(vec2(id*1.7,l));float height=.35+hash21(vec2(id,l))*mix(.35,.65,l/4.);
  float tree=pine(vec2(x,q.y-base),height);vec3 color=mix(vec3(.25,.28,.32),vec3(.015,.035,.026),depthFade);
  float fog=.35+.35*cloud(p*vec2(1.2,2.)+l,t);c=mix(c,color,tree*(1.-fog*(1.-l*.15))*.8);
 }
 c=mix(c,vec3(.35,.37,.43),spot(p,vec2(.5*sin(t),-.1),vec2(.5,1.4))*.5);return finishLight(c);
}
