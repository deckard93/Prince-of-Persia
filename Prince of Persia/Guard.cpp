#include "Game.h"
#include "Guard.h"

Guard::Guard(Audio* audio, int x, int y) {

	this->audio = audio;

	setX(x);
	setY(y);

	type = guardT;

	//Sprite* sprite = Game::getSprite("guardIdle");
	fightIdle    = new Animation(Game::getSprite("guardFightIdle")   , 1);
	fightStep    = new Animation(Game::getSprite("guardFightStep")   , 4);
	fightParry   = new Animation(Game::getSprite("guardFightParry")  , 6);
	fightParried = new Animation(Game::getSprite("guardFightParried"), 1);
	fightStrike  = new Animation(Game::getSprite("guardFightStrike") , 8);
	fightInjure  = new Animation(Game::getSprite("guardFightInjure") , 3);
	fightDying   = new Animation(Game::getSprite("guardFightDying")  , 5);
	swordDeath   = new Animation(Game::getSprite("guardSwordDeath")  , 1);

	fightStep->setDisplayTime(100);
	fightParry->setDisplayTime(120);
	fightStrike->setDisplayTime(100);
	fightInjure->setDisplayTime(100);
	fightParried->setDisplayTime(150);
	fightDying->setDisplayTime(80);

	//fightStep->setReverse();
	//fightStrike->setReverse();
	fightParry->setReverse();

	currentHealth = 3;
	maxHealth = 3;

	hasSword = true;
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

	if (action == aParry) {
		int i = 0;
		i++;
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
	if (dead) { Entity::Animate(graphics); return; }

	int moveX = 0;
	int moveY = 0;

	if (this->getAnim()->isFinished()) {
		//side effects at the end of an animation
		if (this->getAnim() == fightParried) {
			setCurrentAnim(fightStrike);
			getAnim()->setCurrentFrame(4);
		}
		if (this->getAnim() == fightDying) {
			setCurrentAnim(swordDeath);
			getAnim()->Freeze();
			dead = true;
		}
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
		if (this->getAnim() == fightInjure) {
			if (this->getAnim()->isFlipped()) {
				moveX -= 2;
			}
			else {
				moveX += 2;
			}
		}

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
	else {
		//side effects at the beggining of an animation
	}

	setDefferX(moveX);
	setDefferY(moveY);

	Entity::Animate(graphics);
}
