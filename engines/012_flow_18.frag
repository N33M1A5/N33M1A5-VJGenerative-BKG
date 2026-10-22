vec3 scene(){
 vec2 p=stage();float t=phase();vec2 q=rot(-.32)*p;
 for(int i=0;i<4;i++)q+=vec2(sin(q.y*2.1+float(i)+sin(t)),cos(q.x*1.8-float(i)+cos(t)))*.18;
 float f=q.x+q.y*.35;float bands=sin(f*34.+.7*sin(t)+cloud(q*3.,t)*3.);
 float sharp=pow(.5+.5*bands,12.);float rough=cloud(q*vec2(3.,14.),t);
 vec3 c=vec3(.002,.009,.006)+vec3(.14,.47,.34)*sharp*(.2+rough);
 c+=vec3(.04,.16,.12)*pow(.5+.5*sin(f*9.+rough*2.),6.);
 return finishLight(toning(c,p,t));
}
