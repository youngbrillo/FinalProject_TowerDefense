
#ifndef BULLET__H
#define BULLET__H
#include <iostream>
#include "sprite.h"
#include "gameData.h"

class Bullet : public Sprite {
public:
  explicit Bullet(const string& name) :
    Sprite(name), 
    distance(0), 
    maxDistance(Gamedata::getInstance().getXmlInt(name+"/distance")), 
    tooFar(false), 
	 rightEdge(Gamedata::getInstance().getXmlInt("world/width")),
	 bottomEdge(Gamedata::getInstance().getXmlInt("world/height"))
  { }
  virtual void update(Uint32 ticks);
  bool goneTooFar() const { return tooFar; }
  void reset() {
    tooFar = false;
    distance = 0;
  }

private:
  float distance;
  float maxDistance;
  bool tooFar;
  int rightEdge;
  int bottomEdge;
};
#endif
