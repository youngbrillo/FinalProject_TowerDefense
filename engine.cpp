#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <iomanip>
#include "sprite.h"
#include "multisprite.h"
#include "NPC.h"
#include "TransportNPC.h"
#include "gameData.h"
#include "engine.h"
#include "frameGenerator.h"

#include <cstdlib>
#include <ctime>


Engine::~Engine() { 
	delete collisionStrategy;
	delete mainHUD;
	delete bulletPoolHud;
	delete pointHUD;
	delete p1Pointer;
	delete p2Pointer;
  std::cout << "Terminating program" << std::endl;
}
bool NPC::funButton = false;


Engine::Engine() :
  	rc( RenderContext::getInstance() ),
  	io( IoMod::getInstance() ),
  	clock( Clock::getInstance() ),
  	renderer(rc.getRenderer()),
  	background("back", Gamedata::getInstance().getXmlInt("back/factor") ),
	foreground("fore", Gamedata::getInstance().getXmlInt("fore/factor") ),

  	viewport( Viewport::getInstance() ),
  	currentSprite(0),  	ecurrentSprite(0),
  	makeVideo( false  ),
	mainHUD( new FPS_HUD("MainHud", Gamedata::getInstance().getXmlInt("MainHud/size"))	),
	collisionStrategy(new PerPixelCollisionStrategy() ),
	bulletPoolHud( new POOL_HUD("Bullet_Hud", Gamedata::getInstance().getXmlInt("Bullet_Hud/size")) ),
	pointHUD(new COUNT_HUD("Count_Hud", Gamedata::getInstance().getXmlInt("Count_Hud/size"), 
									Gamedata::getInstance().getXmlInt("Count_Hud/NumToReplace"))),
	playerManager("PlayerHQ"),
	opponent("EnemyHQ"),
	p1Pointer(new MultiSprite("p2Pointer")),
	p2Pointer(new MultiSprite("p1Pointer")),  sound()


{
	Viewport::getInstance().setObjectToTrack(playerManager.units[currentSprite]);
	//sound.startMusic();

	playerManager.units[currentSprite]->gain( opponent.units[0]);
	playerManager.HQ->gain(opponent.units[0]);
	
	opponent.units[0]->gain(playerManager.units[0]);
	opponent.HQ->gain(playerManager.units[0]);
  	std::cout << "Loading complete" << std::endl;
}

void Engine::draw() const {



  	background.draw();
	foreground.draw();


	playerManager.draw();
	opponent.draw();
	if(opponent.getHealth() < 1){
		if(opponent.HQ->checkExplosion()){
			playerManager.winScreen();
			clock.pause();
		}
	}
	if(playerManager.getHealth() < 1){
		if(playerManager.HQ->checkExplosion()){
			opponent.winScreen();
			clock.pause();
		}
	}




	if(playerManager.units.size() > 0){
		Vector2f pos = playerManager.units[currentSprite]->getPosition();

		pos[0]+= 0;//playerManager.units[currentSprite]->getScaledWidth()/2;
		pos[1] -= 50;

		p1Pointer->setPosition(pos);
	}
	else{
		Vector2f pos = playerManager.HQ->getPosition();

		pos[0] += 0;//playerManager.HQ->getScaledWidth()/2;
		pos[1] -= 50;

		p1Pointer->setPosition(pos);
	}

	if(opponent.units.size() > 0){
		Vector2f pos = opponent.units[ecurrentSprite]->getPosition();

		pos[0]+= 0;//playerManager.units[currentSprite]->getScaledWidth()/2;
		pos[1] -= 50;

		p2Pointer->setPosition(pos);
	}
	else{
		Vector2f pos = opponent.HQ->getPosition();

		pos[0] += 0;//opponent.HQ->getScaledWidth()/2;
		pos[1] -= 50;

		p2Pointer->setPosition(pos);
	}



	p1Pointer->draw();
	p2Pointer->draw();



	viewport.draw();
  	SDL_RenderPresent(renderer);
}

