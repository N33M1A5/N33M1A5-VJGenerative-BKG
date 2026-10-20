#pragma once
// N33M1A5 V1.6: five original stage-ready procedural GLSL motors.
// Design references: stage wireframe architecture, liquid reflective gradients,
// lightning energy, Art Deco symmetry, and volumetric stage rays.
// No paid assets, copied motion loops or third-party shaders.
static const char* kPremiumMotorFunctions=R"GLSL(
float premiumLine(float d,float w){
    return 1.-smoothstep(w,w+max(fwidth(d)*1.45,.0025),abs(d));
}
float premiumRectangle(vec2 p,vec2 size){
    vec2 d=abs(p)-size;
    return length(max(d,0.))+min(max(d.x,d.y),0.);
}
float premiumHash(float x){return fract(sin(x*132.431+4.11)*43758.5453);}

// 18 — ARQUITETURA LED
// Wide architectural wireframe stage with realistic successive portals.
// High-contrast symmetric structures leave an intentionally dark centre for a performer.
vec3 premiumCathedral(vec2 p,float t){
    vec3 col=vec3(0.);
    float breath=.5+.5*cos(uBeat*TAU);
    for(int i=0;i<11;i++){
        float f=float(i);
        float z=fract(f/11.+t*(.029+.025*uDepth));
        float size=.17+3.3*z;
        vec2 v=p/(size+.0005);
        v.x+=.07*sin(f*.8+t*.08)*(1.-z);
        // Structural pillars and ceiling beams.
        float frame=premiumLine(premiumRectangle(v,vec2(.88,.61)),.013+.012*uDetail);
        float arch=premiumLine(length(vec2(v.x,max(0.,v.y)))-.49,.013)
                    *smoothstep(.20,.30,v.y)*(1.-smoothstep(.72,.92,abs(v.x)));
        float column=premiumLine(abs(v.x)-.68,.013)*smoothstep(-.59,-.38,v.y)*(1.-smoothstep(.54,.65,v.y));
        float floorEdge=premiumLine(v.y+.60,.014)*smoothstep(.55,.87,abs(v.x));
        float halo=exp(-abs(premiumRectangle(v,vec2(.88,.61)))*13.)*.08;
        float attenuation=(.30+.70*z)*(1.-z*.32);
        vec3 tint=pal(f*.13+t*.014);
        col+=tint*(frame*.50+arch*.33+column*.35+floorEdge*.30+halo*.65)*attenuation;
    }
    // Moving luminous rails reinforce the sense of physical stage depth.
    float railY=p.y+.71;
    float rail=exp(-abs(railY-p.x*.23)*19.)+exp(-abs(railY+p.x*.23)*19.);
    col+=pal(t*.02+.5)*rail*.09;
    col+=pal(.28+t*.015)*exp(-length(p*vec2(.55,1.))*1.4)*(.025+.042*breath);
    return col;
}

// 19 — CHROME FLOW
// Five broad, flowing chrome ribbons with smooth glossy specular bands.
// Large smooth regions avoid the tiny noisy pixels of the rejected previous prototype.
vec3 premiumChrome(vec2 p,float t){
    vec2 uv=p;
    float time=t*(.13+.08*uDepth);
    vec3 result=vec3(.006,.009,.015);
    float noise=fbm(p*.43+vec2(time*.13,-time*.10));
    for(int i=0;i<6;i++){
        float k=float(i);
        float center=(k-2.5)*(.29+.075*uDensity)
               +sin(uv.x*(1.05+.035*k)+time*.76+k*.95)*(.20+.11*uDepth)
               +sin(uv.x*2.7-time*.32+k*.9)*.055
               +(noise-.5)*.115;
        float distance=uv.y-center;
        float width=.19+.065*sin(time*.27+k);
        float core=exp(-pow(distance/(max(width,.05)),2.));
        float rim=exp(-pow(abs(distance)/(width*.84),2.))*pow(.5+.5*cos(distance*23.+sin(uv.x*1.5+time+k)*2.4),12.);
        float spec=exp(-pow((distance-width*.28)/(.022+.013*uDetail),2.));
        float spec2=exp(-pow((distance+width*.35)/(.04+.015*uDetail),2.));
        float shade=.12+.48*pow(.5+.5*cos(uv.x*.75+time*.28+k*.73),2.);
        vec3 metal=vec3(.052,.063,.075)+pal(k*.14+uv.x*.08+time*.022)*(.50+.24*shade);
        result+=metal*core*(.14+.54*shade)+vec3(.75,.85,.98)*(spec*.34+spec2*.12)*core;
        result+=pal(k*.16+time*.013)*rim*.10;
    }
    // Long spatial reflections move on top of the ribbons, not pixel noise.
    float reflected=pow(.5+.5*cos(uv.x*1.1+sin(uv.y*1.8+time)*.65-time*.3),15.);
    result+=pal(time*.02+.15)*reflected*.055;
    return result;
}

