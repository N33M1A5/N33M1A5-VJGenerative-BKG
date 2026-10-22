vec3 scene(){
 vec2 p=stage();float t=phase();vec2 flow=vec2(cloud(p*2.,t),cloud(p*2.+9.1,t));
 float granules=cloud(p*7.+flow*2.,t);float filaments=cloud(p*14.+flow*4.,t);
 float hot=pow(smoothstep(.22,.80,granules),2.);float dark=pow(1.-smoothstep(.18,.6,filaments),3.);
 vec3 c=mix(vec3(.12,.016,.001),vec3(.90,.35,.025),hot);
 c+=vec3(.65,.36,.08)*pow(filaments,5.);c*=1.-dark*.55;return finishLight(c);
}