void Engine::update(Uint32 ticks) {

	//have to manually update every object loaded in 
	playerManager.update(ticks);
	opponent.update(ticks);
	playerManager.HQ->broadcast();
	for(auto&& i: playerManager.units){
		i->update(ticks);
		i->broadcast();
		
		for(auto&& o : opponent.units)
		{
			if(i->collidedWith(o) && !(o->checkExplosion())){
				o->explode(i->getDamage());
			}

			if(i->collidedWith(o) && o->checkExplosion()){
				playerManager.gainPoints(o->getValue());
			}
		}
	
		if(i->collidedWith(opponent.HQ) && !opponent.HQ->checkExplosion()){
			opponent.HQ->explode(i->getDamage());
		}
	
	}

	
	for(auto&& i: opponent.units){
		i->update(ticks);
		i->broadcast();
		opponent.HQ->broadcast();
		for(auto&& o : playerManager.units)
		{
			if(i->collidedWith(o) && !(o->checkExplosion())){
				o->explode(i->getDamage());
			}

			if(i->collidedWith(o) && o->checkExplosion()){
				opponent.gainPoints(o->getValue());
				std::cout << "?\n";
			}
		}

		if(i->collidedWith(playerManager.HQ) && !playerManager.HQ->checkExplosion()){
			playerManager.HQ->explode(i->getDamage());
		}
	}

	auto it = opponent.units.begin();
	  while ( it != opponent.units.end() ) {
      Drawable* doa = *it;
		if(doa->checkExplosion()){
			pointHUD->update(1);
			//detach from all
			for(auto&& i : playerManager.units){
				i->detach(doa);
			}
			playerManager.HQ->detach(doa);
				playerManager.gainPoints(doa->getValue());
      	delete doa;
     		it = opponent.units.erase(it);
			sound[2];
    }	 else ++it;

  }
		if(ecurrentSprite > (int)opponent.units.size()){
			ecurrentSprite = 0;
		}

	auto itt = playerManager.units.begin();
	  while ( itt != playerManager.units.end() ) {
      Drawable* doa = *itt;
		if(doa->checkExplosion()){
			pointHUD->update(1);
			//detach from all
			for(auto&& i : opponent.units){
				i->detach(doa);
			}
			opponent.HQ->detach(doa);
				opponent.gainPoints(doa->getValue());
      	delete doa;
     		itt = playerManager.units.erase(itt);
			sound[2];
    }	 else ++itt;
  }

		if(currentSprite > (int) playerManager.units.size()){
			currentSprite = 0;
		}


  	background.update();
	foreground.update();

	mainHUD->update();

	p1Pointer->update(ticks);
	p2Pointer->update(ticks);
  	viewport.update(); // always update viewport last


}
void Engine::P1switchSprite(){
  ++currentSprite;
  currentSprite = currentSprite % playerManager.units.size();

}

void Engine::P2switchSprite(){
  ++ecurrentSprite;
  ecurrentSprite = ecurrentSprite % opponent.units.size();

}







bool Engine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;

  while ( !done ) {
    // The next loop polls for events, guarding against key bounce:
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_F4]) {
          done = true;
          clock.unpause();
			 return true;
        }
        if (keystate[SDL_SCANCODE_ESCAPE]) {
          done = true;
          break;
        }

        if ( keystate[SDL_SCANCODE_F1] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
//player 1
        if ( keystate[SDL_SCANCODE_Q] ) {
			if(playerManager.units.size())
				P1switchSprite();
        }
//player 2

        if ( keystate[SDL_SCANCODE_P] ) {
			if(opponent.units.size())
				P2switchSprite();
        }

//player 1
        if( keystate[SDL_SCANCODE_F] ) {
				playerManager.deploy();			
        }

        if ( keystate[SDL_SCANCODE_E] ) {
			playerManager.selectUnit();
        }

//player 2
        if ( keystate[SDL_SCANCODE_I] ) {
			opponent.selectUnit();
        }

        if ( keystate[SDL_SCANCODE_J] ) {
			opponent.deploy();
        }

        if ( keystate[SDL_SCANCODE_SPACE] ) {
	//			static_cast<HQ_NPC*>(playerManager.HQ)->upTheAnte();
        }





		  if(keystate[SDL_SCANCODE_F1]){
			 	mainHUD->flipVisibility();
		  }
		  if(keystate[SDL_SCANCODE_F2]){
			 	bulletPoolHud->flipVisibility();
		  }
        if (keystate[SDL_SCANCODE_F5] && !makeVideo) {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        }
        else if (keystate[SDL_SCANCODE_F5] && makeVideo) {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }
      }
    }

    // In this section of the event loop we allow key bounce:

    ticks = clock.getElapsedTicks();
   if ( ticks > 0 ) {
      clock.incrFrame();
//player 1
		if(playerManager.units.size() > 0){

			if (keystate[SDL_SCANCODE_A] ) {
				playerManager.units[currentSprite]->goLeft();
		   }

		   if (keystate[SDL_SCANCODE_D]) {
				playerManager.units[currentSprite]->goRight();
		   }
		   if (keystate[SDL_SCANCODE_W]) {
				playerManager.units[currentSprite]->goUp();
		   }
		   if (keystate[SDL_SCANCODE_S]) {
				playerManager.units[currentSprite]->goDown();
		   }
			if	(keystate[SDL_SCANCODE_LSHIFT] ){
					playerManager.units[currentSprite]->shoot();
					sound[0];
			}

		}
//player 2
		if(opponent.units.size() > 0){
			if (keystate[SDL_SCANCODE_K] ) {
				opponent.units[ecurrentSprite]->goLeft();
		   }

		   if (keystate[SDL_SCANCODE_SEMICOLON]) {
				opponent.units[ecurrentSprite]->goRight();
		   }
		   if (keystate[SDL_SCANCODE_O]) {
				opponent.units[ecurrentSprite]->goUp();
		   }
		   if (keystate[SDL_SCANCODE_L]) {
				opponent.units[ecurrentSprite]->goDown();
		   }
			if	(keystate[SDL_SCANCODE_RSHIFT] ){
					opponent.units[ecurrentSprite]->shoot();
					sound[1];
			}
		}

      draw();
      update(ticks);
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
    }
  }

	return false;
}




