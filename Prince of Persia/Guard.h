#pragma once

#include "Entity.h";
#include "Character.h";

class Guard : public Character {
private:

public:
	Guard(int x, int y);
	void ActionHandler(Action action);
	void Animate(Graphics* graphics);
	void defaultToIdle();
	void SwitchFacing();
};