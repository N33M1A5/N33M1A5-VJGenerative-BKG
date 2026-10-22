#ifdef NDEBUG
#undef NDEBUG
#endif
#include "src/v17/TextAtlas.h"
#include <cassert>
#include <iostream>
int main(){
 TextAtlas a,b;assert(a.build(u8"ACAO E GRACA\nJESUS"));assert(b.build(u8"AÇÃO E GRAÇA\nJESUS"));
 assert(a.pixels.size()==1024*512&&b.pixels.size()==1024*512);
 int ink=0,different=0;for(size_t i=0;i<a.pixels.size();i++){ink+=b.pixels[i]>0;different+=a.pixels[i]!=b.pixels[i];}
 assert(ink>1000&&different>100);assert(b.build(""));for(auto x:b.pixels)assert(x==0);
 std::cout<<"PASS: Windows GDI text rasterized; UTF-8 accents alter glyph pixels; empty text supported. Ink="<<ink<<" differences="<<different<<"\n";
}
