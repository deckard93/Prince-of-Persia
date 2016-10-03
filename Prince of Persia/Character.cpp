#include <list>
#include <map>

#include "Level.h"
#include "Entity.h"
#include "Animation.h"

#include "Character.h"


Character::Character() {
	inFight = false;
	dead = false;
	inScene = true;
}

void Character::Hurt() {
	
	if (currentHealth > 1) {
		currentHealth--;
		this->setCurrentAnim(fightInjure);
		return;
	}
	currentHealth--;
	this->setCurrentAnim(fightDying);
	//dead = true;

}

characterState Character::getState() {
	if (this->getAnim() == swordDeath) {
		return sDead;
	}

	if (this->getAnim() == staticJump) {
		if (this->getAnim()->getCurrentFrame() >= 1 &&
			this->getAnim()->getCurrentFrame() <= 12) {
			return sJumping;
		}
	}
	if (this->getAnim() == runningJump) {
		if (this->getAnim()->getCurrentFrame() <= 6) {
			return sJumping;
		}
	}

	if (getAnim() == running) {
		return sRunning;
	}
	else if (getAnim() == fall) {
		return sFalling;
	}
	else if (getAnim() == jumpGrab) {
		return sJumpGrab;
	}
	else if (getAnim() == climbUp) {
		return sClimbUp;
	}
	else if (getAnim() == step) {
		return sStep;
	}
	else if (getAnim() == runningTurn) {
		return sRunningTurn;
	}

	return sIdle;
}

void Character::setState(characterState state) {
	this->state = state;
}

void Character::spikeKill() {
	state = sDead;
	this->setCurrentAnim(spikeDeath);
	this->getAnim()->Freeze();
}

void Character::Land(int currentBlockY) {
	int fallHeight = currentBlockY - lastBlockY;

	if (getAnim() == fall && getAnim()->getCurrentFrame() == 4 && getAnim()->isFrozen()) {
		//this->setDefferY(0);
		switch (fallHeight) {
		case 0:
			getAnim()->Play();
			break;
		case 1:
			this->setCurrentAnim(crouch);
			//getAnim()->Play();
			break;
		case 2:
			this->setCurrentAnim(crouch);
			this->currentAnim->setCurrentFrame(2);
			this->currentAnim->setCurrentDisplayTime(2000.0);
			this->currentHealth--;
			break;
		default:
			//TODO: check for spikes
			this->setCurrentAnim(swordDeath);
			this->currentAnim->Freeze();
			this->currentHealth = 0;
			//this->defferX = 0;
			//this->defferY = 0;
			//this->yPos -= 30;

		}
	}
}

void Character::setFall(int currentBlockY) {
	// Sanity checks
	// Assert(currentAnim == running || currentAnim == runningJump || currentAnim == staticJump || currentAnim == fall);

	if (this->getAnim() == runningJump) {
		if (this->getAnim()->getCurrentFrame() < 6) {
			return;
		}
	}

	if (this->getAnim() == staticJump) {
		if (this->getAnim()->getCurrentFrame() < 11) {
			return;
		}
	}

	if (this->getAnim() == drop) { return; }
	if (this->getAnim() == hang) { return; }
	if (this->getAnim() == jumpGrab) { return; }
	if (this->getAnim() == climbUp) { return; }

	if (this->getAnim() != fall) {
		this->getAnim()->Reset();
		this->setCurrentAnim(fall);
		this->lastBlockY = currentBlockY;
		this->getAnim()->setCurrentFrame(4); //TODO
		this->getAnim()->Play();
		this->setAccY(4);
	}
	else if (this->getAnim()->getCurrentFrame() == 4) {
		this->getAnim()->Freeze();
	}
}

void Character::Heal() {
	if (currentHealth < maxHealth) {
		currentHealth++;
	}
}
int Character::getHealth() {
	return currentHealth;
}
int Character::getMaxHealth() {
	return maxHealth;
}

