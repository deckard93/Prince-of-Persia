#include "Guard.h"
#include "Game.h"

Guard::Guard(int x, int y) {
	setX(x);
	setY(y);

	type = guardT;

	//Sprite* sprite = Game::getSprite("guardIdle");
	fightIdle   = new Animation(Game::getSprite("fightIdle"),   1);
	fightStep   = new Animation(Game::getSprite("fightStep"),   4);
	fightParry  = new Animation(Game::getSprite("fightParry"),  3);
	fightStrike = new Animation(Game::getSprite("fightStrike"), 6);
	fightInjure = new Animation(Game::getSprite("fightInjure"), 3);

	fightStep->setDisplayTime(100);
	fightParry->setDisplayTime(120);
	fightStrike->setDisplayTime(100);

	fightStep->setReverse();
	fightParry->setReverse();
	fightStrike->setReverse();

	facingRight = false;

	Entity::setCurrentAnim(fightIdle);
}



void Guard::ActionHandler(Action action) {
	if (this->getAnim()->isFinished()) {
		//input at the end of an animation
	}
	else {
		//input during animation
	}

	if (this->getAnim() != fightIdle) { return; }

	//accept input
	if (action == aGoRight && facingRight) {
		fightStep->setReverse();
		this->setCurrentAnim(fightStep);
	}
	if (action == aGoRight && !facingRight) {
		fightStep->setForward();
		this->setCurrentAnim(fightStep);
	}

	if (action == aGoLeft && facingRight) {
		fightStep->setForward();
		this->setCurrentAnim(fightStep);
	}
	if (action == aGoLeft && !facingRight) {
		fightStep->setReverse();
		this->setCurrentAnim(fightStep);
	}

	if (action == aStrike)    { this->setCurrentAnim(fightStrike); }
	if (action == aParry)     { this->setCurrentAnim(fightParry);  }
	if (action == aEngage)    { this->setCurrentAnim(fightStart);  }
	if (action == aDisengage) { this->setCurrentAnim(fightFinish); }
}

void Guard::defaultToIdle() {
	/* Must be called after checking for
	animation side effects upon finishing
	*/
	if (this->getAnim()->isFinished()) {
		setCurrentAnim(fightIdle);
	}
}

void Guard::Animate(Graphics* graphics) {
	//if (state == sDead) { return; }

	int moveX = 0;
	int moveY = 0;

	if (this->getAnim()->isFinished()) {
		//side effects at the end of an animation

		//set idle state
		defaultToIdle();
	} 
	else if (this->getAnim()->getCurrentFrame() != 0) {
		//side effects during animation

		/*
		if (this->getAnim() == drop) {
			if (getAnim()->getCurrentFrame() >= 3 && getAnim()->getCurrentFrame() <= 4 && getAnim()->isEffectPending()) {
				moveY += 12;	//this->MoveX(13);
				getAnim()->setEffectDone();
			}
		}
		*/

		//fighting side effects
		if (this->getAnim() == fightStep) {

			//fighting side effects
			if (this->getAnim() == fightStep) {

				if (this->getAnim()->isReversed()) {
					if (this->getAnim()->isFlipped()) {
						moveX += 2;
					}
					else {
						moveX -= 2;
					}
				}
				else {
					if (this->getAnim()->isFlipped()) {
						moveX -= 2;
					}
					else {
						moveX += 2;
					}
				}
			}
		}
	}
	else {
		//side effects at the beggining of an animation
	}

	defferMoveX(moveX);
	defferMoveY(moveY);

	Entity::Animate(graphics);
}
