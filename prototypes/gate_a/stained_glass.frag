// A layered architectural rose window, entirely procedural.
float glassCell(vec2 g,out vec2 best){
 vec2 cell=floor(g),f=fract(g);float d1=10.,d2=10.;best=cell;
 for(int y=-1;y<=1;y++)for(int x=-1;x<=1;x++){
  vec2 id=cell+vec2(x,y);
  vec2 o=.15+.7*vec2(hash21(id),hash21(id+31.7));
  float d=length(vec2(x,y)+o-f);
  if(d<d1){d2=d1;d1=d;best=id;}else d2=min(d2,d);
 }
 return d2-d1;
}
float foil(vec2 p,float radius,float lobes){
 float a=atan(p.y,p.x),r=length(p);
 return abs(r-radius*(.82+.18*cos(a*lobes)));
}
vec3 scene(){
 float t=phase();vec2 p=canvas();
 // The architecture stays anchored; light travels behind the glass.
 p*=1.15+.018*sin(t);p=rot(.008*sin(t))*p;
 float r=length(p),a=atan(p.y,p.x);
 float sectors=12.+2.*floor(symmetry*4.);
 float wedge=TAU/sectors;
 float local=mod(a+wedge*.5,wedge)-wedge*.5;
 vec2 q=vec2(sin(local)*r,cos(local)*r);
 float aa=max(fwidth(q.x),.0012);
 // Inner lancets taper to the central boss and close with pointed crowns.
 float y=clamp((q.y-.13)/.69,0.,1.);
 float width=.132*pow(max(sin(y*3.14159265),0.),.60)*(1.+.12*sin(y*9.));
 float innerD=max(abs(q.x)-width,max(.13-q.y,q.y-.82));
 float inner=1.-smoothstep(-aa,aa,innerD);
 // Long outer panels, with pointed tops, divided by heavy stone spokes.
 float oy=clamp((q.y-.90)/.95,0.,1.);
 float ow=.18*pow(max(sin(oy*3.14159265),0.),.32);
 float outerD=max(abs(q.x)-ow,max(.90-q.y,q.y-1.85));
 float outer=1.-smoothstep(-aa,aa,outerD);

 float edge=exp(-abs(innerD+.006)*210.)*inner+exp(-abs(outerD+.006)*180.)*outer;
 // Quatrefoils and small rose medallions occupy the masonry between lancets.
 float halfLocal=mod(a,wedge)-wedge*.5;
 vec2 lace=vec2(sin(halfLocal)*r,cos(halfLocal)*r-1.71);
 float laceA=atan(lace.y,lace.x),laceR=length(lace);
 float floralR=.17*(.82+.18*cos(laceA*6.));
 float medallion=1.-smoothstep(-aa,aa,laceR-floralR);
 float tracery=exp(-foil(lace,.12,6.)*200.)+exp(-abs(laceR-.042)*210.);
 float ribs=1.-smoothstep(.005,.010,abs(sin(laceA*3.))*laceR);
 // Smaller ornaments between the long panels and inner ring.
 vec2 smallQ=vec2(sin(halfLocal)*r,cos(halfLocal)*r-1.05);
 float smallD=foil(smallQ,.077,4.);
 float tiny=exp(-smallD*200.);
 vec2 best;vec2 mosaic=q*vec2(21.,20.)*mix(.65,1.5,detail)*mix(.75,1.2,density)+seed;
 float cell=glassCell(mosaic,best);
 float lead=smoothstep(.018,.075,cell);
 float marble=.35+.65*fbm(q*22.+seed);
 // Decorative branching leadwork is separate from the broken glass mosaic.
 float stem=abs(q.x-(.018*sin(q.y*42.)));
 float leaf=abs(abs(q.x)-.055*abs(sin(q.y*24.)));
 float botanical=min(stem,leaf);
 float veins=smoothstep(.002,.007,botanical);
 float centerMullion=smoothstep(.006,.018,abs(q.x));
 float transverse=smoothstep(.005,.015,abs(mod(q.y+.06,.25)-.125));
 float came=lead*mix(.35,1.,veins)*mix(1.,centerMullion*transverse,.70*outer);
 float field=.5+.5*sin(a*3.+r*4.5+.5*sin(t));
 vec3 glass=mix(vec3(.16,.015,.30),vec3(.62,.006,.23),field);
 float cyan=pow(.5+.5*sin(a*5.-r*4.+t),9.);
 glass=mix(glass,vec3(.015,.42,.58),cyan*.95);
 float light=.06+1.8*pow(.5+.5*cos(a*2.+t+r*2.1),9.);
 light*=.45+.55*fbm(p*2.8+vec2(cos(t),sin(t))*.8);
 float innerSpine=smoothstep(.009,.017,abs(q.x));
 float pane=inner*innerSpine+outer*.65+medallion*.28;
 vec3 c=glass*pane*came*(.16+.84*marble)*light;
 c+=glass*edge*.17*light;
 c+=glass*medallion*(tracery*.08+ribs*.013)*light;
 c+=glass*tiny*.06;
 // Strong structural shadows and a subtle, separately moving light veil.
 float boss=1.-smoothstep(.075,.10,r);c*=1.-boss;
 float ring=exp(-abs(r-.105)*240.);c+=glass*ring*.04;
 c*=.35+.65*exp(-r*r*.4);
 float veil=pow(.5+.5*cos(a*sectors+.4*sin(t)),18.)*exp(-r*.85);
 c+=glass*veil*.022*depth;
 return finishLight(c*2.4);
}
