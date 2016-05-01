#include "Gate.h"
#include "Game.h"


Gate::Gate(int x, int y, int levelX, int levelY) {
	slowTime = 900;
	slowTime = 30;

	type = gateT;

	setX(x);
	setY(y);

	setLevelX(levelX);
	setLevelY(levelY);

	gate = new Animation(Game::getSprite("gate"), 7);
	setCurrentAnim(gate);
	gate->Stop();

	gate->setReverse();
	gate->setDisplayTime(slowTime);
	gate->setCurrentFrame(gate->getLastFrameNr());

	//gate->Play();
	
	state = sClosed;
	timer = new Timer();
}

void Gate::Open() {

	if (state == sClosed || state == sClosing) {
		state = sOpening;

		slowTime = 1000;

		timer->StopWatch();
		timer->StartWatch();

		this->getAnim()->setReverse();
		this->getAnim()->setDisplayTime(fastTime);
		this->getAnim()->Play();

	}
	else if (state == sOpened) {
		timer->StopWatch();
		timer->StartWatch();
	}
}

void Gate::Close() {
	if (state == sOpened || state == sClosing) {
		state = sClosing;

		this->getAnim()->setForward();
		this->getAnim()->setDisplayTime(fastTime);
		this->getAnim()->Play();
	}
}

void Gate::Animate(Graphics* graphics) {
	
	if (getAnim()->getCurrentFrame() == 0 && state == sOpening) {
		getAnim()->Stop();
		state = sOpened;
	}

	if (getAnim()->getCurrentFrame() == getAnim()->getLastFrameNr() && state == sClosing) {
		getAnim()->Stop();
		state = sClosed;
	}

	if (state == sOpened) {
		if (timer->GetTimeMilli() > slowTime) {
			state = sClosing;

			this->getAnim()->setForward();
			this->getAnim()->setDisplayTime(slowTime);
			this->getAnim()->Play();
		}
	}

	Entity::Animate(graphics);
}

bool Gate::isOpen() {
	if (state == sOpened) {
		return true;
	}
	else if (state == sClosing) {
		if (this->currentAnim->getCurrentFrame() <= 1) {
			return true;
		}
	}
	return false;
}