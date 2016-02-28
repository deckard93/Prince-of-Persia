#pragma once

#include <Windows.h>

class Input {

public:
	Input();

	void setLeftStatus(bool status);
	bool isLeftPressed();

	void setUpStatus(bool status);
	bool isUpPressed();

	void setRightStatus(bool status);
	bool isRightPressed();

	void setDownStatus(bool status);
	bool isDownPressed();

	void setShiftStatus(bool status);
	bool isShiftPressed();

	void setCtrlStatus(bool status);
	bool isCtrlPressed();

	void setSpaceStatus(bool status);
	bool isSpacePressed();
	
	bool hasUpBeenPressed();
	bool hasDownBeenPressed();

	bool getKeyStatus(char k);
	void setKeyStatus(int k, bool status);
	bool hasBeenPressed(char k);


private:
	bool down;
	bool up;
	bool left;
	bool right;
	bool shift;
	bool ctrl;
	bool space;

	bool upMessageSent;
	bool downMessageSent;

	bool key[256];
	bool keyMessageSent[256];

};