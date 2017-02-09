#pragma once

#include "Graphics.h"
#include "Timer.h"
#include "Sprite.h"


class SpriteSheet {

// Variables
private:
    Sprite* sheet;

    int frameWidth;
    int frameHeight;

    int frameNum;

// Functions
public:
    // Constructors
    SpriteSheet(const WCHAR* spriteName, int frames);
    SpriteSheet(Sprite* sprite, int frames);            
    SpriteSheet(Sprite* sprite, int width, int height);    

    // Getters
    int getFrameCount();
    int getFrameWidth();
    int getFrameHeight();

    Sprite* getSprite();

    // Destructor
    ~SpriteSheet();
};