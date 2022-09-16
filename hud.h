#ifndef HUD__H
#define HUD__H


#include "gameData.h"
#include <SDL_image.h>
#include <sstream>
#include <string>
#include "clock.h"
#include "renderContext.h"
#include "ioMod.h"
#include <SDL2/SDL.h>

#include <vector>


//where i would use a Renderer pointer object (which i don't want to make for...reasons)
//use	:			RenderContext::getInstance().getRenderer()
class HUD{
	public:
		HUD();
		HUD(std::string messg, int size);		
		virtual ~HUD() {}
		virtual void draw() const;
		virtual void update(); 
		virtual void update( int,  int){

		}
		virtual void update(int){
	
		}
		void flipVisibility();

		void replace(std::string a, int i){
			if(i < (int) display.size()){
				display[i] = a;
			}
		}
	protected:
		bool isVisible;
	
	private:
		std::vector< std::string > display;
		std::string title;

};

class FPS_HUD : public HUD{
	public:
		FPS_HUD();
		FPS_HUD(std::string title, int size);
		virtual void update();
	private:
		std::string FPScounter;
		Clock& clock;
};

class POOL_HUD :public HUD{
	public:
		POOL_HUD(std::string name, int size) : HUD(name, size), freeList("FREE"), poolList("Pool") {}
		virtual void update( int freeListCount,  int inPoolListCount){
			std::stringstream ss;
			ss << "Active Projectiles: " << freeListCount;
			freeList = ss.str();
			replace(freeList, 0);
			ss.clear();
			ss.str("");
			ss << "Projectile Pool: " << inPoolListCount;
			poolList = ss.str();
			replace(poolList, 1);
		}
	private:
		std::string freeList;
		std::string poolList;
};

class COUNT_HUD :public HUD{
	public:
		COUNT_HUD(std::string name, int size, int replaceCount);
		virtual void update(int score);
	private:
		int args;
		std::vector< std::string > subs;
		int count;
};

class statusHUD{
	public:
		statusHUD(std::string title) :
		displayBox({	Gamedata::getInstance().getXmlInt(title + "/loc/x"),
							Gamedata::getInstance().getXmlInt(title+	"/loc/y"),
							Gamedata::getInstance().getXmlInt(title+	"/loc/w"),
							Gamedata::getInstance().getXmlInt(title+	"/loc/h") 
						}),
		boxColor({ 
			(Uint8)Gamedata::getInstance().getXmlInt(title+ "/boxColor/r"),
			(Uint8)Gamedata::getInstance().getXmlInt(title+ "/boxColor/g"),
			(Uint8)Gamedata::getInstance().getXmlInt(title+ "/boxColor/b"),
			(Uint8)Gamedata::getInstance().getXmlInt(title+ "/boxColor/a"),
			}),
		textColor({ 
			(Uint8)Gamedata::getInstance().getXmlInt(title+ "/text/r"),
			(Uint8)Gamedata::getInstance().getXmlInt(title+ "/text/g"),
			(Uint8)Gamedata::getInstance().getXmlInt(title+ "/text/b"),
			(Uint8)Gamedata::getInstance().getXmlInt(title+ "/text/a"),
			}),
		offset(Gamedata::getInstance().getXmlInt(title+"/spacing")),
		health(Gamedata::getInstance().getXmlStr(title+ "/healthtxt")), h(health),
		funds(Gamedata::getInstance().getXmlStr(title+ "/fundstxt")), f(funds),
		troops(Gamedata::getInstance().getXmlStr(title+ "/troopstxt")), t(troops)
	{}


	void drawHealth() const{
		IoMod::getInstance().writeText( health, displayBox.x, displayBox.y, textColor);
	}
	void drawFunds() const{
		IoMod::getInstance().writeText(funds, displayBox.x, (displayBox.y + offset), textColor);
	}
	void drawTroops() const{
		IoMod::getInstance().writeText( troops, displayBox.x, (displayBox.y + offset*2), textColor);
	}



	void draw() const {
		SDL_SetRenderDrawBlendMode( RenderContext::getInstance().getRenderer(),
			SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(RenderContext::getInstance().getRenderer(),
			boxColor.r, boxColor.g, boxColor.b, boxColor.a );
		SDL_RenderFillRect(RenderContext::getInstance().getRenderer(), &displayBox);
		SDL_RenderDrawRect(RenderContext::getInstance().getRenderer(), &displayBox);
		
	}

	void update(float hp, int munny, int manpwr){	
		std::stringstream stream; 
			stream << h << ": " << hp;
		health = stream.str();
		stream.str(std::string());
			stream << f << ": " << munny;
		funds = stream.str();
		stream.str(std::string());
			stream << t << ": " << manpwr;
		troops = stream.str();
	}
		SDL_Rect displayBox;
	private:

 		SDL_Color boxColor;
		SDL_Color textColor;
		int offset;
		std::string health, h;
		std::string funds, f;
		std::string troops, t;


};
#endif
/**/


