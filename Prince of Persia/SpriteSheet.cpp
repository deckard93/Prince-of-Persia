#include "SpriteSheet.h"


//constructors
SpriteSheet::SpriteSheet(Sprite* sprite, int width) {

	sheet = sprite;
	frameWidth = width;
	frameHeight = 0;
	frameNum = sprite->width / width;

}	
SpriteSheet::SpriteSheet(Sprite* sprite, int width, int height) {

	frameWidth = width;
	frameHeight = height;

	sheet = sprite;

	frameNum = (sprite->height / height) * (sprite->width / width);

}	
SpriteSheet::SpriteSheet(const WCHAR* spriteName, int frames) {
	sheet = LoadSprite(spriteName);
	frameWidth = sheet->width / frames;
	frameHeight = 0;
	frameNum = frames;

}

//getters
int SpriteSheet::getFrameCount() {

	return frameNum;

}
int SpriteSheet::getFrameWidth() {

	return frameWidth;

}
int SpriteSheet::getFrameHeight() {
	return frameHeight;
}

Sprite* SpriteSheet::getSprite() {

	return sheet;

}

//destructor
SpriteSheet::~SpriteSheet() {

	if(sheet != NULL) {
		free(sheet);
		sheet = NULL;
	}
}
