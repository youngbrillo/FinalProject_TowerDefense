#include "TransportNPC.h"


T_NPC::T_NPC(const std::string name, Vector2f sp):
	NPC(name),
	footSoldiers(), 
	SpawnPoint(sp),
	acceleration(Gamedata::getInstance().getXmlFloat(name+"/acceleration")),
	dflag(false),
	timeSinceLastDeployment(0),
	deploymentInterval(Gamedata::getInstance().getXmlFloat(name+"/DeploymentInterval")),
	troopCapacity(Gamedata::getInstance().getXmlInt(name+"/troopCapacity")),
	spawnedSoFar(0)
{

}
 
T_NPC::T_NPC(const T_NPC& npc) :
	NPC( npc),
	footSoldiers(npc.footSoldiers),

	SpawnPoint(npc.SpawnPoint),
	acceleration(npc.acceleration),	
	dflag(npc.dflag),
	timeSinceLastDeployment(npc.timeSinceLastDeployment),
	deploymentInterval(npc.deploymentInterval),
	troopCapacity(npc.troopCapacity),
	spawnedSoFar(npc.spawnedSoFar)
{}

T_NPC& T_NPC::operator=(const T_NPC& npc){
	footSoldiers = npc.footSoldiers;

	SpawnPoint = npc.SpawnPoint;
	dflag = npc.dflag; 
	acceleration = npc.acceleration; 
	timeSinceLastDeployment =(npc.timeSinceLastDeployment);
	deploymentInterval =(npc.deploymentInterval);
	troopCapacity = npc.troopCapacity;
	spawnedSoFar = npc.spawnedSoFar;
	return *this;
}
 

void T_NPC::draw() const{
	NPC::draw();

}

void T_NPC::update(Uint32 ticks){
	//avoiding enemies takes priority
	timeSinceLastDecision += ticks;
	timeSinceLastDeployment +=ticks;
	//at this point the broadcaster has probably given the NPc it's location
	float x = getX()+getImage()->getWidth()/2;
	float y = getY()+getImage()->getHeight()/2;
  	float distanceToSpawnPoint = distance( x, y, SpawnPoint[0], SpawnPoint[1] );
	if(	(distanceToSpawnPoint < 50) && (mode != DEPLOY)){	


		timeSinceLastDecision = 0;
		if(spawnedSoFar < troopCapacity)
			mode = DEPLOY;		
		else
			mode = NORMAL;
	}
	switch(mode){
		case(NORMAL):
			if(getDeployFlag())
				lowerDeployFlag(); 
			if(spawnedSoFar < troopCapacity)
				moveTo(SpawnPoint);
			break;	
		case(EVADE):
			if(getDeployFlag())
				lowerDeployFlag(); 
			break;
		case(FIGHT):
			break;	
		case(DEPLOY):
			if(!getDeployFlag())
				raisDeployFlag();

			setVelocityX(0.0);
			setVelocityY(0.0);
			break;
		case(CAPTURE):
			
			break;
		case(PLAYER):
		
			break;
	}

	NPC::update(ticks); 
} 

bool T_NPC::canDeploy(){
	if( timeSinceLastDeployment > deploymentInterval && (	spawnedSoFar < troopCapacity	)){
		timeSinceLastDeployment = 0;
		++spawnedSoFar;
		std::cout << "plop!\n";
		return true;
	}
	else
		return false;


}
bool T_NPC::inRange(Vector2f point, float range){
	//1st calculate distance of 2 points
	float dist = sqrt(pow((getX() -  point[0]	),2) + pow(	(getY() -  point[1]),2)	);
	if(dist <= range)
		return true;
	else 
		return false;

}

























/*
void T_NPC::deploy(){
	if(timeSinceLastDeployment > deploymentInterval){
		if(footSoldiers.size() < troopCapacity){
			addRifleman();
			timeSinceLastDeployment = 0;
		}
	}

}


void T_NPC::addRifleman(){
	footSoldiers.emplace_back(new NPC("rifleman"));
	std::cout << "rifleman added!!\n";
}
void T_NPC::AddRocketman(){
	footSoldiers.emplace_back(new NPC("rocketman"));
}

*/











