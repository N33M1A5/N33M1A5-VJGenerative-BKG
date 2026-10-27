#version 410 core
in vec2 uv;
out vec4 fragColor;
uniform sampler2D source,bloom;
uniform float glow,opacity,contrast,detailAmount,coverage;
void main(){
 vec3 base=texture(source,uv).rgb,soft=texture(bloom,uv).rgb;
 if(detailAmount<.5)base=mix(soft,base,detailAmount*2.);
 else base+=max(base-soft,vec3(0))*((detailAmount-.5)*2.);
 vec3 c=base+soft*(.12+glow*.65);
 float light=max(max(c.r,c.g),c.b);
 if(coverage<.5)c*=mix(1.,smoothstep(.015,.38,light),1.-coverage*2.);
 else c+=soft*(coverage-.5)*.8*(1.-smoothstep(.1,.7,light));
 c=1.-exp(-max(c,0.));c=pow(c,vec3(1./2.2));c=(c-.5)*exp2((contrast-.5)*2.)+.5;
 float d=fract(52.9829189*fract(dot(gl_FragCoord.xy,vec2(.06711056,.00583715))))-.5;
 fragColor=vec4(clamp(c+d/255.,0.,1.),opacity);
}
