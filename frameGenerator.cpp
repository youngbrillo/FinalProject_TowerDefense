#include <sys/stat.h>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include "frameGenerator.h"
#include "renderContext.h"

FrameGenerator::FrameGenerator() : 
  //rend(RenderContext::getInstance()->getRenderer()), 
  rend(RenderContext::getInstance().getRenderer()), 
  //window(RenderContext::getInstance()->getWindow()), 
  window(RenderContext::getInstance().getWindow()), 
  WIDTH( Gamedata::getInstance().getXmlInt("view/width") ),
  HEIGHT( Gamedata::getInstance().getXmlInt("view/height") ),
  USERNAME( Gamedata::getInstance().getXmlStr("username") ),
  MAX_FRAMES( Gamedata::getInstance().getXmlInt("maxFrames") ),
  frameCount(0) 
{
  struct stat info;
  if( stat( "frames", &info ) != 0 ) {
    mkdir("frames", 0755);
  }
}


void FrameGenerator::makeFrame() {
  if ( frameCount > MAX_FRAMES ) return;

  SDL_Surface* screenCap = SDL_GetWindowSurface(window);
  if ( screenCap ) {
    SDL_RenderReadPixels(rend, NULL, 
      SDL_GetWindowPixelFormat(window), 
      screenCap->pixels, screenCap->pitch); 
  }
  std::stringstream strm;
  strm << "frames/" << USERNAME << '.' 
       << std::setfill('0') << std::setw(4) 
       << frameCount++ << ".bmp";
  std::string filename( strm.str() );
  std::cout << "Making frame: " << filename << std::endl;
  SDL_SaveBMP(screenCap, filename.c_str());
  SDL_FreeSurface(screenCap);
}

