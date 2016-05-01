#include "Prince.h"
#include "NormalInput.h"

//constructors
Prince::Prince() {
	state = sIdle;

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
	climbUp     = new Animation(Game::getSprite("newClimbUp") , 17);
	jumpGrab    = new Animation(Game::getSprite("newJumpGrab"), 15);
	crouch      = new Animation(Game::getSprite("crouch")     , 13);
	staticJump  = new Animation(Game::getSprite("staticJump") , 17);
	step        = new Animation(Game::getSprite("step")       , 12);
	hang        = new Animation(Game::getSprite("newHang")    , 11);
	runningJump = new Animation(Game::getSprite("runningJump"), 11);
	runningTurn = new Animation(Game::getSprite("runningTurn"), 9);
	fall        = new Animation(Game::getSprite("fall")       , 5);
	drop        = new Animation(Game::getSprite("newDrop")    , 5);
	drink       = new Animation(Game::getSprite("drink")      , 15);
	pickSword   = new Animation(Game::getSprite("pickSword")  , 13);


	fightIdle   = new Animation(Game::getSprite("fightIdle")  , 1);
	fightStep   = new Animation(Game::getSprite("fightStep")  , 4);
	fightParry  = new Animation(Game::getSprite("fightParry") , 3);
	fightStrike = new Animation(Game::getSprite("fightStrike"), 6);
	fightStart  = new Animation(Game::getSprite("fightStart") , 4);
	fightFinish = new Animation(Game::getSprite("fightFinish"), 8);
	fightInjure = new Animation(Game::getSprite("fightInjure"), 3);
	fightDying  = new Animation(Game::getSprite("fightDying") , 5);
	swordDeath  = new Animation(Game::getSprite("swordDeath") , 1);
	fightParried = NULL;

	spikeDeath = new Animation(Game::getSprite("spikeKill"),   1);

	fightStep->setDisplayTime(100);
	fightParry->setDisplayTime(80); //120
	fightStrike->setDisplayTime(100);
	fightStart->setDisplayTime(100);
	fightFinish->setDisplayTime(100);
	fightInjure->setDisplayTime(100);
	fightDying->setDisplayTime(100);


	fall->setDisplayTime(70);
	step->setDisplayTime(49);
	drop->setDisplayTime(70);
	turn->setDisplayTime(49);
	hang->setDisplayTime(100);
	drink->setDisplayTime(60);
	crouch->setDisplayTime(49);
	climbUp->setDisplayTime(49);
	//climbUp->setDisplayTime(2000);
	running->setDisplayTime(49);
	jumpGrab->setDisplayTime(65);
	pickSword->setDisplayTime(100);
	staticJump->setDisplayTime(65);
	runningJump->setDisplayTime(49);
	runningTurn->setDisplayTime(49);
	//fallDamage->setDisplayTime(49);
	

	climbUp->setReverse();
	runningJump->setReverse();
	runningTurn->setReverse();
	jumpGrab->setReverse();
	drink->setReverse();
	drop->setReverse();
	//fightInjure->setReverse();
	


	fightStep->setReverse();
	fightParry->setReverse();
	//fightStrike->setReverse();
	fightStart->setReverse();
	fightStart->setCurrentFrame(fightStart->getLastFrameNr());
	fightFinish->setReverse();
	fightFinish->setCurrentFrame(fightFinish->getLastFrameNr());


	facingRight = false;
	inFight = false;


	setCurrentAnim(idle);

}

