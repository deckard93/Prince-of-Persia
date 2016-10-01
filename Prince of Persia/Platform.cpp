#include "Entity.h"
#include "Animation.h"
#include "Game.h"
#include "Platform.h"


Platform::Platform(int x, int y) {
	type = platformT;

	xPos = x;
	yPos = y;

	state = lodged;

	platformMove = new Animation(Game::getSprite("platformMove"), 4);
	//platformMove->setDisplayTime(80);
	platformMove->setDisplayTime(animDisplayTime);
	platformMove->setForward();
	//platformMove->Play();
	//platformMove->setLoop(true);
	currentAnim = platformMove;

}

void Platform::Delete() { //TODO DEPRECATE
	state = broken;
}

void Platform::Move() {
	this->setCurrentAnim(platformMove);
}

void Platform::Drop() {
	if (state == lodged) {
		platformMove->setCurrentFrame(0);
		this->setCurrentAnim(platformMove);
		state = dislodging;
	}
}

void Platform::Animate(Graphics* graphics) {
	if (state == broken) { return;  }
	if (state == dislodging) {
		if (platformMove->getCurrentFrame() == platformMove->getLastFrameNr()) {
			platformMove->Freeze();
			state = dislodged;
		}
	}
	Entity::Animate(graphics);
}

platformState Platform::getState() {
	return state;
}