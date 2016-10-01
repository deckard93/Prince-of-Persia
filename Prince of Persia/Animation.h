#pragma once;


#include "Graphics.h"


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
	bool effectPending;

	float currentDisplayTime;
	float defaultDisplayTime;
	float* frameDisplayTime;

//functions
public:
	//Constructors
	Animation(SpriteSheet* s);
	Animation(const WCHAR* spriteName,int frames);
	Animation(Sprite* sprite, int frames);
	Animation(Sprite* sprite, int frames, float* timing);

	//Getters
	bool isEffectPending();
	float getDefaultDisplayTime();
	float getCurrentDisplayTime();
	float getFrameDisplayTime();
	int getLastFrameNr();
	SpriteSheet* getSheet();
	int getCurrentFrame();

	//Setters
	void setEffectDone();
	void setForward();
	void setReverse();
	void setLoop(bool in);
	void setFlipped(bool in);
	void setDisplayTime(float dispTim);
	void setDisplayTime(float* dispTim);
	void setCurrentDisplayTime(float dispTim);

	//Functions
	void Play();
	void Stop();
	void Update(Graphics* graphics, int x, int y);
	void NextFrame();
	void setCurrentFrame(int frame);
	void Freeze();
	bool isFrozen();
	void Reset();

	//drawing
	void Display(Graphics* graphics, int x, int y);
	void DisplayFrame(Graphics* graphics,int x, int y, int index);
	
	bool isFinished();
	bool isReversed();
	bool isFlipped();

	//Destructors
	~Animation();

private:

};