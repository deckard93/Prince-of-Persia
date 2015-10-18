#pragma once

#include "Graphics.h"
#include "Timer.h"
#include "Sprite.h"


class SpriteSheet {

//variables
private:
	Sprite* sheet;

	int frameWidth;
	int frameHeight;

	int frameNum;

//functions
public:
	//constructors
	SpriteSheet(const WCHAR* spriteName, int frames);
	SpriteSheet(Sprite* sprite, int frames);			
	SpriteSheet(Sprite* sprite, int width, int height);	

	//getters
	int getFrameCount();
	int getFrameWidth();
	int getFrameHeight();

	Sprite* getSprite();

	//destructor
	~SpriteSheet();

};