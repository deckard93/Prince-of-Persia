#pragma once

#include "Entity.h"
#include "Timer.h"
#include "Graphics.h"

class Gate : public Entity {

//methods
public:
	Gate(int x, int y, int levelX, int levelY);
	~Gate() {}

	void Open();
	void Close();

	void Animate(Graphics* graphics);

private:



//attributes
private:
	int state;
	Timer* timer;

	Animation* gate;

public:

};