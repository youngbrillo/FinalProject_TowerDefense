#include "NPC.h"
#include "gameData.h"
#include "imageFactory.h"
float distance(float x1, float y1, float x2, float y2) {
  float x = x1-x2;
  float y = y1-y2;
  return hypot(x, y);
}

NPC::NPC(const std::string& name) :
	Drawable(name, 
		Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
				 	Gamedata::getInstance().getXmlInt(name+"/startLoc/y")),
		Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
				 	Gamedata::getInstance().getXmlInt(name+"/speedY")) ),
		images (ImageFactory::getInstance().getImages(name)),
		currentFrame(0),
		numberOfFrames(Gamedata::getInstance().getXmlInt(name+"/frames") ),
		startFrame(0),
		endFrame(numberOfFrames/2),
		frameInterval(Gamedata::getInstance().getXmlInt(name +"/frameInterval")),
		timeSinceLastFrame(0),
		worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
		worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
		invisibleWall1(Gamedata::getInstance().getXmlInt("world/ceiling")),
		invisibleWall2(Gamedata::getInstance().getXmlInt("world/floor")),
		explosion(nullptr),
		hp(new HealthBar(name)),
		mode(NORMAL),
		safeDistance(Gamedata::getInstance().getXmlInt(name+"/safeDistance")),
		acceleration(Gamedata::getInstance().getXmlInt(name+"/acceleration")),
		timeSinceLastDecision(0), 
		decisionInterval(Gamedata::getInstance().getXmlFloat(name+"/DecisionInterval")),
		initialVelocity(Gamedata::getInstance().getXmlInt(name+"/speedX"),
				 	Gamedata::getInstance().getXmlInt(name+"/speedY")),
		underPlayerControl(false),
	 	bullets("bullet"),
		minSpeed(Gamedata::getInstance().getXmlInt("bullet/minSpeed")),
		damage(Gamedata::getInstance().getXmlInt("bullet/damage")),
		value(Gamedata::getInstance().getXmlInt(name+"/value"))
{}
 
NPC::NPC(const std::string& name, int x, int y) :
	Drawable(name, 
		Vector2f(x, y),	Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
				 							Gamedata::getInstance().getXmlInt(name+"/speedY")) ),
		images (ImageFactory::getInstance().getImages(name)),
		currentFrame(0),
		numberOfFrames(Gamedata::getInstance().getXmlInt(name+"/frames") ),
		startFrame(0),
		endFrame(numberOfFrames/2),
		frameInterval(Gamedata::getInstance().getXmlInt(name +"/frameInterval")),
		timeSinceLastFrame(0),
		worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
		worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
		invisibleWall1(Gamedata::getInstance().getXmlInt("world/ceiling")),
		invisibleWall2(Gamedata::getInstance().getXmlInt("world/floor")),
		explosion(nullptr),
		hp(new HealthBar(name)),
		mode(NORMAL),
		safeDistance(Gamedata::getInstance().getXmlInt(name+"/safeDistance")),
		acceleration(Gamedata::getInstance().getXmlInt(name+"/acceleration")),
		timeSinceLastDecision(0), 
		decisionInterval(Gamedata::getInstance().getXmlFloat(name+"/DecisionInterval")),
		initialVelocity(Gamedata::getInstance().getXmlInt(name+"/speedX"),
				 	Gamedata::getInstance().getXmlInt(name+"/speedY")),
		underPlayerControl(false),
	 	bullets("bullet"),
		minSpeed(Gamedata::getInstance().getXmlInt("bullet/minSpeed")),
		damage(Gamedata::getInstance().getXmlInt("bullet/damage")),
				value(Gamedata::getInstance().getXmlInt(name+"/value"))


{}
NPC::NPC(const NPC& s) :
	Drawable(s),
	images(s.images),
	currentFrame(s.currentFrame),
	numberOfFrames( s.numberOfFrames),
	startFrame(0),
	endFrame(s.numberOfFrames /2),
	frameInterval( s.frameInterval),
	timeSinceLastFrame(s.timeSinceLastFrame),
	worldWidth( s.worldWidth ),	
	worldHeight( s.worldHeight ),  
	invisibleWall1(s.invisibleWall1),
	invisibleWall2(s.invisibleWall2),
	explosion(),
	hp( s.hp),
	mode(s.mode),
	safeDistance(s.safeDistance),
	acceleration(s.acceleration),
	timeSinceLastDecision(s.timeSinceLastDecision),
	decisionInterval(s.decisionInterval),
	initialVelocity(s.initialVelocity),
	underPlayerControl(s.underPlayerControl),
	bullets(s.bullets),
	minSpeed(s.minSpeed),
	damage(s.damage),
	value(s.value)
	{} 							

