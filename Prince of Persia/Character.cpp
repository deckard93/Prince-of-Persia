#include "Character.h"

Character::Character() {}

void Character::Hurt() {
	currentHealth--;
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


