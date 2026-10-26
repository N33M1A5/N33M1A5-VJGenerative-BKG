#include "src/v17/VJGenerativeV17.cpp"
#include <cassert>
#include <cstring>
#include <iostream>
static float floatValue(FFMixed x){float value;std::memcpy(&value,&x.UIntValue,sizeof(value));return value;}
int main(){
 VJGenerative plugin;assert(plugin.GetNumParams()==P_COUNT);
 assert(plugin.GetParamType(P_ENGINE)==FF_TYPE_OPTION);
 auto n=plugin.GetNumParamElements(P_ENGINE);assert(n==kTotalEngines);
 for(unsigned j=0;j<n;j++){
  int engine=(int)std::lround(floatValue(plugin.GetParamElementDefault(P_ENGINE,j)));
  assert(engine==kMotorUIOrder[j]);
  assert(plugin.SetFloatParameter(P_ENGINE,(float)engine)==FF_SUCCESS);
  assert(std::lround(plugin.GetFloatParameter(P_ENGINE))==engine);
  assert(std::lround(plugin.GetFloatParameter(P_ACTIVE_ENGINE)*81)==engine);
 }
 assert(plugin.GetParamType(P_TEXT)==FF_TYPE_TEXT);
 const char* text=u8"AÇÃO E GRAÇA\nCRIAÇÃO — JESUS";assert(plugin.SetTextParameter(P_TEXT,text)==FF_SUCCESS);assert(std::strcmp(plugin.GetTextParameter(P_TEXT),text)==0);
 assert(plugin.SetTextParameter(P_TEXT,"")==FF_SUCCESS);
 for(int palette=0;palette<36;palette++){assert(plugin.SetFloatParameter(P_PALETTE,(float)palette)==FF_SUCCESS);assert(std::lround(plugin.GetFloatParameter(P_PALETTE))==palette);}
 std::cout<<"PASS: one native selector with 82 engines; shared selection persistence; 36 palettes; FF_TYPE_TEXT UTF-8 roundtrip\n";
}
