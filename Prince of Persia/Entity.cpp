#include "Entity.h"


//constructor
Entity::Entity(Animation* a,int x, int y, entityType t) {
	type = t;
	
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

//coords
void Entity::setX(int x) {
	xPos = x;
}
void Entity::setY(int y) {
	yPos = y;
}

void Entity::setLevelX(int x) {
	levelX = x;
}
void Entity::setLevelY(int y) {
	levelY = y;
}

void Entity::setSceneX(int x) {
	sceneX = x;
}
void Entity::setSceneY(int y) {
	sceneY = y;
}

void Entity::defferMoveX(int x) {
	defferX = x;
}
void Entity::defferMoveY(int y) { 
	defferY = y;
}

void Entity::setCurrentAnim(Animation* anim) {
	currentAnim = anim;
	if (anim == NULL) { return; }
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
int Entity::getLevelX() {
	return levelX;
}
int Entity::getLevelY() {
	return levelY;
}
int Entity::getSceneX() {
	return sceneX;
}
int Entity::getSceneY() {
	return sceneY;
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
void Entity::setType(entityType t) {
	type = t;
}
entityType Entity::getType() {
	return type;
}

//destructor
Entity::~Entity() {
	if(currentAnim != NULL) {
		delete currentAnim;
		currentAnim = NULL;
	}
}

