#pragma once
#if defined(_WIN32) && !defined(NOMINMAX)
#define NOMINMAX
#endif
#include <string>
#include <vector>
#include <algorithm>
#ifdef _WIN32
#include <windows.h>
// FFGL first loads windows.h with NOUSER/NONLS, then undefines them.
// Its include guard prevents a later windows.h from restoring these APIs.
#include <winnls.h>
#include <winuser.h>
#endif
// Rasterize with the operating system font. No third-party font files are shipped.
struct TextAtlas {
 static constexpr int W=1024,H=512;
 std::vector<unsigned char> pixels;
 bool build(const std::string& utf8){
  pixels.assign(W*H,0);if(utf8.empty())return true;
#ifdef _WIN32
  int count=MultiByteToWideChar(CP_UTF8,MB_ERR_INVALID_CHARS,utf8.data(),(int)utf8.size(),nullptr,0);
  if(count<=0)return false;
  std::wstring text(count,L' ');MultiByteToWideChar(CP_UTF8,MB_ERR_INVALID_CHARS,utf8.data(),(int)utf8.size(),&text[0],count);
  HDC dc=CreateCompatibleDC(nullptr);if(!dc)return false;
  BITMAPINFO info{};info.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);info.bmiHeader.biWidth=W;info.bmiHeader.biHeight=-H;info.bmiHeader.biPlanes=1;info.bmiHeader.biBitCount=32;info.bmiHeader.biCompression=BI_RGB;
  void* data=nullptr;HBITMAP bitmap=CreateDIBSection(dc,&info,DIB_RGB_COLORS,&data,nullptr,0);
  if(!bitmap){DeleteDC(dc);return false;}
  auto oldBitmap=SelectObject(dc,bitmap);std::fill_n((unsigned char*)data,W*H*4,0);
  int lines=1+(int)std::count(utf8.begin(),utf8.end(),'\n');int size=std::min(100,400/std::max(1,lines));
  HFONT font=CreateFontW(-size,0,0,0,FW_BOLD,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY,DEFAULT_PITCH|FF_SWISS,L"Segoe UI");
  if(!font){SelectObject(dc,oldBitmap);DeleteObject(bitmap);DeleteDC(dc);return false;}
  auto oldFont=SelectObject(dc,font);SetBkMode(dc,TRANSPARENT);SetTextColor(dc,RGB(255,255,255));
  RECT rect{24,24,W-24,H-24};DrawTextW(dc,text.c_str(),count,&rect,DT_CENTER|DT_WORDBREAK|DT_NOPREFIX);
  GdiFlush();const auto* rgba=(const unsigned char*)data;
  for(int y=0;y<H;y++)for(int x=0;x<W;x++)pixels[(H-1-y)*W+x]=rgba[(y*W+x)*4];
  SelectObject(dc,oldFont);DeleteObject(font);SelectObject(dc,oldBitmap);DeleteObject(bitmap);DeleteDC(dc);
  return true;
#else
  // The EGL test host supplies a Pillow-rasterized UTF-8 atlas through the test bridge.
  return false;
#endif
 }
};
