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

	void SwitchFacing();
	Animation* idle;
	Animation* turn;
	Animation* running;
	Animation* climbUp;
	Animation* crouch;
	Animation* jumpGrab;
	Animation* staticJump;
	Animation* step;
	Animation* hang;
	Animation* runningJump;
	Animation* runningTurn;
	Animation* fall;
	Animation* drop;
	Animation* drink;
	Animation* pickSword;

protected:
	void defaultToIdle();

protected:

	Animation* fightIdle;
	Animation* fightStep;
	Animation* fightParry;
	Animation* fightStrike;

	Animation* fightStart;
	Animation* fightFinish;

	Animation* swordDeath;
	Animation* guilotineDeath;
	Animation* spikeDeath;

//variables
protected:
	bool isAnimating;
	bool facingRight;
	int maxHealth;
	int currentHealth;

public:

};