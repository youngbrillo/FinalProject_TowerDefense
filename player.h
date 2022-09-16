


#ifndef PLAYER__H
#define PLAYER__H

#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"
#include "bulletPool.h"

class Player : public Drawable {
public:
  Player(const std::string&);
  Player(const Player&);

  virtual void draw() const;
  virtual void update(Uint32 ticks);

  virtual const Image* getImage() const { 
    return images[currentFrame]; 
  }
  int getScaledWidth()  const { 
    return getScale()*images[currentFrame]->getWidth(); 
  } 
  int getScaledHeight()  const { 
    return getScale()*images[currentFrame]->getHeight(); 
  } 
  virtual const SDL_Surface* getSurface() const { 
    return images[currentFrame]->getSurface();
  }

  void right();
  void left();
  void up();
  void down();
  void stop();	
	virtual void goLeft(){

	}
	virtual void goRight(){

	
	}
	virtual void goUp(){

	}
	virtual void goDown(){
	
	}


	//bullet stuff
	void shoot();
	virtual bool collidedWith(const Drawable*) const;

	virtual  int getBulletCount() const{ return bullets.bulletCount(); }

	virtual  int getFreeCount()const {return bullets.freeCount(); }

	int getDamage(){return damage; }
	virtual int getValue() const{return damage;}

private:
  std::vector<Image *> images;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned startFrame;
  unsigned endFrame;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;
  Vector2f initialVelocity;
  void advanceFrame(Uint32 ticks);

	//bullet stuff
  	std::string bulletName;
  	BulletPool bullets;
  	float minSpeed;
	int damage;
	
	
  


  Player& operator=(const Player&);
};

















#endif















