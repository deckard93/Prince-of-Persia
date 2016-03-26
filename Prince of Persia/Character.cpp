#include "Character.h"
#include "Level.h"

Character::Character() {
	inFight = false;
	dead = false;
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

bool Character::isHitting(Character* enemy)
{
	
	if (this->getY() != enemy->getY()) {
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
	return dead;
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
