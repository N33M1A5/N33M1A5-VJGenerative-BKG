// Architectural church windows: fixed pointed arches, stone and leaded panes.
float churchArch(vec2 p,float w,float bottom,float shoulder){
 float side=abs(p.x)-w;
 float crown=max(length(p-vec2(-w,shoulder)),length(p-vec2(w,shoulder)))-2.*w;
 return max(p.y>shoulder?crown:side,bottom-p.y);
}
vec3 scene(){
 float t=phase();vec2 p=canvas();p.y+=.025*sin(t);p.x+=.008*sin(t);
 float bayWidth=1.04;float index=floor((p.x+bayWidth*.5)/bayWidth);vec2 q=vec2(p.x-index*bayWidth,p.y);
 float w=.43;float arch=churchArch(q,w,-1.03,.19);
 float aa=max(fwidth(arch),.001);
 float inside=1.-smoothstep(-aa,aa,arch);
 float bevel=exp(-abs(arch+.018)*100.);
 float ribs=smoothstep(.018,.027,abs(q.x-.145))*smoothstep(.018,.027,abs(q.x+.145));
 float transom=smoothstep(.015,.024,abs(q.y+.48))*smoothstep(.014,.022,abs(q.y+.06));
 vec2 g=q*vec2(21.,18.)*mix(.8,1.4,detail);vec2 cell=floor(g),f=fract(g),winner=cell;
 float d1=10.,d2=10.;
 for(int y=-1;y<=1;y++)for(int x=-1;x<=1;x++){
  vec2 id=cell+vec2(x,y);vec2 o=.15+.7*vec2(hash21(id),hash21(id+9.1));float d=length(vec2(x,y)+o-f);
  if(d<d1){d2=d1;d1=d;winner=id;}else d2=min(d2,d);
 }
 float lead=smoothstep(.018,.08,d2-d1);
 float glassTexture=.50+.50*fbm(q*34.+seed);
 float key=hash21(winner+seed+index*17.);
 vec3 blue=vec3(.016,.10,.34),red=vec3(.48,.012,.035),gold=vec3(.62,.29,.035);
 vec3 tint=mix(blue,red,smoothstep(.2,.7,key));tint=mix(tint,gold,pow(key,5.));
 float pattern=.5+.5*sin(q.y*18.+sin(q.x*23.)*.7+index);
 tint=mix(tint,vec3(.03,.32,.39),pow(pattern,10.)*.5);
 // Upper quatrefoil: a small ornament inside each pointed crown, not a radial scene.
 vec2 rosette=q-vec2(0,.53);float roseD=10.;
 for(int k=0;k<4;k++){float a=float(k)*TAU/4.;roseD=min(roseD,length(rosette-.073*vec2(cos(a),sin(a)))-.076);}
 float rose=1.-smoothstep(-.003,.003,roseD);float rim=exp(-abs(roseD)*160.);
 float ornamental=1.-smoothstep(.005,.014,abs(q.x-.045*sin(q.y*27.)));
 float transmitted=.35+1.4*exp(-dot((p-vec2(.85*cos(t),.4+.2*sin(t)))*vec2(.7,.6),(p-vec2(.85*cos(t),.4+.2*sin(t)))*vec2(.7,.6)));
 vec3 glass=tint*lead*glassTexture*ribs*transom*(1.-ornamental*.35);
 glass=mix(glass,vec3(.34,.11,.022)*lead,.4*rose);
 vec3 c=glass*inside*transmitted*2.;c+=tint*bevel*.24*inside;c*=1.-rim*.8;
 // Aged stone has thickness, a pale inner bevel and very low ambient illumination.
 float masonry=.45+.55*fbm(p*12.+seed);
 vec3 stone=vec3(.013,.016,.023)*masonry;
 stone+=vec3(.020,.023,.027)*exp(-max(arch,0.)*45.);
 c+=stone*(1.-inside);
 // Broad, oblique transmitted light veil; the structure never rotates or folds.
 float veil=exp(-pow((p.x+p.y*.45-.65*cos(t))/.6,2.));
 c+=vec3(.018,.025,.035)*veil*depth*.35;
 c*=.55+.45*exp(-dot(p*vec2(.35,.2),p*vec2(.35,.2)));
 c*=mix(.85,1.15,density);c=mix(c,vec3(dot(c,vec3(.22,.65,.13))),symmetry*.25);
 return finishLight(c);
}
