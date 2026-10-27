vec3 scene(){
 vec2 p=stage();float t=phase();vec2 grid=floor((p+2.)*vec2(290.,180.));
 float flicker=sin(t*61.+grid.y*.77);float tear=sin(grid.y*.18+sin(t)*8.);
 float h=hash21(grid+seed);float noise=.5+.5*sin(h*90.+t*67.);
 float signal=smoothstep(.52,.95,noise)*smoothstep(-.7,.7,tear)*exp(-abs(p.x+.6+.35*sin(t))*1.7);
 vec3 c=vec3(signal)*.15;vec3 rgb=vec3(sin(h*20.+t*23.),sin(h*41.-t*31.),sin(h*70.+t*17.))*.5+.5;
 c+=rgb*signal*.5;c*=.4+.6*step(0.,flicker);
 c+=vec3(.008,.01,.013)*(.5+.5*cos(p.y*800.));return finishLight(c);
}
