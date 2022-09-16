
#include "sprite.h"
#include "multisprite.h"
#include "NPC.h"
#include "TransportNPC.h"
#include "hud.h"
#include "imageFactory.h"
#ifndef PLAYERMANAGER__H
#define PLAYERMANAGER__H

class ManagerHUD {
	public:
	ManagerHUD(std::string title, int cash, int capacity): 
		funds(cash), unitCount(capacity), 
  		reactions( ImageFactory::getInstance().getImages(title+"/Portrait") ),
		unitSelect( ImageFactory::getInstance().getImages(title+"/UnitSelect") ),
		numberofFramesReact(Gamedata::getInstance().getXmlInt(title+"/Portrait/frames")),
		currentFrameReact(0),
		numberofFramesUS(Gamedata::getInstance().getXmlInt(title+"/UnitSelect/frames")),
		currentFrameUS(0),
		posx(Gamedata::getInstance().getXmlInt(title+"/UnitSelect/pos/x")),
		posy(Gamedata::getInstance().getXmlInt(title+"/UnitSelect/pos/y")),
		scale(Gamedata::getInstance().getXmlInt(title+"/UnitSelect/imgScale")),
		hider({
			Gamedata::getInstance().getXmlInt(title+"/UnitSelect/pos/x"),
			Gamedata::getInstance().getXmlInt(title+"/UnitSelect/pos/y"),
			60, 60}),
		hiderColor({			
			(Uint8)Gamedata::getInstance().getXmlInt(title+ "/hider/r"),
			(Uint8)Gamedata::getInstance().getXmlInt(title+ "/hider/g"),
			(Uint8)Gamedata::getInstance().getXmlInt(title+ "/hider/b"),
			(Uint8)Gamedata::getInstance().getXmlInt(title+ "/hider/a")
			}),
		shouldHideUnit(false),
		hud(title+"/hud"),
		instructions(title+"/instructions", Gamedata::getInstance().getXmlInt(title+ "/instructions/size"))
		{ }
		void update(float currHealth, int currFunds, int currUnitCount){
			
			funds = currFunds;
			unitCount = currUnitCount;
			hud.update(currHealth, currFunds, currUnitCount);
			if(currHealth > 90) currentFrameReact = 0;
			if(currHealth < 61 && currHealth > 40) currentFrameReact = 5;
			if(currHealth < 20) currentFrameReact = 7;
			if(currHealth < 40 && currHealth > 20) currentFrameReact = 6;
		}
		void draw()const{
			hud.draw();
			hud.drawHealth();
			hud.drawFunds();
			hud.drawTroops();



			reactions[currentFrameReact]->draw(hud.displayBox.x+2,hud.displayBox.y +2 , 1);
			unitSelect[currentFrameUS]->draw(posx, posy, scale);
			if(shouldHideUnit){
					SDL_SetRenderDrawBlendMode( RenderContext::getInstance().getRenderer(),
							SDL_BLENDMODE_BLEND);
					SDL_SetRenderDrawColor(RenderContext::getInstance().getRenderer(),
					hiderColor.r, hiderColor.g, hiderColor.b, hiderColor.a );
					SDL_RenderFillRect(RenderContext::getInstance().getRenderer(), &hider);
					SDL_RenderDrawRect(RenderContext::getInstance().getRenderer(), &hider);
			}

			instructions.draw();
		}

		int swap(){
			++currentFrameUS;
			currentFrameUS = currentFrameUS%numberofFramesUS;
			return currentFrameUS;
		}

		int getTotalUnitCount(){ return numberofFramesUS; }

		void hide(){
			shouldHideUnit = true;

		}
		void unhide(){
			shouldHideUnit = false;
		}

	~ManagerHUD(){}
	private:
		int funds;
		int unitCount; //how many units are spawned
		std::vector<Image *> reactions;
		std::vector<Image *> unitSelect;
		unsigned numberofFramesReact;
		unsigned currentFrameReact;
		unsigned numberofFramesUS;
		unsigned currentFrameUS;

		int posx, posy;
		float scale;
		SDL_Rect hider;
		SDL_Color hiderColor;
		bool shouldHideUnit;
		statusHUD hud;
		HUD instructions;
};




class PlayerManager{
	public:
		PlayerManager(std::string);
		~PlayerManager();

		void update(Uint32);
		void draw() const ;
		Drawable* HQ;
		std::vector<Drawable*> units;							   	 

		void deploy();
		void selectUnit();

		void moveUnitLeft();
		void moveUnitRight();
		void moveUnitUp();
		void moveUnitdown();
		void gainPoints(int value){
			funds += value;
		}
		int getHealth() const {return health;}
		void winScreen()const ;
	private:
		std::string playername;
		PlayerManager(const PlayerManager&) = delete;
  		PlayerManager& operator=(const PlayerManager&) = delete;
/* *********************************************************** */
		float health; 														/**/	
		int funds; 														  /**/	
		int unitCapacity;			 									 /**/							
		ManagerHUD hud;										 	   /**/
		bool isPlayer;												  /**/
		std::vector<int>prices;									 /**/
		int currentSelectedUnit;								/**/
		bool canDeploy;										  /**/
		int currentUnit;										 /**/
		int refreshPoint;										/**/
		unsigned refreshRate;							  /**/
		unsigned lastRefresh;						    /**/
/* ********************************************* */
};





#endif
