#include "Entity.h"
#include "Game.h"
#include "Guilotine.h"


Guilotine::Guilotine(int x, int y) {
	xPos = x;
	yPos = y;

	type = guilotineT;

	currentAnim = new Animation(Game::getSprite("guilotine"), 5);

	currentAnim->setCurrentFrame(rand() % 5);
	currentAnim->setDisplayTime(90);
	currentAnim->setLoop(true);
	currentAnim->Play();
}