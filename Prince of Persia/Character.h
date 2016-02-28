#pragma once;

#include "Animation.h"
#include "Entity.h"

enum Action { aLeftJump, aRightJump, aJumpGrab, aCrouch, aGoRight, aGoLeft, aClimbUp, aClimbDown, aLeftStep, aRightStep, aHang, aNone, aStrike, aParry, aEngage, aDisengage };

class Character : public Entity {
//Functions
public:
	Character();
	void Hurt();
	void Heal();

	int getHealth();
	int getMaxHealth();

protected:
	void defaultToIdle();

private:

//variables
protected:
	bool isAnimating;
	bool facingRight;
	int maxHealth;
	int currentHealth;

public:

};