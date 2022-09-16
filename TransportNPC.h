#include "NPC.h"

#ifndef TRANSPORTNPC__H
#define TRANSPORTNPC__H
class T_NPC : public NPC{
	public:
		T_NPC( const std::string name, Vector2f);
		T_NPC( const T_NPC& npc);
		virtual void draw() const;
		virtual void update(Uint32 ticks);
		~T_NPC(){ for(auto&& i : footSoldiers) delete i; }
		T_NPC& operator=(const T_NPC& );

		bool canDeploy();
		virtual bool collidedWith(const Drawable* obj) const{
			if(obj->getVisibility())
				return false;
			else return false;
		}
	protected:
		bool inRange(Vector2f point, float range);
	//	void deploy();
	//	void addRifleman();
	//	void AddRocketman();
		std::vector<Drawable*> footSoldiers;
		Vector2f SpawnPoint;
		float acceleration;
		bool dflag;
		float timeSinceLastDeployment;
		float deploymentInterval;
		int troopCapacity;
		int spawnedSoFar;
	
};

#endif
