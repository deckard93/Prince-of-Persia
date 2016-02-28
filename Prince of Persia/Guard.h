#pragma once

#include "Entity.h";
#include "Character.h";

class Guard : public Character {
private:
	Animation* fightIdle;
	Animation* fightStep;
	Animation* fightParry;
	Animation* fightStrike;

	Animation* fightStart;
	Animation* fightFinish;

	Animation* swordDeath;
	Animation* guilotineDeath;
	Animation* spikeDeath;

public:
	Guard(int x, int y);
	void ActionHandler(Action action);
	void Animate(Graphics* graphics);
	void defaultToIdle();
};