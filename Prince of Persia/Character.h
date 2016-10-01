#pragma once;


#include "Entity.h"

#include <map>


class Level;


enum Action { aLeftJump, aRightJump, aJumpGrab, aCrouch, aGoRight, aGoLeft, aClimbUp, aClimbDown, aLeftStep, aRightStep, aHang, aNone, aStrike, aParry, aEngage, aDisengage };
enum characterState { sRunning, sFalling, sIdle, sDead, sFinish, sJumping };

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
	bool isHitting(Character * enemy, Level & level);
	bool checkParryBy(Character* enemy);
	bool isFighting();
	bool isIdle();
	bool isDead();
	
	bool isFacingRight();
	void FaceCharacter(Character& character, Level& level);

	void EngageEnemy(Character & enemy);

	bool isInScene();
	void setInScene(bool val);

	characterState Character::getState();
	void setState(characterState state);
	void spikeKill();

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

	std::map<Character*, bool> seenEnemies;

	characterState state;
	bool hasSword;

//variables
protected:
	bool isAnimating;
	bool facingRight;
	bool inFight;
	bool dead;

	bool inScene;

	int maxHealth;
	int currentHealth;
	
public:

};
