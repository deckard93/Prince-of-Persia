#pragma once;

#include "Animation.h"
#include "Entity.h"

class Level;

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
	bool isImmune();
	bool isParying();
	bool checkParryBy(Character* enemy);
	bool isHitting(Character * enemy);
	bool isFighting();
	bool isIdle();
	bool isDead();
	bool isFacingRight();
	void FaceCharacter(Character& character, Level& level);

	void EngageEnemy(Character & enemy);

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
	Animation* idle;

	Animation* fightIdle;
	Animation* fightStep;
	Animation* fightParry;
	Animation* fightParried;
	Animation* fightStrike;

	Animation* fightInjure;
	Animation* fightDying;
	Animation* fightStart;
	Animation* fightFinish;

	Animation* swordDeath;
	Animation* guilotineDeath;
	Animation* spikeDeath;

//variables
protected:
	bool isAnimating;
	bool facingRight;
	bool inFight;
	bool dead;
	int maxHealth;
	int currentHealth;
	
public:

};
