
#include "ioMod.h"
#include "drawable.h"

#ifndef	STATBAR__H
#define	STATBAR__H
class HealthBar {//: public Drawable {
public:
  HealthBar(const std::string&);
  HealthBar(const std::string& name, int x, int y);

  void draw() const;
//  void update(Uint32);
  void update(Vector2f&);
  void reset() { currentLength = barRect.w; }

  HealthBar(const HealthBar&);
  HealthBar& operator=(const HealthBar&);

	//void for now but may want to make this return an int, to have like damage numbers pop up on the screen
	void decrement(int scale){ //scale should be  0 - 3, anything more than that is a bit much
		if(scale < 0){		
			scale = 1;
		}
		currentLength -= (increment *scale );
	}

	int getCurrentLength()const {return currentLength;}
	int getPercentage() const {
		float i = (currentLength * 1.0)/(barRect.w*1.0);
		return i * 100;
	}

private:
	//just needs a position
  Vector2f position;
  std::string title;
  SDL_Renderer* renderer;
  SDL_Rect barRect;

  int currentLength;
  int increment;  

  SDL_Color barColor;
  SDL_Color backColor;
  IoMod& io;

};
#endif
