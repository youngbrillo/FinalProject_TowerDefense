#include <iostream>
#include <cmath>
#include "bullet.h"
#include "collisionStrategy.h"

void Bullet::update(Uint32 ticks) { 
  Vector2f pos = getPosition();
  Sprite::update(ticks);
  distance += ( hypot(getX()-pos[0], getY()-pos[1]) );
  if (distance > maxDistance) tooFar = true;
	

	//needs to be a check that says if the bullet goes off screen, then it has also 
	//gone to far
	if( pos[0] > rightEdge){
		//tooFar = true;
	}
}

