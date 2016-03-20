#include "Character.h"

Character::Character() {}

void Character::Hurt() {
	
	if (currentHealth > 0) {
		currentHealth--;
		this->setCurrentAnim(fightInjure);
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
		//fightStart->setFlipped(true);
		//fightFinish->setFlipped(true);

		//fall->setFlipped(true);
		//spikeDeath->setFlipped(true);
		//idle->setFlipped(true);

	}
}

bool Character::isImmune() {
	if (this->getAnim() == fightInjure) {
		return true;
	} else {
		return false;
	}

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
