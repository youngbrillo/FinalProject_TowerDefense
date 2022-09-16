#include <vector>
#include <SDL.h>
#include "ioMod.h"
#include "renderContext.h"
#include "world.h"
#include "viewport.h"
#include "player.h"
#include "hud.h"
#include "collisionStrategy.h"
#include "playerManager.h"
#include "sound.h"
class Engine {
public:
  Engine ();
  ~Engine ();
  bool play();
  void P1switchSprite();
  void P2switchSprite();
private:
	Engine(const Engine&) = delete;
  	Engine& operator=(const Engine&) = delete;

  	const RenderContext& rc;
  	const IoMod& io;

  	Clock& clock;
  	SDL_Renderer * const renderer;
  	World background;
	World foreground;
  	Viewport& viewport;
  	int currentSprite;
	int ecurrentSprite;
	bool makeVideo;
  	void draw() const;
  	void update(Uint32);
 	void printScales() const;
  	void checkForCollisions();
	HUD* mainHUD;		
	CollisionStrategy *collisionStrategy;
	HUD *bulletPoolHud;
	HUD *pointHUD;
	PlayerManager playerManager;
	PlayerManager opponent;
	Drawable* p1Pointer;
	Drawable* p2Pointer;
	
	SDLSound sound;
};
