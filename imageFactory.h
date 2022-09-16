#include <string>
#include <vector>
#include <map>
#include "image.h"
#include "gameData.h"
#include "spriteSheet.h"

//a singleton's requirements
	// all constructor's are private			 
	//	declare a static instance variable 
	//create a static function to get the static instance variable
	// define static instance variable in global memory 

#ifndef IMAGEFACTORY_H
#define IMAGEFACTORY_H

class ImageFactory {
public:

  //static ImageFactory* getInstance();
	 static ImageFactory& getInstance();
  ~ImageFactory();

  Image* getImage(const std::string&);
  std::vector<Image*> getImages(const std::string&);

private:
 // static ImageFactory* instance;
  const Gamedata& gdata;

  std::map<std::string, SDL_Surface*> surfaces;
  std::map<std::string, SDL_Texture*> textures;
  std::map<std::string, Image*> images;

  std::map<std::string, std::vector<SDL_Surface*> > multiSurfaces;
  std::map<std::string, std::vector<SDL_Texture*> > multiTextures;
  std::map<std::string, std::vector<Image*> > multiImages;

  ImageFactory() : 
    gdata( Gamedata::getInstance() ), 
    surfaces(),
    textures(),
    images(),
    multiSurfaces(),
    multiTextures(),
    multiImages()
  {}
  ImageFactory(const ImageFactory&) = delete;
  ImageFactory& operator=(const ImageFactory&) = delete;
};

#endif

