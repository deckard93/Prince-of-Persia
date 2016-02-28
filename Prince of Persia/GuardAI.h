#pragma once

class Level;
class Prince;
class Guard;

class GuardAI : public Guard {
private:
	bool engaged; //triggered //aggroed
	
public:
	GuardAI(int x, int y);
	void Control(Prince & prince, Level & level);
};