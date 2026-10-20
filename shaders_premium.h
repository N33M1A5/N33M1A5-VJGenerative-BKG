#pragma once
// N33M1A5 ORIGINAL premium stage visuals — shader source, not stock/VJ video assets.
// Designed to use the exact existing palette/beat/scale/intensity/alpha controls.
// Function IDs in the extension: 18 Catedral, 19 Chrome, 20 Energy, 21 Infinity, 22 Nova.
static const char* kPremiumMotorFunctions=R"GLSL(
// 18 — CATEDRAL DE LUZ: layered luminous arch portal, converging depth and atmosphere.
float sdArch(vec2 p){
    p.y+=.18;
    float side=abs(abs(p.x)-.62);
    float legs=max(side, max(-1.0-p.y,p.y-.12));
    float arc=abs(length(vec2(p.x,max(0.,p.y-.12)))-.62);
    arc=max(arc,.12-p.y);
    return min(legs,arc);
}
vec3 premiumCathedral(vec2 p,float t){
    vec3 col=vec3(0.);
    vec2 r=p;float beat=.5+.5*cos(uBeat*TAU);
    r.x+=.05*sin(t*.16+r.y*1.9);
    for(int i=0;i<10;i++){
        float n=float(i);
        float phase=fract(n/10.+t*(.032+.03*uDepth));
        float perspective=.16+phase*2.5;
        vec2 q=r/max(perspective,.01);
        q.y+=.12;
        float d=sdArch(q);
        float thin=aaLine(d,.012+.014*uDetail)/max(.4,perspective);
        float halo=exp(-abs(d)*(15.+10.*uDensity))*(.05+.07*beat);
        float tint=n*.13+phase*.3+t*.012;
        col+=pal(tint)*(thin*.65+halo*.18)*(1.-phase*.35);
        // Thin central light arrays reinforce symmetric stage proportions.
        float glass=exp(-abs(q.x)*18.)*exp(-abs(q.y)*2.)*.045;
        col+=pal(tint+.18)*glass;
    }
    float atmosphere=exp(-length(r)*.65)*(.035+.06*beat);
    col+=pal(t*.025+.3)*atmosphere;
    return col;
}
// 19 — CHROME TIDE: domain warped metallic liquid with specular highlights and prismatic reflections.
vec3 premiumChrome(vec2 p,float t){
    vec2 q=p*(1.05+uDensity*.95);
    vec2 flow=vec2(fbm(q*.82+vec2(t*.07,.15)),fbm(q*.9+vec2(-.21,t*.065)));
    vec2 w=q+flow*(1.5+1.4*uDepth);
    float h=fbm(w*1.8+vec2(t*.15,-t*.09));
    vec2 e=vec2(.025,0.);
    float hx=fbm((w+e.xy)*1.8+vec2(t*.15,-t*.09))-h;
    float hy=fbm((w+e.yx)*1.8+vec2(t*.15,-t*.09))-h;
    vec3 normal=normalize(vec3(hx*19.,hy*19.,.6));
    vec3 key=normalize(vec3(-.55,.7,.8));
    vec3 fill=normalize(vec3(.6,-.45,.85));
    float keySpec=pow(max(0.,dot(reflect(-key,normal),vec3(0.,0.,1.))),18.+32.*uDetail);
    float fillSpec=pow(max(0.,dot(reflect(-fill,normal),vec3(0.,0.,1.))),29.);
    float stripes=pow(.5+.5*cos(h*18.+flow.x*11.+q.x*1.8-t*.7),10.);
    float edge=pow(1.-clamp(normal.z,0.,1.),2.);
    vec3 metal=vec3(.018)+vec3(.13,.16,.20)*(.3+.5*normal.z);
    vec3 spectral=pal(h*.64+flow.x*.22+t*.025);
    metal+=spectral*(keySpec*1.45+fillSpec*.6+stripes*.3+edge*.16);
    float softBox=pow(max(0.,.5+.5*cos(w.y*8.7-w.x*3.1+t*.17)),22.);
    float flowingStripe=pow(max(0.,.5+.5*cos(w.y*14.+flow.x*8.5-t*.31)),18.);
    metal+=vec3(.65,.8,.98)*(keySpec*.45+softBox*.16);
    metal+=pal(flow.y*.4+t*.017)*flowingStripe*.23;
    return metal;
}
// 20 — ENERGY RIFT: turbulent rift with asymmetric lightning, branching and sparks.
vec3 premiumRift(vec2 p,float t){
    vec2 q=p;
    float w=fbm(vec2(q.y*(3.+uDensity*3.),t*.18+uSeed*.013));
    float w2=fbm(vec2(q.y*9.-t*.44,3.21+uSeed*.03));
    float center=(w-.5)*(.36+.24*uDepth)+sin(q.y*6.+t*.35)*.08+(w2-.5)*.10;
    float x=q.x-center;
    float fracture=.012+.022*(.5+.5*sin(q.y*21.+t*.7));
    float core=exp(-abs(x)/(fracture+.006));
    float halo=exp(-abs(x)/(.11+.16*uDensity));
    float band=pow(.5+.5*sin(q.y*(14.+13.*uDetail)-t*.8+w2*4.),13.);
    float twitch=pow(.5+.5*cos(q.y*31.+t*1.5),10.);
    vec3 col=pal(q.y*.18+t*.036)*(core*.75+halo*.12+band*halo*.26);
    col+=vec3(.85,.9,1.)*core*(.15+.25*twitch);
    // Disconnected electrical streamers create asymmetry instead of a plain centered line.
    for(int i=0;i<5;i++){
        float fi=float(i);
        float branch=center+.17*sin(q.y*(3.+fi)+t*(.33+fi*.05)+fi*2.5);
        float dist=abs(q.x-branch);
        float envelope=pow(.5+.5*sin(q.y*6.-fi*3.+t*.24),4.);
        col+=pal(fi*.19+t*.03)*exp(-dist*(34.+uDetail*25.))*envelope*.17;
    }
    // Major portal glow reaches far beyond the lightning seam so LED walls read it from a distance.
    float haze=exp(-abs(x)/( .28 + .12*uDensity ))*(.28+.17*pow(.5+.5*cos(uBeat*TAU),4.));
    float slash=pow(.5+.5*sin(q.y*3.9+t*.7+w*6.5),9.)*exp(-abs(x)*2.6);
    float branchGlow=exp(-abs(q.x-(center+.24*sin(q.y*4.1-t*.22)))*11.)*
                     pow(.5+.5*cos(q.y*9.7+t*.27),6.)*.40;
    float after=exp(-length(q)*1.3)*(.045+.14*pow(.5+.5*cos(uBeat*TAU),4.));
    return col+pal(t*.04)*after + pal(q.y*.12+t*.05)*(haze*.55+slash*.24+branchGlow*.36);
}
// 21 — INFINITY CUBES: rotating perspective 3D wireframes with convincing depth parallax.
float sdSegment(vec2 p,vec2 a,vec2 b){
    vec2 v=b-a;
    return length(p-(a+v*clamp(dot(p-a,v)/max(dot(v,v),.00001),0.,1.)));
}
mat2 rotate2(float a){float s=sin(a),c=cos(a);return mat2(c,s,-s,c);}
float squareFrame(vec2 q,float r){
    vec2 e=abs(q);
    return max(e.x,e.y)-r;
}
vec3 premiumCubes(vec2 p,float t){
    vec3 col=vec3(0.);
    float beat=.5+.5*cos(uBeat*TAU);
    for(int i=0;i<9;i++){
        float fi=float(i);
        float depth=fract(fi/9.+t*(.025+.03*uDepth));
        float perspective=.19+2.3*depth;
        float a=t*(.08+.09*uDetail)+fi*.09;
        vec2 q=rotate2(a)*p;
        q.x+=.11*sin(fi*2.+t*.11)*depth;
        float inner=.45*perspective;
        float outer=.68*perspective;
        float edge1=aaLine(abs(squareFrame(q,inner)),.012+.009*uDensity);
        float edge2=aaLine(abs(squareFrame(rotate2(.785398+a*.22)*p,outer)),.010);
        vec2 v=abs(q);
        float diag=aaLine(abs(v.x-v.y),.018)*smoothstep(inner*.7,inner,v.x)
                   *(1.-smoothstep(outer,outer*1.15,v.x));
        float glow=exp(-abs(squareFrame(q,inner))*18.)*.035;
        col+=pal(fi*.13+t*.012)*(edge1*.38+edge2*.28+diag*.13+glow)
             *(1.2-depth*.55);
    }
    col+=pal(t*.032+.3)*exp(-length(p)*2.4)*(.04+.07*beat);
    return col;
}
// 22 — NOVA PULSE: wide-screen shockwave explosions, bright photonic spokes and energy rings.
vec3 premiumNova(vec2 p,float t){
    float r=length(p),a=atan(p.y,p.x),pulse=.5+.5*cos(uBeat*TAU);
    float lobes=16.+floor(uDetail*32.);
    float flare=pow(max(0.,cos(a*lobes*.5+sin(r*4.-t*.34)*.35)),24.);
    float phase=fract(t*(.08+.08*uDepth));
    float radius=.25+phase*(1.1+.4*uDensity);
    float ring=aaLine(abs(r-radius),.025+.012*pulse)*(1.-phase*.38);
    float backRing=aaLine(abs(r-(radius*.58)),.018)*.3;
    float lightShaft=flare*exp(-r*(.65+.5*uDensity));
    float corona=exp(-r*(3.+2.*uDensity));
    float ripples=pow(.5+.5*cos(r*18.-t*1.2+fbm(p*1.4)*2.5),20.);
    vec3 col=pal(a*.16+t*.02)*(lightShaft*.63+ring*.95+backRing*.42);
    float volume=exp(-r*(1.25+.65*uDensity))*(.20+.19*pulse);
    float star=pow(.5+.5*cos(a*(6.+floor(uDetail*8.))),18.)*exp(-r*.88);
    float sparkles=pow(.5+.5*cos(a*44.+r*14.-t*.55),32.)*exp(-abs(r-radius)*4.);
    col+=pal(r*.23+.32+t*.03)*(corona*.95+ripples*.22+volume*.7);
    col+=pal(a*.14+t*.024)*(star*.30+sparkles*.17);
    col+=vec3(.85,.9,1.)*ring*pulse*.43;
    return col;
}
)GLSL";