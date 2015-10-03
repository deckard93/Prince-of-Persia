#include "Input.h"

Input::Input() {
	down = false;
	up = false;
	left = false;
	right = false;

	shift = false;
	space = false;
	ctrl = false;


	prevDown = false;
	prevUp = false;
	prevLeft = false;
	prevRight = false;

	prevShift = false;
	prevSpace = false;
	prevCtrl = false;

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

bool Input::getRightPrev() {
	return prevRight;
}
bool Input::getLeftPrev() {
	return prevLeft;


}
bool Input::getUpPrev() {
	return prevUp;
}
bool Input::getDownPrev() {
	return prevDown;
}
bool Input::getShiftPrev() {
	return prevShift;
}
bool Input::getSpacePrev() {
	return prevSpace;
}
bool Input::getCtrlPrev() {
	return prevCtrl;
}

void Input::setDownStatus(bool status) {
	prevDown = down;
	down = status;
	if(status == false) 
	{
		downMessageSent = false;
	}
}
void Input::setUpStatus(bool status) {
	prevUp = up;
	up = status;
	if(status == false) 
	{
		upMessageSent = false;
	}
}
void Input::setLeftStatus(bool status) {
	prevLeft = left;
	left = status;
}
void Input::setRightStatus(bool status) {
	prevRight = right;
	right = status;
}
void Input::setShiftStatus(bool status) {
	prevShift = shift;
	shift = status;
}
void Input::setCtrlStatus(bool status) {
	prevCtrl = ctrl;
	ctrl = status;
}
void Input::setSpaceStatus(bool status) {
	prevSpace = space;
	space = status;
}

bool Input::hasUpBeenPressed()
{
	if (up == true) 
	{
		if(!upMessageSent) 
		{
			upMessageSent = true;
			return true;
			
		}
	}

	return false;
}
bool Input::hasDownBeenPressed()
{
	if (down == true) 
	{
		if(!downMessageSent) 
		{
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