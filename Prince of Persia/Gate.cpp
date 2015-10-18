#include "Gate.h"
#include "Game.h"


Gate::Gate(int x, int y, int levelX, int levelY) {

	type = gateT;

	setX(x);
	setY(y);

	setLevelX(levelX);
	setLevelY(levelY);

	gate = new Animation(Game::getSprite("gate"), 7);
	setCurrentAnim(gate);
	gate->Stop();

	gate->setReverse();
	gate->setDisplayTime(60);
	gate->setCurrentFrame(6);

	//gate->Play();
	
	timer = new Timer();
}

void Gate::Open() {
	if(this->getAnim() == gate) {
		if(getAnim()->getCurrentFrame() != 0) {
			timer->StopWatch();
			timer->StartWatch();
			int fr = this->getAnim()->getCurrentFrame();
			this->getAnim()->setReverse();
			this->getAnim()->setCurrentFrame(fr);
			this->getAnim()->setDisplayTime(30);
			this->getAnim()->Play();
		}
	}
}

void Gate::Close() {}

void Gate::Animate(Graphics* graphics) {
	//at the end of animation
	
	
	if(this->getAnim() == gate) {
		if(this->getAnim()->getCurrentFrame() == 0) {
			if(!this->getAnim()->isFrozen()) {
				this->getAnim()->Freeze();
			} else {
				if(timer->GetTimeMilli() > 1000) {
					timer->StopWatch();
					this->getAnim()->setForward();
					this->getAnim()->setDisplayTime(1000);
					this->getAnim()->Play();
				}
			}
		}
		if(this->getAnim()->isFinished()) {
			this->getAnim()->setCurrentFrame(6);
		}
	}	
	
	Entity::Animate(graphics);
}