#include "playerManager.h"

PlayerManager::PlayerManager(std::string name) :
  	HQ(new HQ_NPC(name+"/HQSprite")), 
	units(),	playername(name),
	health(100),
	funds(Gamedata::getInstance().getXmlInt(name +"/startingFunds")),
	unitCapacity(Gamedata::getInstance().getXmlInt(name +"/unitCapacity")),
	hud(name, funds, unitCapacity ),
	isPlayer(Gamedata::getInstance().getXmlInt(name+"/playerTag")),
	prices(),
	currentSelectedUnit(0), canDeploy(false), currentUnit(0),
	refreshPoint(Gamedata::getInstance().getXmlInt("RefreshPoint")),
	refreshRate(Gamedata::getInstance().getXmlInt("RefreshRate")),
	lastRefresh(0)
{
	for(int i = 0; i < hud.getTotalUnitCount(); i++){
		std::stringstream s;
		s << name << "/unitPrice/i" << i;
		int tmp = Gamedata::getInstance().getXmlInt(s.str());
		prices.push_back(tmp);
	}

	//start each player off with 1 infantry sprite
		units.emplace_back(new NPC(playername+"/infantry"));

}

PlayerManager::~PlayerManager(){
	delete HQ;
	for(auto&& i : units){
		delete i;
	}

}

void PlayerManager::update(Uint32 ticks) {
	HQ->update(ticks);

	health = static_cast<HQ_NPC*>(HQ)->getHP_percent();
/*	for(auto&& i :units){ 
		i->update(ticks);
	}
*/
	if( prices[currentSelectedUnit] <= funds)
	{
		hud.unhide();
		canDeploy = true;
	}else{
		hud.hide();
		canDeploy = false;
	}
	lastRefresh += ticks;
	if(lastRefresh >= refreshRate){
		funds += refreshPoint;
		lastRefresh = 0;
	}
	hud.update(health, funds, (int) units.size() );


}

void PlayerManager::draw()const {
	HQ->draw();

	for(auto&& i :units){
		i->draw();
	}
	if(isPlayer)
	hud.draw();
}
 
void PlayerManager::deploy(){
	if(canDeploy){
		if(currentSelectedUnit == 0){
			units.emplace_back(new NPC(playername+"/tank"));
			funds -= prices[currentSelectedUnit];
		}
		if(currentSelectedUnit == 1){
			units.emplace_back(new NPC(playername+"/truck"));
			funds -= prices[currentSelectedUnit];
		}	
		if(currentSelectedUnit == 2){
			units.emplace_back(new NPC(playername+"/infantry", HQ->getX(), HQ->getY()+HQ->getScaledHeight()));
			funds -= prices[currentSelectedUnit];
		}
		if(currentSelectedUnit == 3){
			units.emplace_back(new NPC(playername+"/rocketman"));
			funds -= prices[currentSelectedUnit];
		}
	}
}
void PlayerManager::selectUnit(){
	 currentSelectedUnit =	hud.swap();
	//get the price of that unit
}


void PlayerManager::winScreen()const{
	SDL_Rect screen;
	screen.x = Gamedata::getInstance().getXmlInt(playername +"/winscreen/pos/x");
	screen.y = Gamedata::getInstance().getXmlInt(playername +"/winscreen/pos/y");
	screen.w = Gamedata::getInstance().getXmlInt(playername +"/winscreen/pos/w");
	screen.h = Gamedata::getInstance().getXmlInt(playername +"/winscreen/pos/h");

	SDL_Color color;
	color.r = (Uint8) Gamedata::getInstance().getXmlInt(playername +"/winscreen/color/r");
	color.g = (Uint8) Gamedata::getInstance().getXmlInt(playername +"/winscreen/color/g");
	color.b = (Uint8) Gamedata::getInstance().getXmlInt(playername +"/winscreen/color/b");
	color.a = (Uint8) Gamedata::getInstance().getXmlInt(playername +"/winscreen/color/a");

 	SDL_SetRenderDrawBlendMode(RenderContext::getInstance().getRenderer(),
											 	SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor( RenderContext::getInstance().getRenderer(), 
										color.r, color.g, color.b, color.a);

	SDL_RenderFillRect(RenderContext::getInstance().getRenderer(), &screen);
	SDL_RenderDrawRect(RenderContext::getInstance().getRenderer(), &screen);

	Image* img(ImageFactory::getInstance().getImage(playername +"/winscreen"));


	SDL_Color color1;
	color1.r = (Uint8) Gamedata::getInstance().getXmlInt(playername +"/winscreen/txtcolor/r");
	color1.g = (Uint8) Gamedata::getInstance().getXmlInt(playername +"/winscreen/txtcolor/g");
	color1.b = (Uint8) Gamedata::getInstance().getXmlInt(playername +"/winscreen/txtcolor/b");
	color1.a = (Uint8) Gamedata::getInstance().getXmlInt(playername +"/winscreen/txtcolor/a");

	std::string message(Gamedata::getInstance().getXmlStr(playername+"/winscreen/mssg"));
	IoMod::getInstance().writeText(message, (screen.w-screen.x)/2, (screen.h-screen.y)/2, color1);
	int x =Gamedata::getInstance().getXmlInt(playername +"/winscreen/startLoc/x");
	int y =Gamedata::getInstance().getXmlInt(playername +"/winscreen/startLoc/y");
	img->draw(x, y , 2);
	


}








