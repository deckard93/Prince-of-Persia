#pragma once

#include "Animation.h"

enum entityType : int { noneT, torchT, guilotineT, gateT, potionT, poisonPotT, extendPotT, spikeT, swordT, guardT };

class Entity {

//variables
protected:
	Animation* currentAnim;

	int xPos;
	int yPos;

	int levelX;
	int levelY;

	int sceneX;
	int sceneY;

	int defferX;
	int defferY;

	entityType type;

//methods
public:
	//constructor
	Entity(Animation* anim,int x, int y, entityType t = noneT);
	Entity();

	//setters
	void setX(int x);
	void setY(int y);

	void setLevelX(int x);
	void setLevelY(int y);

	void setSceneX(int x);
	void setSceneY(int y);

	void defferMoveX(int x);
	void defferMoveY(int y);

	void setCurrentAnim(Animation* anim);

	//getters
	Animation* getAnim();
	int getX();
	int getY();

	int getLevelX();
	int getLevelY();

	int getSceneX();
	int getSceneY();

	int getMidX();
	int getMidY();

	int getDefferX();
	int getDefferY();
	
	//functions
	virtual void Animate(Graphics* graphics, int& moveX, int& moveY);
	virtual void Animate(Graphics* graphics);

	void setType(entityType t);
	entityType getType();

	virtual void EventHandler(int Event) {}

	void MoveX(int x);
	void MoveY(int y);

	//destructor
	~Entity();

};