void Character::SwitchFacing() {
	if (facingRight) {		
		facingRight = false;

		fightIdle->setFlipped(false);
		fightStep->setFlipped(false);
		fightParry->setFlipped(false);
		fightStrike->setFlipped(false);
		fightInjure->setFlipped(false);
		fightDying->setFlipped(false);
		if (fightParried != NULL) { fightParried->setFlipped(false); }
		//fightStart->setFlipped(false);
		//fightFinish->setFlipped(false);

		//fall->setFlipped(false);
		//spikeDeath->setFlipped(false);
		//idle->setFlipped(false);
	
	}
	else {
		facingRight = true;

		fightIdle->setFlipped(true);
		fightStep->setFlipped(true);
		fightParry->setFlipped(true);
		fightStrike->setFlipped(true);
		fightInjure->setFlipped(true);
		fightDying->setFlipped(true);
		if (fightParried != NULL) { fightParried->setFlipped(true); }
		//fightStart->setFlipped(true);
		//fightFinish->setFlipped(true);

		//fall->setFlipped(true);
		//spikeDeath->setFlipped(true);
		//idle->setFlipped(true);

	}
}

bool Character::isImmune() {
	if (this->getAnim() == fightInjure || this->getAnim() == fightParry) {
		return true;
	} else {
		return false;
	}

}

bool Character::checkParryBy(Character* enemy) {

	if (this->getY() != enemy->getY())           { return false; }
	if (this->getAnim() != fightStrike)          { return false; }
	if (this->getAnim()->getCurrentFrame() >= 2) { return false; }

	if (enemy->isParying()) { 
		this->setCurrentAnim(fightParried);
		//this->getAnim()->setCurrentFrame(enemy->getAnim()->getCurrentFrame());
		return true;
	}
	return false;
}

bool Character::isParying()
{
	if (this->getAnim() == fightParry) {
		if (this->getAnim()->getCurrentFrame() < 2) {
			return true;
		}
	}
	return false;
}

bool Character::isHitting(Character* enemy, Level &level)
{
	
	if (level.getCharLevelBlockY(this) != level.getCharLevelBlockY(enemy)) {
		return false;
	}

	if (this->getAnim() != fightStrike) {
		return false;
	}

	if (enemy->isImmune()) {
		return false;
	}

	if (this->getAnim()->getCurrentFrame() != 2) {
		return false;
	}
	return true;
}

bool Character::isFighting() {
	return inFight;
}

bool Character::isIdle() {
	if (this->getAnim() == this->idle || this->getAnim() == this->fightIdle) {
		return true;
	}
	return false;
}

bool Character::isDead() {
	return this->getState() == sDead;
}

bool Character::isFacingRight() {
	return facingRight;
}

void Character::FaceCharacter(Character& character, Level& level) {
	int characterLevelX = level.getSceneBlockYByCoord(character.getX());
	int guardLevelX = level.getSceneBlockYByCoord(this->getX());

	if (characterLevelX < guardLevelX && facingRight) {
		this->SwitchFacing();
	}
	if (characterLevelX > guardLevelX && !facingRight) {
		this->SwitchFacing();
	}
}

void Character::EngageEnemy(Character & enemy) {
	if (!hasSword) {
		return;
	}
	if (!isFighting()) {
		if (this->getAnim() == idle) {
			if (seenEnemies.find(&enemy) == seenEnemies.end()) {
				std::pair<Character*, bool>  node = std::make_pair<Character*, bool>(&enemy, true);
				seenEnemies.insert(node);
				this->setCurrentAnim(fightStart);
				inFight = true;
			}
		}
	}
}

bool Character::isInScene() {
	return inScene;
}

bool Character::isMovingUp() {
	if (getAnim() == climbUp || getAnim() == jumpGrab || getAnim() == staticJump || getAnim() == hang) {
		return true;
	}
	return false;
}

void Character::setInScene(bool val) {
	inScene = val;
}