//function
void Prince::Animate(Graphics* graphics) {
	if (state == sFinish) { return; }

	int moveX = 0;
	int moveY = 0;

	if(this->getAnim()->isFinished()) {
		//side effects at the end of an animation
		if(this->getAnim() == climbUp) { 
			if(climbUp->isReversed()){		
				climbUp->setForward();
				//moveY += BLOCK_HEIGHT_PX; 
				//==TODO
				//this->MoveY(Level::BLOCK_HEIGHT_PX);
			}
		}
		if (this->getAnim() == fightDying) {
			this->setCurrentAnim(swordDeath);
			getAnim()->Freeze();
			dead = true;
			return;
		}

		if(this->getAnim() == runningJump) {
			this->setCurrentAnim(running);
			getAnim()->setCurrentFrame(12);
			this->getAnim()->Play();
		}

		if (this->getAnim() == jumpGrab) {
			this->setCurrentAnim(drop);
		}

		if (hang == this->getAnim()) {
			this->setCurrentAnim(drop);
		}

		//set idle state
		defaultToIdle();

		
	} else if(this->getAnim()->getCurrentFrame() != 0) {
		//side effects during animation
		if(this->getAnim() == running) {
	
			if(getAnim()->getCurrentFrame() > 5 && getAnim()->getCurrentFrame() < 13) {
				if (facingRight) {
					moveX += 4;	
					//moveX += 10;
					//this->MoveX(runningSpeedX);
				} else {
					moveX -= 4;
					//moveX -= 10;
					//this->MoveX(-runningSpeedX);
				}
			} else if(getAnim()->getCurrentFrame() < 19){
				if(facingRight) {
					moveX += 3;	
					//moveX += 8;
					//this->MoveX(4);	
				} else {		
					moveX -= 3;	
					//moveX -= 8;
					//this->MoveX(-4);
				}
			}
		}
		if (this->getAnim() == climbUp && getAnim()->isEffectPending()) {
			int frame = getAnim()->getCurrentFrame();
			int changeY = 0;
			int changeX = 0;
			switch (16 - frame) {
				case 1:
				{
					changeX = 0;
					changeY = 18;
				} break;
				case 2:
				{
					changeX = 0;
					changeY = 10;
				} break;
				case 3:
				{
					changeX = 4;
					changeY = 11;
				} break;
				case 4:
				{
					changeX = 3;
					changeY = 10;
				} break;
				case 5:
				{
					changeX = -4;
					changeY = 13;
				} break;
				case 6:
				{
					changeX = 2;
					changeY = 13;
				} break;
				case 7:
				{
					changeX = 14;
					changeY = 18;
				} break;
				case 8:
				{
					changeX = 7;
					changeY = 13;
				} break;
				case 9:
				{
					changeX = 10;
					changeY = 0;
				} break;
				case 10:
				{
					changeX = 0;
					changeY = -5;
				} break;
				case 11:
				{
					changeX = 0;
					changeY = 0;
				} break;
				case 12:
				{
					changeX = 0;
					changeY = 0;
				} break;
				case 13:
				{
					changeX = 0;
					changeY = 0;
				} break;
				case 14:
				{
					changeX = 0;
					changeY = 0;
				} break;
				case 15:
				{
					changeX = 0;
					changeY = 0;
				} break;
				case 16:
				{
					changeX = 0;
					changeY = 0;
				} break;
			}

			if (!getAnim()->isReversed()) {
				moveY += changeY;
				if (facingRight) {
					moveX -= changeX;
				}
				else {
					moveX += changeX;
				}
			}
			else {
				moveY -= changeY;
				if (facingRight) {
					moveX += changeX;
				}
				else {
					moveX -= changeX;
				}
			}
			getAnim()->setEffectDone();
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
					moveX += 7;	//this->MoveX(13);
				} else {
					moveX -= 7;	//this->MoveX(-13);
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

		if (this->getAnim() == jumpGrab) {
			if (getAnim()->getCurrentFrame() > 0 && getAnim()->getCurrentFrame() < 4 && getAnim()->isEffectPending()) {
				moveY -= 8;	//this->MoveX(13);
				getAnim()->setEffectDone();
			}
		}

		if (this->getAnim() == drop) {
			if (getAnim()->getCurrentFrame() >= 3 && getAnim()->getCurrentFrame() <= 4 && getAnim()->isEffectPending()) {
				moveY += 12;	//this->MoveX(13);
				getAnim()->setEffectDone();
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

		//fighting side effects
		if (this->getAnim() == fightInjure) {
			if (this->getAnim()->isFlipped()) {
				moveX -= 2;
			} else {
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


	} else {
		//side effects at the beggining of an animation
		if(this->getAnim() == jumpGrab) {
			//moveY -= BLOCK_HEIGHT_PX;  
			//this->MoveY(-Level::BLOCK_HEIGHT_PX);
		}

		if(this->getAnim() == hang) {
			//moveY -= BLOCK_HEIGHT_PX;	
			//this->MoveY(-Level::BLOCK_HEIGHT_PX);
		}
	}

	defferMoveX(moveX);
	defferMoveY(moveY);

	Entity::Animate(graphics);
}


void Prince::ActionHandler(Action action)
{
	if (this->getAnim() != idle) { return; }

	switch (action) {
		case aLeftJump: {
			if (!facingRight) {	setCurrentAnim(staticJump); }
		} break;
		case aRightJump: {
			if (facingRight) { setCurrentAnim(staticJump);  }
		} break;
		case aLeftStep: {
			if (!facingRight) { setCurrentAnim(step); }
		} break;
		case aRightStep: {
			if (facingRight) { setCurrentAnim(step); }
		} break;
		case aClimbUp:	{

		} break;
		case aClimbDown: {
			this->setCurrentAnim(climbUp);
			climbUp->setForward();
			climbUp->Reset();
		} break;

		case aJumpGrab: {
			this->setCurrentAnim(jumpGrab);
		} break;
		case aHang:		{
		} break;

		case aGoRight: {
			if (facingRight) {
				setCurrentAnim(running);
			} else {
				setCurrentAnim(turn);
				switchFacing();
			}
		} break;

		case aGoLeft: {
			if (!facingRight) {	
				setCurrentAnim(running);
			} else {
				setCurrentAnim(turn);
				switchFacing();
			}
		} break;

		case aCrouch: {
			setCurrentAnim(crouch);
		} break;

		case aNone: { } break;


	}

}


void Prince::FightController(Input* input) {
	if (this->getAnim()->isFinished()) {
		//input at the end of an animation
		if (this->getAnim() == fightFinish) {
			inFight = false;
			this->MoveX(fightDisplacement);
		}
	}
	else {
		//input during animation
	}
	
	if (this->getAnim() != fightIdle) {	return;	}

	//accept input
	if (input->isRightPressed() && facingRight) {
		fightStep->setReverse();
		this->setCurrentAnim(fightStep);
	}
	if (input->isRightPressed() && !facingRight) {
		fightStep->setForward();
		this->setCurrentAnim(fightStep);
	}

	if (input->isLeftPressed() && facingRight) {
		fightStep->setForward();
		this->setCurrentAnim(fightStep);
	}
	if (input->isLeftPressed() && !facingRight) {
		fightStep->setReverse();
		this->setCurrentAnim(fightStep);
	}

	if (input->isCtrlPressed())      { this->setCurrentAnim(fightStrike); }
	if (input->hasUpBeenPressed())   { this->setCurrentAnim(fightParry);  }

	//Debugging
	if (input->hasDownBeenPressed()) { this->setCurrentAnim(fightFinish); }
	if (input->hasBeenPressed('B'))  { this->setCurrentAnim(fightStart);  }
	if (input->hasBeenPressed('T'))  { this->setCurrentAnim(fightInjure); }
}
void Prince::NormalController(Input* input) {

	NormalInput nInput(input);

	if (this->getAnim() == NULL) { return; }

	if (this->getAnim()->isFinished()) {

		//input at the end of an animation
		if (hang == this->getAnim()) {
			if (input->isShiftPressed()) { this->getAnim()->Play(); }
		}
		if (climbUp == this->getAnim() && !getAnim()->isReversed()) {
			this->setCurrentAnim(hang);
		}

		if (jumpGrab == this->getAnim()) {
			if (input->isShiftPressed()) { this->setCurrentAnim(hang); }
			if (input->isUpPressed()) {
				this->setCurrentAnim(climbUp);
				getAnim()->setReverse();
			}
		}

		if (this->getAnim() == runningTurn) {
			this->setCurrentAnim(running);
			this->getAnim()->setCurrentFrame(5);
		}

		if (this->getAnim() == runningJump) {
			this->setCurrentAnim(running);
			this->getAnim()->setCurrentFrame(12);
		}

	}
	else {
		//input during animation
		if (this->getAnim() == crouch && this->getAnim()->getCurrentFrame() == 3) {
			if (input->isDownPressed()) {
				this->getAnim()->setCurrentFrame(2);
			}
		}

		if (this->getAnim() == running) {
			if (getAnim()->getCurrentFrame() == 13) {
				if ((input->isRightPressed() && facingRight) ||
					(input->isLeftPressed() && !facingRight))
				{
					getAnim()->setCurrentFrame(5);
				}

				if ((input->isRightPressed() && !facingRight) ||
					(input->isLeftPressed() && facingRight))
				{
					this->setCurrentAnim(runningTurn);
					switchFacing();
				}
			}

			if (input->isUpPressed() &&
				getAnim()->getCurrentFrame() > 10 &&
				getAnim()->getCurrentFrame() < 14) {
				this->setCurrentAnim(runningJump);
			}
		}

		if (this->getAnim() == hang) {
			if ((!input->isShiftPressed() || hang->isFinished())) {
				this->getAnim()->Stop();
			}
			else if (input->isUpPressed()) {
				this->setCurrentAnim(climbUp);
				getAnim()->setReverse();
				climbUp->Reset();
			}
		}
	}

	
	if (this->getAnim() != idle) { return; }

	//accept input
	if (nInput.doLeftStep()  && !facingRight){ this->setCurrentAnim(step);	     return; }
	if (nInput.doRightStep() && facingRight) { this->setCurrentAnim(step);       return; }
	if (nInput.doLeftJump()  && !facingRight){ this->setCurrentAnim(staticJump); return; }
	if (nInput.doRightJump() && facingRight) { this->setCurrentAnim(staticJump); return; }

	if (input->isDownPressed() && input->isShiftPressed()) {
		this->setCurrentAnim(climbUp);
		climbUp->setForward();
		climbUp->Reset();
		return;
	}
	if (nInput.doClimb()) { this->setCurrentAnim(jumpGrab); return;  }
	if (nInput.goDown())  { this->setCurrentAnim(crouch);   return;  }
	

	if (input->isRightPressed()) {
		if (facingRight) {
			this->setCurrentAnim(running);
			return;
		} else {
			this->setCurrentAnim(turn);
			switchFacing();
			return;
		}
	}
	if (input->isLeftPressed()) {
		if (facingRight) {
			this->setCurrentAnim(turn);
			switchFacing();
			return;
		} else {
			this->setCurrentAnim(running);
			return;
		}
	}

	if (input->isCtrlPressed() && hasSword) {
		inFight = true;
		this->MoveX(-fightDisplacement);
		setCurrentAnim(fightStart);
	}
}

void Prince::Disengage() {
	setCurrentAnim(fightFinish);
}

void Prince::HandlePrince(Input* input) {
	if (state == sDead || state == sFinish) { return; }

	int moveX = 0;
	int moveY = 0;

	if (inFight) {		
		FightController(input);
	} else {
		NormalController(input);
	}
}

bool Prince::isStriking() {
	if (getAnim() == fightStrike) {
		return true;
	}
	return false;
}

int Prince::Drink() {
	if(this->getAnim() == idle) {
		this->setCurrentAnim(drink);
		return 1;
	}
	return 0;
}
void Prince::increaseMaxHealth() {
	maxHealth++;
	currentHealth = maxHealth;
}

int Prince::setFall(int currentBlockY) {
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

	if (this->getAnim() == drop) {
		return 0;
	}

	if (this->getAnim() == hang) {
		return 0;
	}

	if (this->getAnim() == jumpGrab) {
		return 0;
	}

	if (this->getAnim() == climbUp) {
		return 0;
	}


	if(this->getAnim() != fall) {
		this->setCurrentAnim(fall);
		this->lastBlockY = currentBlockY;
		this->getAnim()->Play();
	} else if(this->getAnim()->getCurrentFrame() == 4) {
		this->getAnim()->Freeze();
	}
	return 3;

}
int Prince::PickUpSword() {
	if (this->getAnim() == idle) {
		this->setCurrentAnim(pickSword);
		hasSword = true;
		return 1;
	}
	return 0;
}
void Prince::Land(int currentBlockY) {
	int fallHeight = currentBlockY - lastBlockY;

	if (getAnim() == fall && getAnim()->getCurrentFrame() == 4 && getAnim()->isFrozen()) { 
		switch (fallHeight) {
		case 0:
			getAnim()->Play();
			break;
		case 1:
			getAnim()->Play();
			break;
		case 2:
			this->setCurrentAnim(crouch);
			this->currentAnim->setCurrentFrame(2);
			this->currentAnim->setCurrentDisplayTime(3000.0);
			this->currentHealth--;
			break;
		default:
			this->setCurrentAnim(swordDeath);
			this->currentAnim->Freeze();
			this->setState(sDead);
			
		}
	}
}

void Prince::Catch() {
	this->setCurrentAnim(hang);
}

//destructors
Prince::~Prince() {}

//privates
void Prince::defaultToIdle() {
	/* Must be called after checking for
	animation side effects upon finishing
	*/
	if (inFight) {
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
		turn->setFlipped(true);
		runningTurn->setFlipped(true);

		drop->setFlipped(false);
		idle->setFlipped(false);
		step->setFlipped(false);
		hang->setFlipped(false);
		fall->setFlipped(false);
		drink->setFlipped(false);
		crouch->setFlipped(false);
		running->setFlipped(false);
		climbUp->setFlipped(false);
		jumpGrab->setFlipped(false);
		pickSword->setFlipped(false);
		staticJump->setFlipped(false);
		spikeDeath->setFlipped(false);
		swordDeath->setFlipped(false);
		runningJump->setFlipped(false);
		//fallDamage->setFlipped(false);

		facingRight = false;

		fightIdle->setFlipped(false);
		fightStep->setFlipped(false);
		fightParry->setFlipped(false);
		fightStart->setFlipped(false);
		fightStrike->setFlipped(false);
		fightFinish->setFlipped(false);
		fightInjure->setFlipped(false);
		fightDying->setFlipped(false);
		
	
	} else {
		turn->setFlipped(false);
		runningTurn->setFlipped(false);

		drop->setFlipped(true);
		idle->setFlipped(true);
		step->setFlipped(true);
		hang->setFlipped(true);
		fall->setFlipped(true);
		drink->setFlipped(true);
		crouch->setFlipped(true);
		running->setFlipped(true);
		climbUp->setFlipped(true);
		jumpGrab->setFlipped(true);
		pickSword->setFlipped(true);
		staticJump->setFlipped(true);
		spikeDeath->setFlipped(true);
		swordDeath->setFlipped(true);
		runningJump->setFlipped(true);
		//fallDamage->setFlipped(true);
		
		facingRight = true;

		fightIdle->setFlipped(true);
		fightStep->setFlipped(true);
		fightParry->setFlipped(true);
		fightStart->setFlipped(true);
		fightStrike->setFlipped(true);
		fightFinish->setFlipped(true);
		fightInjure->setFlipped(true);
		fightDying->setFlipped(true);

	}
}

princeState Prince::getPrinceState() {
	if (getAnim() == running) {
		return sRunning;
	}
	else if (getAnim() == fall) {
		return sFalling;
	}

	return sIdle;
}

void Prince::setState(princeState state) {
	this->state = state;
}

void Prince::spikeKill() {
	state = sDead;
	this->setCurrentAnim(spikeDeath);
	this->getAnim()->Freeze();
}

bool Prince::isJumping() {
	if (this->getAnim() == staticJump) {
		if (this->getAnim()->getCurrentFrame() >= 1 &&
			this->getAnim()->getCurrentFrame() <= 12) {
			return true;
		}
	}
	if (this->getAnim() == runningJump) {
		if (this->getAnim()->getCurrentFrame() <= 6) {
			return true;
		}
	}
	return false;
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
