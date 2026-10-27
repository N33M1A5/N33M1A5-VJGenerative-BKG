#version 410 core
in vec2 uv;
out vec4 fragColor;
uniform sampler2D source, bloom;
uniform float glow,opacity;
void main(){
 vec3 c=texture(source,uv).rgb+texture(bloom,uv).rgb*(.12+glow*.65);
 c=1.-exp(-c);c=pow(max(c,0.),vec3(1./2.2));
 float d=fract(52.9829189*fract(dot(gl_FragCoord.xy,vec2(.06711056,.00583715))))-.5;
 fragColor=vec4(clamp(c+d/255.,0.,1.),opacity);
}