// 20 — ELECTRIC STORM
// Multiple full-height branching bolts with solid readable cores, wide halos and short flashes.
vec3 premiumRift(vec2 p,float t){
    vec3 result=vec3(0.);
    float beat=.5+.5*cos(uBeat*TAU);
    float flash=pow(beat,6.);
    for(int i=0;i<6;i++){
        float f=float(i);
        float x0=(f-2.5)*(.46+.055*uDensity);
        float y=p.y;
        float n=fbm(vec2(y*(2.2+.10*f)+f*7.1,t*.32+f*2.1+uSeed*.06));
        float n2=fbm(vec2(y*(10.5+.4*uDetail)+f*5.5,-t*.83+f*.81));
        float path=x0+(n-.5)*(.36+.22*uDepth)+(n2-.5)*(.105+.06*uDetail);
        float distance=abs(p.x-path);
        float core=exp(-distance*(73.+30.*uDetail));
        float bloom=exp(-distance/(.10+.065*uDensity));
        float flicker=.60+.40*sin(t*.91+f*4.2+y*13.)*sin(t*.46+f*.8);
        vec3 hue=pal(f*.14+t*.016+y*.10);
        result+=hue*(core*(.62+.48*flash)*flicker+bloom*.090);
        result+=vec3(.80,.91,1.)*core*.19;
        float branchPosition=path+.18*sin(y*(4.5+f*.3)+t*.5+f*1.2);
        float branchMask=pow(.5+.5*cos(y*8.4+f*3.-t*.55),4.);
        float twig=exp(-abs(p.x-branchPosition)*(33.+16.*uDetail));
        result+=hue*twig*branchMask*.18;
    }
    // Wide white flash is a strobe-like envelope, not a full-frame blown-out flash.
    float sweep=exp(-length(p*vec2(.52,.75))*1.7);
    result+=pal(t*.018+.4)*sweep*(.013+.13*flash);
    return result;
}

// 21 — GOLDEN FAN
// Art Deco radial fans, petals and nested scalloped arches on black.
// Elegant choice for awards, wedding and worship presentations.
vec3 premiumCubes(vec2 p,float t){
    vec3 result=vec3(.002,.002,.003);
    float move=t*(.055+.035*uDepth);
    float petals=12.+floor(uDetail*16.);
    for(int i=0;i<3;i++){
        float f=float(i);
        vec2 center=vec2((f-1.)*1.6,-.97+.06*sin(move+f));
        vec2 q=p-center;
        float r=length(q);
        float angle=atan(q.y,q.x);
        float fan=pow(abs(cos(angle*petals*.5+move*.65+f*.5)),19.);
        float sector=abs(sin(angle*petals*.5+move*.38+f));
        float inner=exp(-abs(r-(.35+.055*sin(move+f)))*27.);
        float middle=exp(-abs(r-(.58+.035*sin(move*.7+f)))*26.);
        float outer=exp(-abs(r-(.90+.025*sin(move*.6+f)))*21.);
        float rim=inner*.40+middle*.52+outer*.37;
        float spokes=fan*(.5+.5*sin(r*9.-move*.8))*(1.-smoothstep(.24,1.4,r));
        float facets=pow(1.-sector,12.)*middle;
        vec3 ink=pal(f*.22+angle*.08+move*.04);
        result+=ink*(rim*.56+spokes*.21+facets*.24);
    }
    // Crown centrepiece — layered semicircular motif above the performers.
    float r=length(p*vec2(.83,1.)-vec2(0.,-.27));
    float rings=exp(-abs(r-.72)*36.)*.27+exp(-abs(r-.52)*37.)*.19;
    float spokes=pow(abs(cos(atan(p.y+.27,p.x)*13.+move)),24.)*exp(-abs(r-.67)*6.);
    result+=pal(.16+move*.04)*(rings+spokes*.16);
    return result;
}

// 22 — STAGE RAYS
// Stage fixture arrays, strong wide beams and atmospheric curtain glows.
// A dark central safe area makes the visual usable behind artists and event logos.
vec3 premiumNova(vec2 p,float t){
    vec3 result=vec3(0.);
    float move=t*(.10+.12*uDepth);
    float beat=.5+.5*cos(uBeat*TAU);
    for(int i=0;i<14;i++){
        float fi=float(i),side=mod(fi,2.)<1.?1.:-1.;
        float bank=floor(fi*.5);
        vec2 lamp=vec2(side*(.37+bank*.26),1.13);
        float angle=-1.570796+side*(.15+.43*sin(move*.6+bank*.35+uSeed*.01));
        vec2 direction=vec2(cos(angle),sin(angle));
        vec2 ray=p-lamp;
        float depth=dot(ray,direction);
        float perp=abs(ray.x*direction.y-ray.y*direction.x);
        float coneWidth=.027+.045*uDensity+.10*max(0.,depth);
        float beam=exp(-pow(perp/max(coneWidth,.006),1.4))
                  *smoothstep(-.04,.15,depth)*exp(-max(depth,0.)*.43);
        float edge=exp(-perp*(30.+25.*uDetail));
        float k=bank*.115+side*.07+t*.014;
        result+=pal(k)*beam*(.065+.065*beat);
        result+=pal(k+.08)*edge*beam*.10;
    }
    for(int j=0;j<7;j++){
        float k=float(j);
        float x=(k-3.)*.52;
        float wobble=.12*sin(p.y*(1.+k*.16)+move*.55+k*1.8);
        float curtain=exp(-abs(p.x-x-wobble)*(4.+4.*uDetail));
        result+=pal(k*.14+t*.014)*curtain*(.030+.043*uDensity);
    }
    float fog=fbm(p*.71+vec2(move*.12,-move*.06));
    result+=pal(fog*.5+t*.017)*fog*.037;
    return result;
}
)GLSL";