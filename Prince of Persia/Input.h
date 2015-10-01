#pragma once

#include <Windows.h>

class Input {
	

public:
	Input();

	void setWStatus(bool status);
	bool getWStatus();

	void setSStatus(bool status);
	bool getSStatus();

	void setDStatus(bool status);
	bool getDStatus();

	void setAStatus(bool status);
	bool getAStatus();




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

	bool hasWBeenPressed();
	bool hasABeenPressed();
	bool hasSBeenPressed();
	bool hasDBeenPressed();

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

	bool wMessageSent;
	bool sMessageSent;
	bool aMessageSent;
	bool dMessageSent;


	bool W;
	bool S;
	bool D;
	bool A;

};