// Spotlight 1 is the fluted Glass collection, not a stage beam.
vec3 lightField(vec2 p,float t){
 vec2 a=p-vec2(-.60+1.15*sin(t),.55+.65*cos(t));
 vec2 b=p-vec2(.65+1.10*cos(t+.3),-.15+.80*sin(t));
 vec2 c=p-vec2(-.40+1.40*sin(t+2.),-.35+.75*cos(t));
 vec3 col=vec3(.0004,.001,.002);
 col+=vec3(1.35,.08,.035)*exp(-dot(a*vec2(1.3,1.9),a*vec2(1.3,1.9)));
 col+=vec3(1.10,.80,.44)*exp(-dot(b*vec2(1.9,1.1),b*vec2(1.9,1.1)));
 col+=vec3(.04,.43,.55)*exp(-dot(c,c)*1.7);
 return col;
}
vec3 scene(){
 float t=phase();vec2 p=canvas();
 p.x=mix(p.x,abs(p.x),symmetry);
 float count=mix(8.,24.,density);
 float x=p.x*count+seed*.13;
 float f=fract(x)-.5;
 float aa=max(fwidth(x),.008);
 // Rounded flute normal, soft refraction and a dark edge between rods.
 float nx=sin(f*3.14159),nz=cos(f*3.14159);
 vec2 refr=p+vec2(nx*mix(.018,.10,depth),.035*sin(t+p.x));
 vec3 col=lightField(refr,t)*(.012+.18*pow(max(nz,0.),6.));
 float edge=exp(-pow((f+.27)/mix(.065,.022,detail),2.));
 col+=lightField(refr+vec2(.04,.02),t)*edge*1.55;
 float fine=exp(-pow((f-.34)/.025,2.));
 col+=lightField(refr+vec2(-.12,.04),t)*fine*.35;
 float gap=1.-smoothstep(.46-aa,.5,abs(f));
 col*=gap;
 col*=.30+.70*exp(-dot(p,p)*.24);
 col*=.18+.82*smoothstep(.02,.7,max(max(col.r,col.g),col.b));
 return finishLight(col);
}
