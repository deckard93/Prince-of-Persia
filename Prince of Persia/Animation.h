#pragma once;

#include <assert.h>

#include "Graphics.h"
#include "SpriteSheet.h"


class Animation  {

//variables
public:
	Timer timer;

private:
	SpriteSheet* sheet;

	int currentFrame;
	int inc;

	bool flipped;
	bool playForward;
	bool finished;
	bool loop;

	float defaultDisplayTime;
	float* frameDisplayTime;

//functions
public:
	//Constructors
	Animation(SpriteSheet* s);
	Animation(const WCHAR* spriteName,int frames);

	//Getters
	float getDefaultDisplayTime();
	float getCurrentDisplayTime();
	float getFrameDisplayTIme();
	SpriteSheet* getSheet();
	int getCurrentFrame();

	//Setters
	void setForward();
	void setReverse();
	void setLoop(bool in);
	void setFlipped(bool in);
	void setDisplayTime(float dispTim);
	void setDisplayTime(float* dispTim);

	//Functions
	void Play();
	void Stop();
	void Update(Graphics* graphics, int x, int y);
	void NextFrame();
	void setCurrentFrame(int frame);
	void Freeze();

	//drawing
	void Display(Graphics* graphics, int x, int y);
	void DisplayFrame(Graphics* graphics,int x, int y, int index);
	
	bool isFinished();
	bool isReversed();

	//Destructors
	~Animation();

private:

};