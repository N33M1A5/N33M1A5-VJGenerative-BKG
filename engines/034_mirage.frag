vec3 scene(){
 vec2 p=stage();float t=phase();vec2 q=rot(-.65)*p;float streak=cloud(q*vec2(13.,1.3),t);
 float layer=sin(q.x*20.+streak*7.+sin(t));
 vec3 c=grad3(streak,vec3(.012,.021,.07),vec3(.21,.07,.09),vec3(.12,.36,.43));
 c+=vec3(.44,.31,.16)*pow(max(layer,0.),9.)*(.2+.8*streak);
 c+=grain(p,t)*.018;return finishLight(max(c,0.));
}
