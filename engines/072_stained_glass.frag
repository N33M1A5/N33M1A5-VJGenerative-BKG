vec3 scene(){
 vec2 p=stage();float t=phase();vec2 q=rot(.12)*p;
 // Close view of a leaded window: gently distorted rectangular glass, no radial symmetry.
 q.x+=.035*sin(q.y*5.);vec2 grid=q*vec2(5.,4.);vec2 id=floor(grid),f=fract(grid)-.5;
 float lead=smoothstep(.0,.045,.5-abs(f.x))*smoothstep(.0,.045,.5-abs(f.y));
 float bevel=exp(-pow((abs(f.x)-.44)/.035,2.))+exp(-pow((abs(f.y)-.44)/.035,2.));
 float key=hash21(id+seed);vec3 tint=mix(vec3(.008,.08,.04),vec3(.02,.10,.26),key);tint=mix(tint,vec3(.18,.012,.035),pow(key,5.));
 float light=spot(p,vec2(.8*sin(t),.6*cos(t)),vec2(.75,1.));float mottled=.4+.6*cloud(q*8.,t);
 vec3 c=tint*lead*(.15+light*1.8)*mottled;c+=vec3(.07,.14,.15)*bevel*lead*light*.3;
 c+=vec3(.003,.004,.005)*(1.-lead);return finishLight(c);
}
