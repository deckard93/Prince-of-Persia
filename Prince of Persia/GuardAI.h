#pragma once

class Level;
class Prince;
class Guard;

class GuardAI : public Guard {

//variables
private:
	bool engaged; //triggered //aggroed

//functions
public:
	GuardAI(int x, int y);
	void Control(Prince & prince, Level & level);
	
private:
	void FacePrince(Prince & prince, Level & level);
};