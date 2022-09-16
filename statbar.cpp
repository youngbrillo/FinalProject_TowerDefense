#include "statbar.h"
#include "renderContext.h"
#include "gameData.h"
#include<sstream>
#include "viewport.h"

HealthBar::HealthBar(const std::string& name) :  
   position(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
  title(name),
  renderer(RenderContext::getInstance().getRenderer()),
  barRect( { Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
             Gamedata::getInstance().getXmlInt(name+"/startLoc/y"),
             Gamedata::getInstance().getXmlInt(name+"/hp/width"),
             Gamedata::getInstance().getXmlInt(name+"/hp/height")}
  ),
  currentLength(barRect.w), 
  increment(barRect.w/Gamedata::getInstance().getXmlInt(name+"/hp/increment")),
  barColor(
    {(Uint8)Gamedata::getInstance().getXmlInt(name+"/hp/barColor/r"),
     (Uint8)Gamedata::getInstance().getXmlInt(name+"/hp/barColor/g"),
     (Uint8)Gamedata::getInstance().getXmlInt(name+"/hp/barColor/b"),
     (Uint8)Gamedata::getInstance().getXmlInt(name+"/hp/barColor/a")}
  ),
  backColor(
    {(Uint8)Gamedata::getInstance().getXmlInt(name+"/hp/backColor/r"),
     (Uint8)Gamedata::getInstance().getXmlInt(name+"/hp/backColor/g"),
     (Uint8)Gamedata::getInstance().getXmlInt(name+"/hp/backColor/b"),
     (Uint8)Gamedata::getInstance().getXmlInt(name+"/hp/backColor/a")}
  ),
  io( IoMod::getInstance() )
{}

HealthBar::HealthBar(const std::string& name, int x, int y) :
    position(Gamedata::getInstance().getXmlInt(name+"/startLoc/x") + x, 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y") + y), 
 	title(name),
  renderer(RenderContext::getInstance().getRenderer()),
  barRect( { Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
             Gamedata::getInstance().getXmlInt(name+"/startLoc/y"),
             Gamedata::getInstance().getXmlInt(name+"/hp/width"),
             Gamedata::getInstance().getXmlInt(name+"/hp/height")}
  ),
  currentLength(barRect.w), 
  increment(barRect.w/Gamedata::getInstance().getXmlInt(name+"/hp/increment")),
  barColor(
    {(Uint8)Gamedata::getInstance().getXmlInt(name+"/hp/barColor/r"),
     (Uint8)Gamedata::getInstance().getXmlInt(name+"/hp/barColor/g"),
     (Uint8)Gamedata::getInstance().getXmlInt(name+"/hp/barColor/b"),
     (Uint8)Gamedata::getInstance().getXmlInt(name+"/hp/barColor/a")}
  ),
  backColor(
    {(Uint8)Gamedata::getInstance().getXmlInt(name+"/hp/backColor/r"),
     (Uint8)Gamedata::getInstance().getXmlInt(name+"/hp/backColor/g"),
     (Uint8)Gamedata::getInstance().getXmlInt(name+"/hp/backColor/b"),
     (Uint8)Gamedata::getInstance().getXmlInt(name+"/hp/backColor/a")}
  ),
  io( IoMod::getInstance() )
{}


HealthBar::HealthBar(const HealthBar& hp) :
	position(hp.position), title(hp.title),
  	renderer(RenderContext::getInstance().getRenderer()),
	barRect(hp.barRect),
	currentLength(hp.currentLength),
	increment(hp.increment),
	barColor(hp.barColor),
	backColor(hp.backColor),
	io(IoMod::getInstance() )
{}

HealthBar& HealthBar::operator=(const HealthBar& hp){
	position = (hp.position); title = (hp.title);
	barRect =(hp.barRect);
	currentLength =(hp.currentLength);
	increment =(hp.increment);
	barColor = (hp.barColor);
	backColor = (hp.backColor);
	return *this;

}
void HealthBar::update(Vector2f& parentPos){
	position = (parentPos);
} 

void HealthBar::draw() const {
  int x = static_cast<int>(position[0]);
  x -= Viewport::getInstance().getX();
  int y = static_cast<int>(position[1]);
  y -= Viewport::getInstance().getY();

  SDL_Rect back = {x, y, barRect.w, barRect.h};
  // First set the blend mode so that alpha blending will work;
  // the default blend mode is SDL_BLENDMODE_NONE!
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  // Set the health bar background color:
  SDL_SetRenderDrawColor( renderer, backColor.r, backColor.g, backColor.b, backColor.a );
  // Draw the  background
  SDL_RenderFillRect( renderer, &back );
  SDL_SetRenderDrawColor( renderer, barColor.r, barColor.g, barColor.b, barColor.a );
  SDL_Rect healthRect = {x, y, currentLength, barRect.h};
  SDL_RenderFillRect( renderer, &healthRect );
  // Set the  color for the health bar outline:
  SDL_SetRenderDrawColor( renderer, barColor.r, barColor.g, barColor.b, barColor.a );
  SDL_RenderDrawRect( renderer, &back );



	std::stringstream s;
	s << getPercentage() << "%";
	IoMod::getInstance().writeText(s.str(),x, y -15);

}



