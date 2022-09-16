#include "player.h"
#include "gameData.h"
#include "imageFactory.h"

void Player::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
		if( getVelocityX() > 0 ){
			startFrame = 0;
			endFrame = numberOfFrames/2;
		}
	
		if( getVelocityX() < 0 ){
			//make sure the current frame isn't < than the startFrame
			startFrame = numberOfFrames/2;
			endFrame = numberOfFrames;
		}

		//advance the frame,
		currentFrame = (currentFrame+1) % endFrame; //numberOfFrames
			if(currentFrame < startFrame)
				currentFrame = startFrame;
		timeSinceLastFrame =0;
	}
}

Player::Player( const std::string& name) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
		  images( ImageFactory::getInstance().getImages(name) ),

		  currentFrame(0),
		  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
		  startFrame(0),
		  endFrame(numberOfFrames /2),
		  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
		  timeSinceLastFrame( 0 ),
		  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
		  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
		  initialVelocity(getVelocity()),
		  bulletName("bullet"),
		  bullets(bulletName),
		  minSpeed(Gamedata::getInstance().getXmlInt(bulletName+"/minSpeed")),
		  damage(Gamedata::getInstance().getXmlInt(bulletName+"/damage"))
	
{ }

Player::Player(const Player& s) :
  Drawable(s), 
  images(s.images),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  startFrame(0),
  endFrame(s.numberOfFrames /2),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  initialVelocity( s.initialVelocity ),
	bulletName(s.bulletName),
	bullets(s.bullets),
	minSpeed(s.minSpeed), damage(s.damage)
	
  { }

Player& Player::operator=(const Player& s) {
  Drawable::operator=(s);
  images = (s.images);
  currentFrame = (s.currentFrame);
  numberOfFrames = ( s.numberOfFrames );
  startFrame =( s.startFrame);
  endFrame = (s.numberOfFrames /2);
  frameInterval = ( s.frameInterval );
  timeSinceLastFrame = ( s.timeSinceLastFrame );
  worldWidth = ( s.worldWidth );
  worldHeight = ( s.worldHeight );
  initialVelocity = ( s.initialVelocity );
	bulletName = s.bulletName;
	bullets = s.bullets;
	minSpeed = s.minSpeed;
	damage = s.damage;
  return *this;
}

void Player::draw() const { 
  images[currentFrame]->draw(getX(), getY(), getScale());

	bullets.draw();
}

void Player::stop() { 
  setVelocity( Vector2f(0, 0) );
  //setVelocityX( 0.93*getVelocityX() );
  setVelocityY(0);
}

void Player::right() { 
  if ( getX() < worldWidth-getScaledWidth()) {
    setVelocityX(initialVelocity[0]);
  }
} 
void Player::left()  { 
  if ( getX() > 0) {
    setVelocityX(-initialVelocity[0]);
  }
} 
void Player::up()    { 
  if ( getY() > 0) {
    setVelocityY( -initialVelocity[1] );
  }
} 
void Player::down()  { 
  if ( getY() < worldHeight-getScaledHeight()) {
    setVelocityY( initialVelocity[1] );
  }
}

void Player::update(Uint32 ticks) { 
  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( getY() < 0) {
    setVelocityY( fabs( getVelocityY() ) );
  }
  if ( getY() > worldHeight-getScaledHeight()) {
    setVelocityY( -fabs( getVelocityY() ) );
  }

  if ( getX() < 0) {
    setVelocityX( fabs( getVelocityX() ) );
  }
  if ( getX() > worldWidth-getScaledWidth()) {
    setVelocityX( -fabs( getVelocityX() ) );
  }  


  stop();

	bullets.update(ticks);
}

bool Player::collidedWith(const Drawable* obj) const{
		//bullet collision func
		return bullets.collidedWith(obj );
}

void Player::shoot(){
	float x = getX()+getImage()->getWidth();
	float y = getY()+getImage()->getHeight()/2;
	if(currentFrame > numberOfFrames/2){
		x = getX();
		bullets.shoot( Vector2f(x,y) , Vector2f(-minSpeed+getVelocityX(), 0) );
	}else{
		bullets.shoot( Vector2f(x,y) , Vector2f(minSpeed+getVelocityX(), 0) );
	}

}










