#include "Prince.h"


//constructors
Prince::Prince() {

	hasSword = false;
	maxHealth = 6;
	currentHealth = 2;

	idleSpeedX = 0;
	runningSpeedX = 2;
	climbSpeedX = 0;

	princeSpeedX = &idleSpeedX;

	idle        = new Animation(Game::getSprite("idle")       , 1);
	turn        = new Animation(Game::getSprite("turn")       , 8);
	running     = new Animation(Game::getSprite("running")    , 21);
	climbUp     = new Animation(Game::getSprite("climbUp")    , 17);
	jumpGrab    = new Animation(Game::getSprite("jumpGrab")   , 16);
	crouch      = new Animation(Game::getSprite("crouch")     , 13);
	staticJump  = new Animation(Game::getSprite("staticJump") , 17);
	step        = new Animation(Game::getSprite("step")       , 12);
	hang        = new Animation(Game::getSprite("hang")       , 11);
	runningJump = new Animation(Game::getSprite("runningJump"), 11);
	runningTurn = new Animation(Game::getSprite("runningTurn"), 9);
	fall        = new Animation(Game::getSprite("fall")       , 5);
	drop        = new Animation(Game::getSprite("drop")       , 6);
	drink       = new Animation(Game::getSprite("drink")      , 15);
	pickSword   = new Animation(Game::getSprite("pickSword")  , 13);


	fightIdle   = new Animation(Game::getSprite("fightIdle")  , 1);
	fightStep   = new Animation(Game::getSprite("fightStep")  , 4);
	fightParry  = new Animation(Game::getSprite("fightParry") , 3);
	fightStrike = new Animation(Game::getSprite("fightStrike"), 6);
	fightStart  = new Animation(Game::getSprite("fightStart") , 4);
	fightFinish = new Animation(Game::getSprite("fightFinish"), 8);



	fightStep->setDisplayTime(100);
	fightParry->setDisplayTime(100);
	fightStrike->setDisplayTime(100);
	fightStart->setDisplayTime(100);
	fightFinish->setDisplayTime(100);




	fall->setDisplayTime(70);
	step->setDisplayTime(49);
	drop->setDisplayTime(49);
	turn->setDisplayTime(49);
	hang->setDisplayTime(100);
	drink->setDisplayTime(60);
	crouch->setDisplayTime(49);
	climbUp->setDisplayTime(49);
	running->setDisplayTime(49);
	jumpGrab->setDisplayTime(49);
	pickSword->setDisplayTime(100);
	staticJump->setDisplayTime(65);
	runningJump->setDisplayTime(49);
	runningTurn->setDisplayTime(49);
	
	
	runningJump->setReverse();
	runningTurn->setReverse();
	drink->setReverse();


	fightStep->setReverse();
	fightParry->setReverse();
	fightStrike->setReverse();
	fightStart->setReverse();
	fightFinish->setReverse();


	facingRight = false;
	fightStance = false;


	setCurrentAnim(idle);

}

//getters
int Prince::getHealth() {
	return currentHealth;
}
int Prince::getMaxHealth() {
	return maxHealth;
}

//function
void Prince::Animate(Graphics* graphics) {
	int moveX = 0;
	int moveY = 0;

	if(this->getAnim()->isFinished()) {
		//side effects at the end of an animation
		if(this->getAnim() == climbUp) { 
			if(climbUp->isReversed()){		
				climbUp->setForward();
				//moveY += BLOCK_HEIGHT_PX; 
				this->MoveY(Level::BLOCK_HEIGHT_PX);
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
					moveX += 4;	//this->MoveX(runningSpeedX);
				} else {
					moveX -= 4;	//this->MoveX(-runningSpeedX);
				}
			} else if(getAnim()->getCurrentFrame() < 19){
				if(facingRight) {
					moveX += 3;	//this->MoveX(4);
				} else {
					moveX -= 3;	//this->MoveX(-4);
				}
			}
		}

		if(this->getAnim() == runningTurn) {
			if(facingRight) {
				moveX -= 3;	//MoveX(-3);
			} else {
				moveX += 3;	//MoveX(3);
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
					moveX += 8;	//this->MoveX(13);
				} else {
					moveX -= 8;	//this->MoveX(-13);
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
			//moveY -= BLOCK_HEIGHT_PX;  
			this->MoveY(-Level::BLOCK_HEIGHT_PX);
		}

		if(this->getAnim() == hang) {
			//moveY -= BLOCK_HEIGHT_PX;	
			this->MoveY(-Level::BLOCK_HEIGHT_PX);
		}
	}

	defferMoveX(moveX);
	defferMoveY(moveY);

	Entity::Animate(graphics);
}

