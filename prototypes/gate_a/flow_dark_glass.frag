// Perspective height-field intersection: folds can occlude one another.
float surface(vec2 p,float t){
 return .42*sin(p.x*1.15+p.y*.78+.9*sin(t))
       +.23*sin(p.x*1.8-p.y*1.1+.8*cos(t))
       +.08*sin(p.x*3.5+p.y*2.1+.55*sin(t+1.));
}
vec3 environment(vec3 d,float t){
 float a=atan(d.x,d.z),y=d.y;
 float warm=exp(-pow((y-.32+.10*sin(a*2.+t))/.13,2.));
 float cool=exp(-pow((y-.61-.11*cos(a*3.))/.095,2.));
 float rim=exp(-pow((y-.12)/.025,2.))*.18;
 return vec3(.0001,.00015,.0002)+vec3(.75,.57,.32)*warm+vec3(.16,.25,.42)*cool+vec3(.7,.8,.9)*rim;
}
vec3 scene(){
 float t=phase();vec2 p=canvas();
 p.x=mix(p.x,abs(p.x),symmetry);
 vec3 ro=vec3(.1*sin(t),1.35,-3.2);
 vec3 forward=normalize(vec3(.08,-.72,1.));
 vec3 right=normalize(cross(forward,vec3(0,1,0)));
 vec3 up=cross(right,forward);
 vec3 rd=normalize(forward+right*p.x*.38+up*p.y*.38);
 float travel=.1,hit=0.;vec3 pos=ro;
 float freq=mix(.65,1.4,density),amp=mix(.65,1.35,depth);
 // Bounded conservative steps for this smooth height field.
 for(int i=0;i<80;i++){
  pos=ro+travel*rd;
  float dist=pos.y-surface(pos.xz*freq+seed*.017,t)*amp;
  if(dist<.0015){hit=1.;break;}
  travel+=max(.003,dist*.38);
  if(travel>18.)break;
 }
 if(hit<.5)return finishLight(vec3(.0015,.002,.003));
 vec2 q=pos.xz*freq+seed*.017;
 float e=.003,h=surface(q,t);
 vec3 n=normalize(vec3(-(surface(q+vec2(e,0),t)-h)/e*amp*freq,1.,-(surface(q+vec2(0,e),t)-h)/e*amp*freq));
 vec3 reflected=reflect(rd,n);
 float facing=max(dot(-rd,n),0.);
 float fres=.045+.955*pow(1.-facing,5.);
 // Slight optical dispersion along reflection bands, not a screen-wide color shift.
 float separation=mix(.006,.02,detail);
 vec3 col;
 col.r=environment(normalize(reflected+vec3(0,separation,0)),t).r;
 col.g=environment(reflected,t).g;
 col.b=environment(normalize(reflected-vec3(0,separation,0)),t).b;
 col*=.19+.81*fres;
 // The dark glass transmits a faint blue-green ambient term through its body.
 col+=vec3(.00012,.00022,.00024)*facing;
 col*=exp(-travel*.012);
 return finishLight(col*2.8);
}
