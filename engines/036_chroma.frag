vec3 scene(){
 vec2 p=stage();float t=phase();vec2 q=p+vec2(.22*sin(p.y*2.+t),.18*cos(p.x*2.-t));
 float rows=floor((q.y+1.)*7.);float slit=1.-smoothstep(.20,.44,abs(fract((q.y+1.)*7.)-.5));
 vec3 c=spectral(q.x*.23+rows*.08+.12*sin(t));
 float stripe=.3+.7*smoothstep(-.5,.8,sin(q.x*5.+t+rows));
 c*=slit*stripe*.58;c+=spectral(q.y*.2-.04*sin(t))*.045;return finishLight(c);
}
