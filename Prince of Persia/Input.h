#pragma once

#include <Windows.h>

class Input {
	

public:
	Input();

	void setLeftStatus(bool status);
	bool isLeftPressed();
	bool getLeftPrev();

	void setUpStatus(bool status);
	bool isUpPressed();
	bool getUpPrev();

	void setRightStatus(bool status);
	bool isRightPressed();
	bool getRightPrev();

	void setDownStatus(bool status);
	bool isDownPressed();
	bool getDownPrev();

	void setShiftStatus(bool status);
	bool isShiftPressed();
	bool getShiftPrev();

	void setCtrlStatus(bool status);
	bool isCtrlPressed();
	bool getCtrlPrev();

	void setSpaceStatus(bool status);
	bool isSpacePressed();
	bool getSpacePrev();
	
	bool Input::hasUpBeenPressed();
	bool Input::hasDownBeenPressed();
private:
	bool down;
	bool up;
	bool left;
	bool right;
	bool shift;
	bool ctrl;
	bool space;

	bool prevDown;
	bool prevUp;
	bool prevLeft;
	bool prevRight;
	bool prevShift;
	bool prevSpace;
	bool prevCtrl;

	bool upMessageSent;
	bool downMessageSent;

};