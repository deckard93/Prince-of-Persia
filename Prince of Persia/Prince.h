#pragma once

#include "Animation.h"
#include "Input.h"
#include "Entity.h"
#include "Character.h"
#include "Game.h"


enum princeState { sRunning, sFalling, sIdle, sDead, sFinish };

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
	//Animation* fallDeath;

	princeState state;

	bool hasSword;

	int idleSpeedX;
	int runningSpeedX;
	int climbSpeedX;
	int* princeSpeedX;
	int lastBlockY;

public:
	

//methods
public:
	//constructors
	Prince();
	Prince(int x, int y);

	//getters
	princeState getPrinceState();

	//setters
	void setState(princeState state);
	void FightController(Input * input);
	void NormalController(Input * input);
	void Disengage();

	//function
	void HandlePrince(Input* input);
	bool isStriking();
	bool isJumping();
	int Drink();
	void increaseMaxHealth();
	void Animate(Graphics* graphics);
	int setFall(int currentBlockY);
	int PickUpSword();
	void Land(int currentBlockY);
	void Catch();

	void spikeKill();
	void defaultToIdle();

	//destructors
	~Prince();

private:
	void ActionHandler(Action action);
	void handleInput();
	void switchFacing();
	void doAnimationEffects();

};