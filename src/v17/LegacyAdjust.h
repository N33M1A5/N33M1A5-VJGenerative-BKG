#pragma once
// Add optional adjustments to a runtime copy. The archived original GLSL stays untouched.
static bool addSharedAdjustments(std::string& s,bool classic){
 const char* rgbHue=R"GLSL(
 vec3 userRGBAHue(vec3 c,float h){vec3 k=normalize(vec3(1));float a=h*6.28318530718;return max(vec3(0),c*cos(a)+cross(k,c)*sin(a)+k*dot(k,c)*(1.-cos(a)));}
)GLSL";
 // Some assembled classic shaders contain more than one pal() declaration.
 // Insert the prototype before the first declaration without requiring uniqueness.
 const std::string palDecl="vec3 pal(float t){";
 const auto palPos=s.find(palDecl);
 if(palPos==std::string::npos)return false;
 s.insert(palPos,"vec3 userRGBAHue(vec3 c,float h);\n");
 if(classic){if(!replaceOnce(s,"if(mode==1)return A;","if(mode==1)return userRGBAHue(A,uHue);"))return false;}
 else{if(!replaceOnce(s,"if(mode==1)return A.rgb;","if(mode==1)return userRGBAHue(A.rgb,uHue);"))return false;}
 if(!replaceOnce(s,"void main(){",std::string(rgbHue)+"\nvoid main(){"))return false;
 if(!replaceOnce(s,"void main(){","uniform float uOrientation,uGlobalDetail,uGlobalDepth,uGlobalDensity;\nvoid main(){"))return false;
 std::string p="vec2 p=(uv-.5)*vec2(ar,1.)*2.;\np=mat2(cos(uOrientation),-sin(uOrientation),sin(uOrientation),cos(uOrientation))*p;";
 if(!classic)p+="\np+=vec2(.35*sin(uSeed),.14*(cos(uSeed)-1.));";
 if(classic)p+="\np.y/=max(.25,1.+(uGlobalDepth-.5)*p.y*.22);";
 if(!replaceOnce(s,"vec2 p=(uv-.5)*vec2(ar,1.)*2.;",p))return false;
 if(classic){
  const char* post=R"GLSL(
  col+=fwidth(col)*(uGlobalDetail-.5)*1.5;
  float light=max(max(col.r,col.g),col.b);
  if(uGlobalDensity<.5)col*=mix(1.,smoothstep(.015,.65,light),1.-uGlobalDensity*2.);
  else col+=col*(uGlobalDensity-.5)*.8*(1.-smoothstep(.1,.9,light));
  fragColor=vec4(clamp(col,0.,1.),alpha);
)GLSL";
  if(!replaceOnce(s,"fragColor=vec4(clamp(col,0.,1.),alpha);",post))return false;
 }
 return true;
}
