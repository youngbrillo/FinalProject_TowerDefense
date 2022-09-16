#ifndef NPC__H
#define NPC__H
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"
#include "explodingSprite.h"
#include "statbar.h"
#include "bulletPool.h"
class NPC : public Drawable {
	public:
		NPC( const std::string &);
		NPC(const std::string&, int, int);
		NPC( const NPC& s);
		virtual void draw() const;
		virtual void update(Uint32 ticks);
		 const Image* getImage() const{
			return images[currentFrame];
		}
		int getScaledWidth() const {
			return getScale()*images[currentFrame]->getWidth();
		}
		int getScaledHeight() const {
			return getScale()*images[currentFrame]->getHeight();
		}
		 const SDL_Surface* getSurface() const {
			return images[currentFrame]->getSurface() ;
		}
		 void explode(int);
	/*	virtual bool checkExplosion(){
			if (explosion->chunkCount() == 0) 
				return true; 
			else 
				return false;
		}
	*/
		virtual ~NPC(){if(explosion) {delete explosion; } delete hp; }

		 void moveAwayFrom(Vector2f other);
		 void moveTo(Vector2f other);
		virtual  void goLeft() {
		 	if( getX() > 0) {
			 	setVelocityX(-initialVelocity[0]);
			}
		}
		virtual void goRight(){
		  if ( getX() < worldWidth-getScaledWidth()) {
			 setVelocityX(initialVelocity[0]);
		  }
		}
		virtual void goUp(){
		  if ( getY() > 0) {
			 setVelocityY( -initialVelocity[1] );
		  }
	
		}
		virtual void goDown(){
		  if ( getY() < worldHeight-getScaledHeight()) {
			 setVelocityY( initialVelocity[1] );
		  }

		}
		virtual void shoot(){
			float x = getX()+getImage()->getWidth();
			float y = getY()+getImage()->getHeight()/2;
			if(currentFrame > numberOfFrames/2){
				x = getX();
				bullets.shoot( Vector2f(x,y) , Vector2f(-minSpeed+getVelocityX(), 0) );
			}else{
				bullets.shoot( Vector2f(x,y) , Vector2f(minSpeed+getVelocityX(), 0) );
			}

		}

		int getHP() const {return hp->getCurrentLength(); }
		float getHP_percent() const { return hp->getPercentage(); }

		void gainControl() {underPlayerControl = true;}
		void releaseControl() {underPlayerControl = false;}

		bool isThisUnderPlayerControl() {return underPlayerControl;}
		virtual bool collidedWith(const Drawable* obj) const;
		int getDamage()const{return damage;}

		int getValue() const {return value;}
		void upTheAnte(){
			if(funButton)
				funButton = false;
			else
				funButton = true;
		}
	protected:	
		enum MODE {NORMAL, EVADE, FIGHT, DEPLOY, CAPTURE, PLAYER};
		std::vector<Image *> images;
		unsigned currentFrame;
		unsigned numberOfFrames;
		unsigned startFrame;  //will initially be images[0]
		unsigned endFrame; 	 //will initially be images[numberOfFrames /2]
		unsigned frameInterval;
		float timeSinceLastFrame;
		int worldWidth;
		int worldHeight;
		int invisibleWall1;
		int invisibleWall2;
		void advanceFrame(Uint32 ticks);
		NPC& operator=(const NPC& );
		ExplodingSprite* explosion;
		HealthBar* hp;
		enum MODE mode;
		int safeDistance;
		float acceleration;
		float timeSinceLastDecision;
		float decisionInterval;
		Vector2f initialVelocity;
		bool underPlayerControl;

		BulletPool bullets;
		float minSpeed;
		int damage;

		int value;
		static bool funButton;
};


class HQ_NPC : public NPC{
	public:
		HQ_NPC(std::string name) : NPC(name){std::cout << "made a HQ SPRITE\n";}
		void update(Uint32 ticks){
			NPC::update(ticks);
			setVelocityX(0.0);
			setVelocityY(0.0);
		}
		void draw()const{
			NPC::draw();

		}
		bool collidedWith(const Drawable* obj) const{
			return NPC::collidedWith(obj);
		}
	private:

};























#endif
