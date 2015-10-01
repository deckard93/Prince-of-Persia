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

void Input::setSStatus(bool status) {

	S = status;
	if(status == false) 
	{
		sMessageSent = false;
	}

}

void Input::setWStatus(bool status) {

	W = status;
	
	if(status == false) 
	{
		wMessageSent = false;
	}

}

void Input::setDStatus(bool status) {

	D = status;

	
	if(status == false) 
	{
		dMessageSent = false;
	}

}

void Input::setAStatus(bool status) {
	A = status;

	
	if(status == false) 
	{
		aMessageSent = false;
	}

}




bool Input::getSStatus() {

	return S;

}

bool Input::getWStatus() {

	return W;

}

bool Input::getDStatus() {

	return D;

}

bool Input::getAStatus() {

	return A;

}


bool Input::hasWBeenPressed() {
	if (W == true) 
	{
		if(!wMessageSent) 
		{
			wMessageSent = true;
			return true;
			
		}
	}

	return false;
}

bool Input::hasABeenPressed() {
	if (A == true) 
	{
		if(!aMessageSent) 
		{
			aMessageSent = true;
			return true;
			
		}
	}

	return false;
}

bool Input::hasSBeenPressed() {
	if (S == true) 
	{
		if(!sMessageSent) 
		{
			sMessageSent = true;
			return true;
			
		}
	}

	return false;
}

bool Input::hasDBeenPressed() {
	if (D == true) 
	{
		if(!dMessageSent) 
		{
			dMessageSent = true;
			return true;
			
		}
	}

	return false;
}
