#include "Prince.h"




//constructors
Prince::Prince() {

	maxHealth = 6;
	currentHealth = 2;

	idleSpeedX = 0;
	runningSpeedX = 2;
	climbSpeedX = 0;

	princeSpeedX = &idleSpeedX;

	idle		= new Animation(L"Assets//prince//base.png",		1	);
	turn		= new Animation(L"Assets//prince//turn.png",		8	);
	running		= new Animation(L"Assets//prince//running.png",		21	);
	climbUp		= new Animation(L"Assets//prince//climbUp.png",		17	);
	jumpGrab	= new Animation(L"Assets//prince//jumpGrab.png",	16	);
	crouch		= new Animation(L"Assets//prince//crouch.png",		13	);
	staticJump	= new Animation(L"Assets//prince//staticJump.png",	17	);
	step		= new Animation(L"Assets//prince//step.png",		12	);
	hang		= new Animation(L"Assets//prince//hang.png"       , 11	);
	runningJump = new Animation(L"Assets//prince//runningJump.png", 11	);
	runningTurn = new Animation(L"Assets//prince//runningTurn.png", 9	);
	fall        = new Animation(L"Assets//prince//fall.png"       , 5	);

	fall->setDisplayTime(70);
	step->setDisplayTime(49);
	turn->setDisplayTime(49);
	hang->setDisplayTime(100);
	crouch->setDisplayTime(49);
	climbUp->setDisplayTime(49);
	running->setDisplayTime(49);
	jumpGrab->setDisplayTime(49);
	staticJump->setDisplayTime(65);
	runningJump->setDisplayTime(49);
	runningTurn->setDisplayTime(49);
	
	runningJump->setReverse();
	runningTurn->setReverse();

	setCurrentAnim(idle);

	facingRight = false;

}


//getters
int Prince::getHealth() {
	return currentHealth;
}
int Prince::getMaxHealth() {
	return maxHealth;
}

//setters

//function
void Prince::Animate(Graphics* graphics) {
	int moveX = 0;
	int moveY = 0;

	

	if(this->getAnim()->isFinished()) {

		//side effects at the end of an animation
		if(this->getAnim() == climbUp) { 
			if(climbUp->isReversed()){		
				climbUp->setForward();
				//moveY += LEVEL_HEIGHT_PIX; 
				this->MoveY(LEVEL_HEIGHT_PIX);
			}
		}

		if(this->getAnim() == runningJump) {
			this->setCurrentAnim(running);
			getAnim()->setCurrentFrame(12);
			this->getAnim()->Play();
		}

	} else if(this->getAnim()->getCurrentFrame() != 0) {

		//side effects during animation
		if(this->getAnim() == running) {
			if(getAnim()->getCurrentFrame() > 5 && getAnim()->getCurrentFrame() < 13) {
				if(facingRight) {
					moveX += runningSpeedX;	//this->MoveX(runningSpeedX);
				} else {
					moveX -= runningSpeedX;	//this->MoveX(-runningSpeedX);
				}
			} else if(getAnim()->getCurrentFrame() < 19){
				if(facingRight) {
					moveX += 2;	//this->MoveX(4);
				} else {
					moveX -= 2;	//this->MoveX(-4);
				}
			}
		}

		if(this->getAnim() == runningTurn) {
			if(facingRight) {
				moveX -= 3;	//MoveX(-3);
			} else {
				moveY += 3;	//MoveX(3);
			}
		}

		if(this->getAnim() == staticJump) {
			if(getAnim()->getCurrentFrame() > 5 && getAnim()->getCurrentFrame() < 13) {
				if(facingRight) {	
					moveX += 5;	//this->MoveX(13);
				} else {
					moveX -= 5;	//this->MoveX(-13);
				}
			}
		}

		if(this->getAnim() == runningJump) {
			if(getAnim()->getCurrentFrame() > 0 && getAnim()->getCurrentFrame() < 12) {
				if(facingRight) {	
					moveX += 5;	//this->MoveX(13);
				} else {
					moveX -= 5;	//this->MoveX(-13);
				}
			}
		}

		if(this->getAnim() == step) {
			if(getAnim()->getCurrentFrame() > 6 && getAnim()->getCurrentFrame() < 12) {
				if(facingRight) {
					//this->MoveX(4);
					moveX += 2;
				} else {
					//this->MoveX(-4);
					moveX -= 2;
				}
			}
		}

	} else {
		if(this->getAnim() == jumpGrab) {
			//moveY -= LEVEL_HEIGHT_PIX;  
			this->MoveY(-LEVEL_HEIGHT_PIX);
		}

		if(this->getAnim() == hang) {
			//moveY -= LEVEL_HEIGHT_PIX;	
			this->MoveY(-LEVEL_HEIGHT_PIX);
		}


	}

	defferMoveX(moveX);
	defferMoveY(moveY);

	Entity::Animate(graphics);
}

