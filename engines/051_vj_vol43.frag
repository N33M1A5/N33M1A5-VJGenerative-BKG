vec3 scene(){
 vec2 p=stage();float t=phase();vec2 q=p;
 for(int i=0;i<5;i++){float f=float(i);q+=.16*vec2(sin(q.y*(2.+f*.3)+sin(t+f)),cos(q.x*(2.3+f*.2)+cos(t-f)));}
 float fold=sin(q.x*7.+q.y*3.+.6*sin(t));float fres=pow(.5+.5*fold,9.);
 vec3 c=grad3(.5+.5*sin(q.x*2.-q.y*1.8),vec3(.13,.001,.033),vec3(.56,.012,.15),vec3(.17,.045,.28));
 c*=.28+.72*fres;c+=vec3(.9,.60,.32)*pow(fres,3.)*.6;c+=vec3(.6,.35,.75)*exp(-pow((fold-.65)/.1,2.))*.25;return finishLight(c);
}
