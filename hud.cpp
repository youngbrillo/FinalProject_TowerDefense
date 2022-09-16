#include "hud.h"






HUD::HUD() : isVisible(true), display(), title("default")
{
	display.push_back("****");

}
HUD::HUD(std::string messg, int size) : isVisible(true), display(), title(messg)
{
	display.reserve(size);
	for(int i = 0; i < size; i++){
		//display.push_back( Gamedata::getInstance().getXmlStr(title+"/message"+i)) );
		
		std::stringstream s;
		s << title << "/message" << i;
		std::string in = s.str();
	//	std::cout << i << ") looking for: " << in <<std::endl;
		display.push_back(Gamedata::getInstance().getXmlStr(in));
	}

}		

void HUD::draw() const {
	if(isVisible && title != "default"){
		//draw a transparent background for the hud
		SDL_Rect r;
		r.x = Gamedata::getInstance().getXmlInt(title + "/loc/x");
		r.y = Gamedata::getInstance().getXmlInt(title + "/loc/y");	
		r.w = Gamedata::getInstance().getXmlInt(title + "/loc/w");
		r.h = Gamedata::getInstance().getXmlInt(title + "/loc/h");

 		SDL_SetRenderDrawBlendMode(	RenderContext::getInstance().getRenderer(),
											 	SDL_BLENDMODE_BLEND);
		int red = Gamedata::getInstance().getXmlInt(title + "/r_color/r");
		int g = Gamedata::getInstance().getXmlInt(title + "/r_color/g");
		int b = Gamedata::getInstance().getXmlInt(title + "/r_color/b");
		int a = Gamedata::getInstance().getXmlInt(title + "/r_color/a");

		SDL_SetRenderDrawColor( RenderContext::getInstance().getRenderer(), 
										red, g, b, a);
		SDL_RenderFillRect(RenderContext::getInstance().getRenderer(), &r);
		SDL_RenderDrawRect(RenderContext::getInstance().getRenderer(), &r);
		Vector2f pos(	r.x, r.y);

		SDL_Color textColor({0xff,0, 0,0});
		textColor.r = Gamedata::getInstance().getXmlInt(title + "/color/r");
		textColor.g = Gamedata::getInstance().getXmlInt(title + "/color/g");
		textColor.b = Gamedata::getInstance().getXmlInt(title + "/color/b");
		textColor.a = Gamedata::getInstance().getXmlInt(title + "/color/a");


		int offset = Gamedata::getInstance().getXmlInt(title +"/spacing");
		for(unsigned i =0; i< display.size(); i++){
			IoMod::getInstance().writeText( display[i], pos[0], ( pos[1]+ (i*offset) ), textColor);
		}

	}
	else if(isVisible && title == "default"){
		SDL_Rect r;
		r.x = 5; 	r.y = 5;		r.w = 30;	r.h = 30;
		SDL_SetRenderDrawBlendMode(	RenderContext::getInstance().getRenderer(),
											 	SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor( RenderContext::getInstance().getRenderer(), 
										0, 100, 150, 175);
		SDL_RenderFillRect(RenderContext::getInstance().getRenderer(), &r);
		SDL_RenderDrawRect(RenderContext::getInstance().getRenderer(), &r);
			IoMod::getInstance().writeText("!", 17, 10);
	}



}

void HUD::update(){


}
void HUD::flipVisibility(){
	isVisible ? isVisible = false : isVisible = true;
}


FPS_HUD::FPS_HUD(): FPScounter("FPS( )"), clock(Clock::getInstance())
{

}
FPS_HUD::FPS_HUD(	std::string name, int size): HUD(name, size), 
						FPScounter("FPS( )"), clock(Clock::getInstance() )
{}
void FPS_HUD::update( ){
	std::stringstream ss;
	ss << "FPS(" << Clock::getInstance().getFps() << ")";
	FPScounter = ss.str();
	replace(FPScounter, 0);

}

COUNT_HUD::COUNT_HUD(std::string name, int size, int replaceCount) : 
	HUD(name, size), args(replaceCount), subs(), count(0){
	subs.reserve(args);		
	for(int i =0; i < args; i++){
		std::stringstream str;
		str << name << "/replace" << i;
		subs.push_back(Gamedata::getInstance().getXmlStr(str.str() ));
	}
}

void COUNT_HUD::update(int score){
	for(int i =0; i < args;i++){
		std::stringstream str;
		count += score;
		str << subs[i] << count;

		replace(str.str(), i);

		//change this so that i can't keep adding points after shooting down an enemy
	}
}







































