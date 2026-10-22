vec3 scene(){
 vec2 p=stage();float t=phase();float horizon=-.83+.10*sin(t);float v=exp(-abs(p.y-horizon)*4.8);
 vec3 c=grad3(clamp((p.y-horizon)*.6,0.,1.),vec3(.52,.32,.007),vec3(.065,.018,.001),vec3(.001,.002,.012));
 c+=vec3(.80,.55,.045)*v*.65;c+=vec3(.08,.05,.02)*spot(p,vec2(.7*cos(t),horizon),vec2(.55,4.));return finishLight(toning(c,p,t));
}
