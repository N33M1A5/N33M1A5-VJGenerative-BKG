#include "src/v17/VJGenerativeV17.cpp"
#include <cassert>
#include <cstring>
#include <iostream>
static float floatValue(FFMixed x){float value;std::memcpy(&value,&x.UIntValue,sizeof(value));return value;}
int main(){
 VJGenerative plugin;assert(plugin.GetNumParams()==P_COUNT);
 int rows=0,total=0;
 for(int row=0;row<kMotorRows;row++){
  unsigned index=row==0?P_ENGINE:(row==1?P_EXTRA_MOTOR:P_MOTOR_ROW_FIRST+row-2);
  assert(plugin.GetParamType(index)==FF_TYPE_OPTION);auto n=plugin.GetNumParamElements(index);assert(n>=1&&n<=6);total+=n;rows++;
  for(unsigned j=0;j<n;j++){
   int engine=(int)std::lround(floatValue(plugin.GetParamElementDefault(index,j)));
   assert(engine==kMotorUIOrder[row*6+j]);assert(plugin.SetFloatParameter(index,(float)engine)==FF_SUCCESS);
   assert(std::lround(plugin.GetFloatParameter(index))==engine);
   float saved=plugin.GetFloatParameter(P_ACTIVE_ENGINE);
   // Host restores all rows. An unrelated row must not override the selection.
   for(int other=0;other<kMotorRows;other++){
    unsigned p=other==0?P_ENGINE:(other==1?P_EXTRA_MOTOR:P_MOTOR_ROW_FIRST+other-2);
    plugin.SetFloatParameter(p,(float)engine);
   }
   assert(std::lround(plugin.GetFloatParameter(P_ACTIVE_ENGINE)*81)==engine);
   plugin.SetFloatParameter(P_ACTIVE_ENGINE,0.f);plugin.SetFloatParameter(P_ACTIVE_ENGINE,saved);
   assert(std::lround(plugin.GetFloatParameter(index))==engine);
  }
 }
 assert(rows==14&&total==82);assert(plugin.GetParamType(P_TEXT)==FF_TYPE_TEXT);
 const char* text=u8"AÇÃO E GRAÇA\nCRIAÇÃO — JESUS";assert(plugin.SetTextParameter(P_TEXT,text)==FF_SUCCESS);assert(std::strcmp(plugin.GetTextParameter(P_TEXT),text)==0);
 assert(plugin.SetTextParameter(P_TEXT,"")==FF_SUCCESS);
 for(int palette=0;palette<36;palette++){assert(plugin.SetFloatParameter(P_PALETTE,(float)palette)==FF_SUCCESS);assert(std::lround(plugin.GetFloatParameter(P_PALETTE))==palette);}
 std::cout<<"PASS: 82 selections in 14 native OPTION rows; shared selection persistence; 36 palettes; FF_TYPE_TEXT UTF-8 roundtrip\n";
}
