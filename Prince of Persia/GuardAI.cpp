#include "Prince.h"
#include "Level.h"
#include "Guard.h"
#include "GuardAI.h"

GuardAI::GuardAI(int x, int y) : Guard(x, y) {}

void GuardAI::Control(Prince& prince, Level& level) {
	//if prince is not on the same level do nothing
	//prince on the same level
	//engaged
		//put down sword
	//not engaged
		//pull up sword
	//when enaged
	//far from prince
		//go to prince
	//close to prince
		//attack
	//about to be hit
		//chance to parry (depending on how close prince is)
	
	int princeLevelY = level.getSceneBlockYByCoord(prince.getY());
	int guardLevelY = level.getSceneBlockYByCoord(this->getY());

	int princeLevelX = level.getSceneBlockYByCoord(prince.getX());
	int guardLevelX = level.getSceneBlockYByCoord(this->getX());


	//FacePrince(prince, level);
	FaceCharacter(prince, level);
	if ( princeLevelY == guardLevelY ) {

//		OutputDebugStringA("We're on the same level \n");
		if (abs(princeLevelX - guardLevelX) == 0) {
			//ActionHandler(aStrike);
			if (prince.isStriking() && prince.getAnim()->getCurrentFrame() == 0) {
				ActionHandler(aParry);
			}
		}
		else {
			if (princeLevelX > guardLevelX) {
				ActionHandler(aGoRight);
			}
			else {
				ActionHandler(aGoLeft);		
			}
		}
	}

	return;
}