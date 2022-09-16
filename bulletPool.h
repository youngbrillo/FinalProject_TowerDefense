

#include <list>
#include "bullet.h"
#include "collisionStrategy.h"

#ifndef BULLETPOOL__H
#define BULLETPOOL__H

class BulletPool {
public:
  BulletPool(const std::string&);
  BulletPool(const BulletPool&);
  ~BulletPool();
  void draw() const;
  void update(Uint32 ticks);

  void shoot(const Vector2f& pos, const Vector2f& vel);

   int bulletCount() const { return (int) bulletList.size(); }
   int freeCount()  const { return  (int) freeList.size(); }
  bool shooting() const { return bulletList.empty(); }
  bool collidedWith(const Drawable* obj) const;
  BulletPool& operator=(const BulletPool&);   // (Item 6)
private:
  std::string name;
  CollisionStrategy* strategy;
  float frameInterval;
  float timeSinceLastFrame;
  mutable std::list<Bullet> bulletList; // BulletPool is a list of Bullet
  mutable std::list<Bullet> freeList;   // when a Bullet is out of range
	unsigned int listSize;

};
#endif
