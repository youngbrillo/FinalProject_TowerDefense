#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#ifndef IOMOD_H
#define IOMOD_H
class IoMod {
public:
  static IoMod& getInstance();
  ~IoMod();
  SDL_Texture* readTexture(const std::string& filename);
  SDL_Surface* readSurface(const std::string& filename);
  void writeText(const std::string&, int, int) const;
	//overloaded writeText
  void writeText(const std::string&, int, int, SDL_Color ) const;
	//overloaded writeText -> enables custom font style and size
  void writeText(const std::string& , int, int, SDL_Color, TTF_Font*) const;
  SDL_Renderer* getRenderer() const { return renderer; }
private:
  int init;
  SDL_Renderer* renderer;
  TTF_Font* font;
  SDL_Color textColor;
  IoMod();
  IoMod(const IoMod&);
  IoMod& operator=(const IoMod&);
};
#endif
