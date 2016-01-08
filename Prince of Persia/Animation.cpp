#include "Animation.h"


//Constructors
Animation::Animation(SpriteSheet* s) {

	sheet = s;

	frameDisplayTime = NULL;
	defaultDisplayTime = 0;

	playForward = true;
	finished    = true;
	loop        = false;
	flipped     = false;

	currentFrame = 0;
	inc = 1;
}
Animation::Animation(Sprite* sprite, int frames, float* timing) : sheet(NULL) {

	sheet = new SpriteSheet(sprite, frames);

	frameDisplayTime = timing;
	defaultDisplayTime = 0;
	currentFrame = 0;

	playForward = true;
	finished = true;
	loop = false;
	flipped = false;

	timer.StartWatch();

}
Animation::Animation(Sprite* sprite, int frames) : sheet(NULL) {

	sheet = new SpriteSheet(sprite, frames);

	frameDisplayTime = NULL;
	defaultDisplayTime = 0;
	currentFrame = 0;

	playForward = true;
	finished = true;
	loop = false;
	flipped = false;

	timer.StartWatch();

}
Animation::Animation(const WCHAR* spriteName,int frames) : sheet(NULL) {

	sheet = new SpriteSheet(spriteName, frames);

	frameDisplayTime = NULL;
	defaultDisplayTime = 0;
	currentFrame = 0;

	playForward = true;
	finished = true;
	loop = false;
	flipped = false;

	timer.StartWatch();

}

//Getters
int Animation::getLastFrameNr() {
	return sheet->getFrameCount() - 1;
}
bool Animation::isEffectPending() { 
	if (effectPending) { OutputDebugString(L"waiting!\n"); } else { OutputDebugString(L"done!\n"); }
	return effectPending; 
}
float Animation::getDefaultDisplayTime() {
	return defaultDisplayTime;
}
float Animation::getCurrentDisplayTime() {
	return timer.GetTimeMilli();
}
float Animation::getFrameDisplayTIme() {
	return frameDisplayTime[currentFrame];
}
SpriteSheet* Animation::getSheet() {
	return sheet;
}
int Animation::getCurrentFrame() {
	return currentFrame;
}

//Setters
void Animation::setEffectDone() { effectPending = false; }
void Animation::setForward() {
	//currentFrame = 0;
	playForward = true;
}
void Animation::setReverse() {
	//currentFrame = sheet->getFrameCount() - 1;
	playForward = false;
}
void Animation::setLoop(bool in) {
	loop = in;
}
void Animation::setFlipped(bool in) {
	flipped = in;
}
void Animation::setDisplayTime(float dispTim) {
	defaultDisplayTime = dispTim;
}
void Animation::setDisplayTime(float* dispTim) {
	frameDisplayTime = dispTim;
}

//Functions
void Animation::Play() {
	finished = false;
	inc = 1;
	timer.StartWatch();
	//effectPending = true;
}
void Animation::Stop() {
	finished = true;
}
void Animation::Update(Graphics* graphics, int x, int y) {
	Display(graphics, x, y);		//draw
	
	if(!finished) {
		NextFrame();				//update animation
	}
}
void Animation::NextFrame() {
	if(frameDisplayTime == NULL) {

		if(timer.GetTimeMilli() > defaultDisplayTime) {
			timer.StopWatch();
			timer.StartWatch();

			if(playForward) {
				currentFrame+=inc;
				effectPending = true;
			} else {
				currentFrame-=inc;
				effectPending = true;
			}

			if(currentFrame >= sheet->getFrameCount()) {
				currentFrame = 0;
				if(!loop) finished = true;
			}

			if(currentFrame < 0) {
				currentFrame = sheet->getFrameCount() - 1;
				if(!loop) finished = true;
			}
		}
	} else {

		if (timer.GetTimeMilli() > frameDisplayTime[currentFrame]) {
			timer.StopWatch();
			timer.StartWatch();

			if (playForward) {
				currentFrame += inc;
				effectPending = true;
			}
			else {
				currentFrame -= inc;
				effectPending = true;
			}

			if (currentFrame >= sheet->getFrameCount()) {
				currentFrame = 0;
				if (!loop) finished = true;
			}

			if (currentFrame < 0) {
				currentFrame = sheet->getFrameCount() - 1;
				if (!loop) finished = true;
			}
		}

	}

}
void Animation::setCurrentFrame(int frame) {
	currentFrame = frame;
}
void Animation::Freeze() {
	inc = 0;
}
bool Animation::isFrozen() {
	if(inc == 0) {
		return true;
	} 
	return false;
}

//drawing
void Animation::Display(Graphics* graphics, int x, int y) {
	graphics->DrawFrame(x, y, sheet, currentFrame, flipped);
}
void Animation::DisplayFrame(Graphics* graphics,int x, int y, int index) {

	graphics->DrawFrame(x, y, sheet, index, flipped);

}
	
bool Animation::isFinished() {
	return finished;
}
bool Animation::isReversed() {
	return !playForward;
}

//Destructors
Animation::~Animation() {

	if(sheet != NULL) {
		delete sheet;
	}

	if(frameDisplayTime != NULL) {
		free(frameDisplayTime);
	}

}