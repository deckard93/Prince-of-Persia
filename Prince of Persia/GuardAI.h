#pragma once

#include "Guard.h"

class GuardAI : public Guard {

	//variables
private:
	int chanceToParry;
	int chanceToStrike;
	bool engaged; //triggered //aggroed

//functions
public:
	GuardAI(Audio* audio, int x, int y);
	void Control(Prince & prince, Level & level);
};