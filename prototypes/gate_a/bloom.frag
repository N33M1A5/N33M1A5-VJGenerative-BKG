#version 410 core
in vec2 uv;
out vec4 fragColor;
uniform sampler2D source;
uniform vec2 axis;
void main(){
 vec4 c=texture(source,uv)*.227027;
 c+=texture(source,uv+axis*1.384615)*.316216;
 c+=texture(source,uv-axis*1.384615)*.316216;
 c+=texture(source,uv+axis*3.230769)*.070270;
 c+=texture(source,uv-axis*3.230769)*.070270;
 fragColor=c;
}