void Prince::HandlePrince(Input* input) {

	int moveX = 0;
	int moveY = 0;


	if (fightStance) {
		OutputDebugString(L"FIGHT!\n");


	


		if (this->getAnim()->isFinished()) {
			//input at the end of an animation
		} else {
			//input during animation
		}



		defaultToIdle();

		if (this->getAnim() == fightIdle) {

			if (input->hasBeenPressed('Z')) { this->setCurrentAnim(fightIdle);   }
			if (input->hasBeenPressed('X')) { this->setCurrentAnim(fightStep);   }
			if (input->hasBeenPressed('C')) { this->setCurrentAnim(fightParry);  }
			if (input->hasBeenPressed('V')) { this->setCurrentAnim(fightStrike); }
			if (input->hasBeenPressed('B')) { this->setCurrentAnim(fightStart);  }
			if (input->hasBeenPressed('N')) { this->setCurrentAnim(fightFinish); }

			
			if (input->hasBeenPressed('K')) {
				fightStance = false;
			}
		}

		

		return;
	}

	OutputDebugString(L"PASSIVE!\n");

	if(this->getAnim()->isFinished()) {

		//input at the end of an animation
		if(this->getAnim() == hang) {
			if(input->isShiftPressed()) { this->getAnim()->Play(); }
		}
	
		if(this->getAnim() == jumpGrab) { 
			if(input->isShiftPressed()){ this->setCurrentAnim(hang); }

			if(input->isUpPressed()) { this->setCurrentAnim(climbUp); }
		}

		if(this->getAnim() == runningTurn) {
			this->setCurrentAnim(running);
			this->getAnim()->setCurrentFrame(5);
		}

		if(this->getAnim() == runningJump) {
			this->setCurrentAnim(running);
			this->getAnim()->setCurrentFrame(12);
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

			if(input->isUpPressed() && 
				getAnim()->getCurrentFrame() > 10 &&
				getAnim()->getCurrentFrame() < 14) {
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

		if (input->hasBeenPressed('K')) { 
			fightStance = true; 
			setCurrentAnim(fightIdle);  
		}


	

	}

}
int Prince::Drink() {
	if(this->getAnim() == idle) {
		this->setCurrentAnim(drink);
	
		return 1;
	}
	return 0;
}
void Prince::Hurt() {
	currentHealth--;
}
void Prince::Heal() {
	if(currentHealth < maxHealth) {
		currentHealth++;
	}
}
void Prince::increaseMaxHealth() {
	maxHealth++;
	currentHealth = maxHealth;
}
int Prince::setFall() {
	//sanity checks
	//assert(currentAnim == running || currentAnim == runningJump || currentAnim == staticJump || currentAnim == fall);
	if(this->getAnim() == runningJump) {
		if(this->getAnim()->getCurrentFrame() < 6) {
			return 0;
		}
	}

	if(this->getAnim() == staticJump) {
		if(this->getAnim()->getCurrentFrame() < 11) {
			return 0;
		}
	}

	if( this->getAnim() != jumpGrab && this->getAnim() != hang) {
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
int Prince::PickUpSword() {
	if (this->getAnim() == idle) {
		this->setCurrentAnim(pickSword);
		return 1;
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
	if (fightStance) {
		if (this->getAnim()->isFinished()) {
			setCurrentAnim(fightIdle);
		}
	} else {
		if(this->getAnim()->isFinished()) { 
			setCurrentAnim(idle); 
		}
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
		drink->setFlipped(false);
		pickSword->setFlipped(false);

		facingRight = false;

		fightIdle->setFlipped(false);
		fightStep->setFlipped(false);
		fightParry->setFlipped(false);
		fightStrike->setFlipped(false);
		fightStart->setFlipped(false);
		fightFinish->setFlipped(false);
		
	
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
		drink->setFlipped(true);
		pickSword->setFlipped(true);
				
		facingRight = true;

		fightIdle->setFlipped(true);
		fightStep->setFlipped(true);
		fightParry->setFlipped(true);
		fightStrike->setFlipped(true);
		fightStart->setFlipped(true);
		fightFinish->setFlipped(true);

	}
}


/*
idle		= new Animation(L"Assets//prince//idle.png"       ,	1	);
turn		= new Animation(L"Assets//prince//turn.png"       , 8	);
running		= new Animation(L"Assets//prince//running.png"    , 21	);
climbUp		= new Animation(L"Assets//prince//climbUp.png"    , 17	);
jumpGrab	= new Animation(L"Assets//prince//jumpGrab.png"   , 16	);
crouch		= new Animation(L"Assets//prince//crouch.png"     , 13	);
staticJump	= new Animation(L"Assets//prince//staticJump.png" ,	17	);
step		= new Animation(L"Assets//prince//step.png"       ,	12	);
hang		= new Animation(L"Assets//prince//hang.png"       , 11	);
runningJump = new Animation(L"Assets//prince//runningJump.png", 11	);
runningTurn = new Animation(L"Assets//prince//runningTurn.png", 9	);
fall        = new Animation(L"Assets//prince//fall.png"       , 5	);
drop        = new Animation(L"Assets//prince//drop.png"       , 6	);
drink       = new Animation(L"Assets//prince//drink.png"      , 15	);
*/