NPC& NPC::operator=(const NPC& s) {
	Drawable::operator=(s);
	images = (s.images);
	currentFrame =(s.currentFrame);
	numberOfFrames =( s.numberOfFrames);
	frameInterval = ( s.frameInterval);
	timeSinceLastFrame =(s.timeSinceLastFrame);
	worldWidth = ( s.worldWidth );
	worldHeight = ( s.worldHeight );  
	invisibleWall1 =(s.invisibleWall1);
	invisibleWall2 =(s.invisibleWall2);
	explosion = s.explosion;
	hp = s.hp;
	mode = s.mode;

	safeDistance = s.safeDistance;
	acceleration = s.acceleration;
	timeSinceLastDecision =(s.timeSinceLastDecision);
	decisionInterval =(s.decisionInterval);
	initialVelocity =(s.initialVelocity);
	underPlayerControl = s.underPlayerControl;

	bullets=(s.bullets);
	minSpeed=(s.minSpeed);
	damage=(s.damage);
	value = s.value;
	return *this;
} 

void NPC::draw() const {


	if(explosion && (hp->getCurrentLength() <= 0 )) 
		explosion->draw();
	else {
		if(getVisibility()){
			images[currentFrame]->draw(getX(), getY(), getScale() );
			hp->draw();
		}
	}
	bullets.draw();

}

void NPC::advanceFrame(Uint32 ticks)
{
	timeSinceLastFrame += ticks;


	if(timeSinceLastFrame > frameInterval)
	{
		if( getVelocityX() > 0 ){
			startFrame = 0;
			if(numberOfFrames > 1)
			endFrame = numberOfFrames/2;
		}
	
		if( getVelocityX() < 0 ){
			//make sure the current frame isn't < than the startFrame
			if(numberOfFrames > 1)
			startFrame = numberOfFrames/2;
			endFrame = numberOfFrames;
		}
			if(endFrame < 1)
				endFrame = 1;
		//advance the frame,
		currentFrame = (currentFrame+1) % endFrame; //numberOfFrames
			if(currentFrame < startFrame)
				currentFrame = startFrame;
		timeSinceLastFrame =0;
	}
}
void NPC::update(Uint32 ticks){
	advanceFrame(ticks);
	timeSinceLastDecision += ticks;	
	Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
	setPosition(getPosition() + incr);

	if(explosion){
		explosion->update(ticks);
		if(explosion ->chunkCount() == 0 ) {
			delete explosion;
			explosion = NULL;
			setExplosion();
		}
	}
	if(underPlayerControl)
 	 	mode = PLAYER;

	if( getY() < 0) 											{	setVelocityY( 	std::abs(getVelocityY())); }
	if( getY() < invisibleWall1 )							{	setVelocityY( 	std::abs(getVelocityY())); }
	if( getY() > invisibleWall2-getScaledHeight() ) {	setVelocityY( -std::abs(getVelocityY())); }
	if( getY() > worldHeight-getScaledHeight() ) 	{	setVelocityY( -std::abs(getVelocityY())); }
	if( getX() < 0) 											{	setVelocityX( 	std::abs(getVelocityX())); }
	if( getX() > worldWidth - getScaledWidth() )		{	setVelocityX( -std::abs(getVelocityX())); }

	Vector2f pos = getPosition();
	pos[1] -= 10;
	hp->update(pos);

	bullets.update(ticks);

	if(!funButton)
		stop();
}

void NPC::explode(int damageMultiplier){
	
	if(hp->getCurrentLength() > 0)
		hp->decrement(damageMultiplier);

	if(!explosion && hp->getCurrentLength() < 1){
		Sprite sprite(getName(), getPosition(), getVelocity(), images[currentFrame]);
		explosion = new ExplodingSprite(sprite);
		//setExplosion();
	}


}
void NPC::moveAwayFrom(Vector2f other){
	if ( getX() < other[0] ) goLeft();
	if	( getX() > other[0] ) goRight();
	if	( getY() < other[1] ) goUp();
	if ( getY() > other[1] ) goDown();	
}
void NPC::moveTo(Vector2f other){
	if(distance(getX(), getY(), other[0], other[1]) > 50){
	if ( getX() < other[0] ) goRight();
	if	( getX() > other[0] ) goLeft();
	if	( getY() < other[1] ) goDown();
	if ( getY() > other[1] ) goUp();	
	}
}

bool NPC::collidedWith(const Drawable* obj) const{
	return bullets.collidedWith(obj);
}








