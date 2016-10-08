#pragma once

#include "Character.h"

class Guard : public Character {
private:

public:
	Guard(Audio* audio,int x, int y);
	void ActionHandler(Action action);
	void Animate(Graphics* graphics);
	void defaultToIdle();
	void SwitchFacing();
};