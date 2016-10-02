#pragma once

#include "Animation.h"
#include "Input.h"
#include "Entity.h"
#include "Character.h"
#include "Game.h"


class Prince : public Character {
	static const int fightDisplacement = 40;

//variables
private:
	/*
	Animation* fightIdle;
	Animation* fightStep;
	Animation* fightParry;
	Animation* fightStrike;

	Animation* swordDeath;
	Animation* guilotineDeath;
	Animation* spikeDeath;
	Animation* fallDeath;
	*/

	/*
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
	*/

	Animation* fallDamage;
	Animation* missStep;
	//Animation* fallDeath;


	int idleSpeedX;
	int runningSpeedX;
	int climbSpeedX;
	int* princeSpeedX;
	

//methods
public:
	//constructors
	Prince();

	void setIdle();
	void FightController(Input * input);
	void NormalController(Input * input);
	void Disengage();

	void HandlePrince(Input* input);
	bool isStriking();
	bool isGoingDown();
	bool isDrop();

	int Drink();
	int PickUpSword();
	void increaseMaxHealth();
	void Animate(Graphics* graphics);
	void setMissStep();
	void setDrop();
	void setClimbDown();
	void Catch();

	characterState getState();
	void defaultToIdle();

	//destructors
	~Prince();

private:
	void ActionHandler(Action action);
	void handleInput();
	void switchFacing();
};