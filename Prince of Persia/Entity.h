#pragma once

#include "Animation.h"

class Entity {

//variables
private:
	Animation* currentAnim;

	int xPos;
	int yPos;

	int defferX;
	int defferY;

//methods
public:
	//constructor
	Entity(Animation* anim,int x, int y);
	Entity();

	//setters
	void setX(int x);
	void setY(int y);

	void defferMoveX(int x);
	void defferMoveY(int y);

	void setCurrentAnim(Animation* anim);

	//getters
	Animation* getAnim();
	int getX();
	int getY();

	int getDefferX();
	int getDefferY();
	
	//functions
	virtual void Animate(Graphics* graphics, int& moveX, int& moveY);
	virtual void Animate(Graphics* graphics);

	virtual void EventHandler(int Event) {}

	void MoveX(int x);
	void MoveY(int y);

	//destructor
	~Entity();
private:

};