void Prince::HandlePrince(Input* input) {

	int moveX = 0;
	int moveY = 0;

	if(this->getAnim()->isFinished()) {
		//input at the end of an animation
		if(this->getAnim() == hang) {
			if(input->isShiftPressed()) { this->getAnim()->Play(); }
		}
	
		if(this->getAnim() == jumpGrab) { 
			if(input->isShiftPressed()){ this->setCurrentAnim(hang); }

			if(input->isUpPressed()) { this->setCurrentAnim(climbUp); }
		}
	
	} else {

		//input during animation
		if(this->getAnim() == crouch && this->getAnim()->getCurrentFrame() == 3) { 
			if(input->isDownPressed()){
				this->getAnim()->setCurrentFrame(2);
			}
		}

		if(this->getAnim() == running) {
			if(getAnim()->getCurrentFrame() == 13) {
				if(
					(input->isRightPressed() && facingRight) || 
					(input->isLeftPressed() && !facingRight)
					) 
				{
					getAnim()->setCurrentFrame(5);
				}

				if(
					(input->isRightPressed() && !facingRight) ||
					(input->isLeftPressed() && facingRight)
					) 
				{
					this->setCurrentAnim(runningTurn);
					switchFacing();
				}
			}

			if((input->isUpPressed())) {
				this->setCurrentAnim(runningJump);
			}
		}

		if(this->getAnim() == hang) {
			if((!input->isShiftPressed() || hang->isFinished())) {
				this->getAnim()->Stop();
			} else if(input->isUpPressed()) {
				this->setCurrentAnim(climbUp);
			}
		}
	}

	//set idle state
	defaultToIdle();

	//accept input
	if(this->getAnim() == idle) {

		if(input->isShiftPressed() && input->isLeftPressed() && !facingRight) { this->setCurrentAnim(step);	return;}

		if(input->isShiftPressed() && input->isRightPressed() && facingRight) { this->setCurrentAnim(step); return;}

		if(input->isUpPressed() && input->isLeftPressed() && !facingRight) { this->setCurrentAnim(staticJump); return; }

		if(input->isUpPressed() && input->isRightPressed() && facingRight) { this->setCurrentAnim(staticJump); return; }

		if(input->isUpPressed()) { this->setCurrentAnim(jumpGrab); }

		if(input->isDownPressed()) { this->setCurrentAnim(crouch); }

		if(input->isDownPressed() && input->isShiftPressed()) {
			this->setCurrentAnim(climbUp);
			climbUp->setReverse();

		}

		if(input->isRightPressed()) {
			if(facingRight) {
				this->setCurrentAnim(running);
			} else {
				this->setCurrentAnim(turn);
				switchFacing();	
			}
		}

		if(input->isLeftPressed()) {
			if(facingRight) {
				this->setCurrentAnim(turn);
				switchFacing();
			} else {
				this->setCurrentAnim(running);
			}
		}

	}

}
void Prince::Hurt() {
	currentHealth--;
}
void Prince::Heal() {
	currentHealth++;
}
void Prince::increaseMaxHealth() {
	maxHealth++;
	currentHealth = maxHealth;
}
int Prince::setFall() {
	//sanity checks
	//assert(currentAnim == running || currentAnim == runningJump || currentAnim == staticJump || currentAnim == fall);
	if( this->getAnim() != jumpGrab) {
		if(this->getAnim() != fall) {
			this->setCurrentAnim(fall);
			this->getAnim()->Play();
		} else if(this->getAnim()->getCurrentFrame() == 4) {
			this->getAnim()->Freeze();
		}
		return 3;
	}
	return 0;
}

//destructors
Prince::~Prince() {}


//privates
void Prince::defaultToIdle() {
	/* Must be called after checking for
	animation side effects upon finishing
	*/

	if(this->getAnim()->isFinished()) { 
		setCurrentAnim(idle); 
	}
}
void Prince::switchFacing() {

	if(facingRight) {
		idle->setFlipped(false);
		turn->setFlipped(true);
		running->setFlipped(false);
		climbUp->setFlipped(false);
		crouch->setFlipped(false);
		jumpGrab->setFlipped(false);
		staticJump->setFlipped(false);
		step->setFlipped(false);
		hang->setFlipped(false);
		runningJump->setFlipped(false);
		fall->setFlipped(false);
		runningTurn->setFlipped(true);

		facingRight = false;
	
	} else {

		idle->setFlipped(true);
		turn->setFlipped(false);
		running->setFlipped(true);
		climbUp->setFlipped(true);
		crouch->setFlipped(true);
		jumpGrab->setFlipped(true);
		staticJump->setFlipped(true);
		step->setFlipped(true);
		hang->setFlipped(true);
		fall->setFlipped(true);
		runningJump->setFlipped(true);
		runningTurn->setFlipped(false);
				
		facingRight = true;

	}
}