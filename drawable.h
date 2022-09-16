#ifndef DRAWABLE__H
#define DRAWABLE__H
#include <SDL.h>
#include <iostream>
#include <string>
#include "vector2f.h"
#include "image.h"

#include <vector>
#include "gameData.h"
#include "ioMod.h"
#include <sstream>

// Drawable is an Abstract Base Class (ABC) that specifies the methods
// that derived classes may or must have.
class Drawable {
public:
  Drawable(const std::string& n, const Vector2f& pos, const Vector2f& vel): 
    name(n), position(pos), velocity(vel), scale(1.0) 
		,listeners() , isVisible(true), hasExploded(false), ObserversPos(pos), deployFlag(false)
	{}

  Drawable(const Drawable& s) : 
    name(s.name), position(s.position), velocity(s.velocity), scale(s.scale) 
	,listeners(),  isVisible(true), hasExploded(false), ObserversPos(s.ObserversPos), 
	 deployFlag(s.deployFlag)
    { }

//virtual Drawable& operator=(const Drawable& rhs);

  virtual ~Drawable() {

	}

  virtual void draw() const = 0;
  virtual void update(Uint32 ticks) = 0;

  float getScale() const  { return scale; }
  void  setScale(float s) { scale = s; }
  virtual int getScaledWidth() const = 0;
  virtual int getScaledHeight() const = 0;

  const std::string& getName() const { return name; }
  void setName(const std::string& n) { name = n;    }

  virtual const Image* getImage() const = 0;

  float getX() const  { return position[0]; }
  void  setX(float x) { position[0] = x;    }

  float getY() const  { return position[1]; }
  void  setY(float y) { position[1] = y;    }

  const Vector2f& getVelocity() const    { return velocity; }
  void  setVelocity(const Vector2f& vel) { velocity = vel;  }
  const Vector2f& getPosition() const    { return position; }
  void  setPosition(const Vector2f& pos) { position = pos;  }

  float getVelocityX() const   { return velocity[0]; }
  void  setVelocityX(float vx) { velocity[0] = vx;   }
  float getVelocityY() const   { return velocity[1]; }
  void  setVelocityY(float vy) { velocity[1] = vy;   }


	//wasn't included but still needs to be here
  virtual const SDL_Surface* getSurface() const = 0;



	//my custom methods

	void listenTo(Drawable* broadcaster) {broadcaster->gain(this);}
	 void gain(Drawable* obj){
		listeners.push_back(obj);
		ObserversPos = obj->getPosition();
	}

	 void listen(Vector2f pos){	//pos - broadcasters position
		ObserversPos = pos;	
	}
	 void broadcast(){
		for(auto&& it : listeners){
			it->listen(position);		
		}

	}
	void detach(Drawable* obj){
		std::vector<Drawable*>::iterator ptr = listeners.begin();
		while(ptr != listeners.end()){
			if(*ptr == obj) {
				ptr = listeners.erase(ptr);
				return;
			}
			++ptr;
		}
	}	


	virtual void explode(int) {throw std::string("I don't explode"); }
	 bool  checkExplosion() { return hasExploded; }
	 void setExplosion() { hasExploded = true; }
	 void resetExplosion() {hasExploded = false; }
	





	//debug functions
	//call this in draw
	void PrintPosition()const {
		std::stringstream string;
		string << name << "( " << position[0] << " , " << position[1] << ")";

		
		int x = Gamedata::getInstance().getXmlInt(name +"/debugLoc/x");
		int y = Gamedata::getInstance().getXmlInt(name +"/debugLoc/y");
		IoMod::getInstance().writeText(string.str(), x, y);
	}

	Vector2f getObserversPos() const {return ObserversPos; }
	
	void raisDeployFlag(){deployFlag = true;}
	void lowerDeployFlag(){deployFlag = false;}
	bool getDeployFlag(){return deployFlag;}


		float distance(float x1, float y1, float x2, float y2) {
		  float x = x1-x2;
		  float y = y1-y2;
		  return hypot(x, y);
		}
	
	bool flipVisibility() { 
		isVisible ? isVisible = false : isVisible = true;
		return isVisible;
	}
	bool getVisibility()const { return isVisible; }

	virtual void goLeft() = 0;
	virtual void goRight() = 0;
	virtual void goUp() = 0;
	virtual void goDown() =0;
	virtual void stop() { setVelocity(Vector2f(0,0)); }
	virtual void shoot(){
		throw std::string("i can't shoot, you need to overload me\n");
	}

	virtual bool collidedWith(const Drawable* ) const = 0;
	virtual int getDamage() const = 0;
	virtual int getValue() const = 0;
private:
  std::string name;
  Vector2f position;
  Vector2f velocity;
  float scale;
 
  std::vector<Drawable*> listeners;
  bool isVisible;
  bool hasExploded;
  Vector2f ObserversPos;
  bool deployFlag;

};

	//int Drawable::range = GameData::getInstance().getXmlInt("player/triggerRange");
#endif




















