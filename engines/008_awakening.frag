vec3 scene(){
 vec2 p=stage();float t=phase();vec2 origin=vec2(.15*sin(t),-.96);vec2 v=p-origin;
 float a=atan(v.x,max(v.y,.02)),r=length(v);vec3 c=vec3(.003,.009,.025);
 for(int i=0;i<34;i++){float fi=float(i);float angle=(hash21(vec2(fi,seed))-.5)*2.4+.07*sin(t+fi);
  float width=mix(.002,.014,hash21(vec2(fi,3.)));float beam=exp(-pow((a-angle)/width,2.))*exp(-r*.33);
  c+=mix(vec3(.04,.20,.55),vec3(1.1,.63,.18),hash21(vec2(fi,7.)))*beam*(.3+.7*depth);
 }
 c+=vec3(1.2,.84,.40)*exp(-length(v)*3.5)*2.;
 c+=vec3(.5,.25,.05)*exp(-abs(p.y+.90)*14.)*.3;
 return finishLight(toning(c,p,t));
}
