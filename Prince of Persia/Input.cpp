#include "Input.h"

Input::Input() {
	down = false;
	up = false;
	left = false;
	right = false;

	shift = false;
	space = false;
	ctrl = false;

	upMessageSent = false;
}

bool Input::isUpPressed() {
	return up;
}
bool Input::isDownPressed() {
	return down;
}
bool Input::isLeftPressed() {
	return left;
}
bool Input::isRightPressed() {
	return right;
}
bool Input::isShiftPressed() {
	return shift;
}
bool Input::isSpacePressed() {
	return space;
}
bool Input::isCtrlPressed() {
	return ctrl;
}

void Input::setDownStatus(bool status) {
	down = status;
	if(status == false) 
	{
		downMessageSent = false;
	}
}
void Input::setUpStatus(bool status) {
	up = status;
	if(status == false) {
		upMessageSent = false;
	}
}
void Input::setLeftStatus(bool status) {
	left = status;
}
void Input::setRightStatus(bool status) {
	right = status;
}
void Input::setShiftStatus(bool status) {
	shift = status;
}
void Input::setCtrlStatus(bool status) {
	ctrl = status;
}
void Input::setSpaceStatus(bool status) {
	space = status;
}

bool Input::hasUpBeenPressed() {
	if (up == true) {
		if(!upMessageSent) {
			upMessageSent = true;
			return true;
		}
	}
	return false;
}
bool Input::hasDownBeenPressed() {
	if (down == true) {
		if(!downMessageSent) {
			downMessageSent = true;
			return true;	
		}
	}
	return false;
}

bool Input::getKeyStatus(char k) {
	return key[(int)k - 65];
}
void Input::setKeyStatus(int k, bool status) {
	key[k - 65] = status;

	if(status == false) {
		keyMessageSent[k - 65] = false;
	}
}
bool Input::hasBeenPressed(char k) {
	if (key[(int)k - 65] == true) {
		if(!keyMessageSent[k - 65]) {
			keyMessageSent[k - 65] = true;
			return true;
		}
	}
	return false;
}