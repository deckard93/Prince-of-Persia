#pragma once

#include "Animation.h"
#include "Input.h"
#include "Entity.h"
#include "Game.h"

class Prince : public Entity {

//variables
private:
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

	Animation* fightIdle;
	Animation* fightStep;
	Animation* fightParry;
	Animation* fightStrike;
	Animation* fightStart;
	Animation* fightFinish;

	bool isAnimating;
	bool facingRight;
	bool hasSword;
	bool fightStance;

	int maxHealth;
	int currentHealth;

	int idleSpeedX;
	int runningSpeedX;
	int climbSpeedX;

	int* princeSpeedX;

public:
	

//methods
public:
	//constructors
	Prince();
	Prince(int x, int y);

	//getters
	int getHealth();
	int getMaxHealth();

	//setters

	//function
	void HandlePrince(Input* input);
	void Hurt();
	void Heal();
	int Drink();
	void increaseMaxHealth();
	void Animate(Graphics* graphics);
	int setFall();
	int PickUpSword();

	//destructors
	~Prince();

private:
	void defaultToIdle();
	void handleInput();
	void switchFacing();
	void doAnimationEffects();

};