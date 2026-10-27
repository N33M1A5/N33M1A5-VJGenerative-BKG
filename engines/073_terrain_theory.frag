float terrainHeight(vec2 p,float t){
 float a=n3(vec3(p*.7+seed*.13,1.));float b=n3(vec3(p*1.5+2.,2.));float c=n3(vec3(p*3.7,3.));
 float ridge=1.-abs(2.*a-1.);return .12+1.10*pow(ridge,3.)+.22*b+.10*c;
}
vec3 scene(){
 vec2 p=stage();float t=phase();vec3 ro=vec3(.2*sin(t),1.1,-3.0);vec3 rd=normalize(vec3(p.x*.55,p.y*.40-.27,1.));
 float travel=.2,hit=0.;vec3 pos=ro;
 for(int i=0;i<55;i++){pos=ro+rd*travel;float d=pos.y-terrainHeight(pos.xz,t);if(d<.004){hit=1.;break;}travel+=max(.015,d*.4);if(travel>18.)break;}
 vec3 fog=vec3(.012,.035,.045);vec3 c=fog*(.5+.5*exp(-abs(p.y)*2.));
 if(hit>.5){float e=.02,h=terrainHeight(pos.xz,t);vec3 n=normalize(vec3(-(terrainHeight(pos.xz+vec2(e,0),t)-h)/e,1.,-(terrainHeight(pos.xz+vec2(0,e),t)-h)/e));
  float lit=max(dot(n,normalize(vec3(-.5,.8,-.3))),0.);vec3 rock=vec3(.008,.017,.026)*(.2+lit*.8);
  float crack=pow(.5+.5*sin(pos.x*9.+pos.z*7.+cloud(pos.xz*2.,t)*6.),30.);rock+=vec3(.04,.015,.15)*crack*.16;
  float haze=1.-exp(-travel*.075);c=mix(rock,fog,haze);
 }
 c+=vec3(.015,.04,.05)*spot(p,vec2(.5*cos(t),.45),vec2(.8,2.));return finishLight(c);
}
