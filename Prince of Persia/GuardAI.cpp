#include <stdlib.h>
#include <time.h> 

#include "Prince.h"
#include "Level.h"
#include "GuardAI.h"

GuardAI::GuardAI(int x, int y) : Guard(x, y) {
	chanceToParry = 100;
	chanceToStrike = 50;
}

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

	int princeLevelY = level.getCharLevelBlockY(&prince);
	int guardLevelY = level.getCharLevelBlockY(this);

	int princeLevelX = level.getSceneBlockYByCoord(prince.getX());
	int guardLevelX = level.getSceneBlockYByCoord(this->getX());

	srand(time(NULL));
	

	//FacePrince(prince, level);
	FaceCharacter(prince, level);
	if ( princeLevelY == guardLevelY ) {

//		OutputDebugStringA("We're on the same level \n");
		if (abs(princeLevelX - guardLevelX) == 0) {
			if (prince.isStriking() && prince.getAnim()->getCurrentFrame() == 0) {
				int result = rand() % (100);
				bool parry = false;
				if (result < chanceToParry) {
					parry = true;
				}
				if (parry) {
					ActionHandler(aParry);
				}
			}
			else {
				int result = rand() % (100);
				if (result < chanceToStrike) {
					ActionHandler(aStrike);
				}
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