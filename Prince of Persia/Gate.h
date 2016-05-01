#pragma once

#include "Entity.h"
#include "Timer.h"
#include "Graphics.h"

enum gateState { sOpened, sClosed, sClosing, sOpening };

class Gate : public Entity {

//methods
public:
	Gate(int x, int y, int levelX, int levelY);
	~Gate() {}

	int slowTime;
	int fastTime;

	void Open();
	void Close();

	bool isOpen();

	void Animate(Graphics* graphics);

private:



//attributes
protected:
	gateState state;
	Timer* timer;

	Animation* gate;

public:

};