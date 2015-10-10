#include "Entity.h"


//constructor
Entity::Entity(Animation* a,int x, int y) {
	currentAnim = a;

	xPos = x;
	yPos = y;

	defferX = 0;
	defferY = 0;
}
Entity::Entity() {
	xPos = 0;
	yPos = 0;

	defferX = 0;
	defferY = 0;
}

//setters
void Entity::setX(int x) {
	xPos = x;
}
void Entity::setY(int y) {
	yPos = y;
}
void Entity::defferMoveX(int x) {
	defferX = x;
}
void Entity::defferMoveY(int y) { 
	defferY = y;
}
void Entity::setCurrentAnim(Animation* anim) {
	currentAnim = anim;
	currentAnim->Play();
}

//getters
Animation* Entity::getAnim() {
	return currentAnim;
}
int Entity::getX() {
	return xPos;
}
int Entity::getY() {
	return yPos;
}
int Entity::getMidX() {
	return xPos + currentAnim->getSheet()->getFrameWidth() / 2;
}
int Entity::getMidY() {
	return yPos + currentAnim->getSheet()->getFrameHeight() / 2;
}
int Entity::getDefferX() {
	int temp = defferX;
	defferX = 0;
	return temp;
}
int Entity::getDefferY() {
	int temp = defferY;
	defferY = 0;
	return temp;
}
//functions
void Entity::Animate(Graphics* graphics, int& moveX, int& moveY) {
	currentAnim->Update(graphics, xPos, yPos);
}
void Entity::Animate(Graphics* graphics) {
	int x;
	int y;

	//staticAnimation->Update(&x, &y);
	currentAnim->Update(graphics, xPos, yPos);

	//MoveX(x);
	//MoveY(y);
}
void Entity::MoveX(int x) {
	xPos += x;
}
void Entity::MoveY(int y) {
	yPos += y;
}

//destructor
Entity::~Entity() {
	if(currentAnim != NULL) {
		delete currentAnim;
		currentAnim = NULL;
	}
}

