#include "Spikes.h"
#include "Game.h"

Spikes::Spikes(int x, int y, int levelX, int levelY) {
	type = spikeT;
	on = false;

	setX(x);
	setY(y);

	setLevelX(levelX);
	setLevelY(levelY);

	background = new Animation(Game::getSprite("spikesBackground"), 6);
	foreground = new Animation(Game::getSprite("spikesForeground"), 6);

	background->Stop();
	background->setDisplayTime(90);
	background->setCurrentFrame(0);

	foreground->Stop();
	foreground->setDisplayTime(90);
	foreground->setCurrentFrame(0);

	miliUntilOff = 700;

	timer = new Timer();
}

void Spikes::On() {
	timer->StartWatch();
	if (!on) {
		background->setForward();
		background->setCurrentFrame(1);
		background->Play();

		foreground->setForward();
		foreground->setCurrentFrame(1);
		foreground->Play();
		on = true;
	}
}

void Spikes::Off() {
	timer->StartWatch();
	if (on) {
		background->setReverse();
		background->setCurrentFrame(background->getLastFrameNr() - 1);
		background->Play();

		foreground->setReverse();
		foreground->setCurrentFrame(foreground->getLastFrameNr() - 1);
		foreground->Play();
		on = false;
	}
}

void Spikes::Kill() {

}

void Spikes::AnimateForeground(Graphics* graphics) {
	CustomAnimate(graphics, foreground);
}

void Spikes::AnimateBackground(Graphics* graphics) {
	CustomAnimate(graphics, background);
}

void Spikes::CustomAnimate(Graphics* graphics, Animation* anim) {
	if(on) {
		if (timer->GetTimeMilli() > miliUntilOff) {
			timer->StopWatch();
			Off();
		}
	}

	if (anim->getCurrentFrame() == 0) {
		anim->Freeze();
	}

	if (anim->getCurrentFrame() == anim->getLastFrameNr()) {
		anim->Freeze();
	}

	anim->Update(graphics, xPos, yPos);
}

void Spikes::Animate(Graphics* graphics) {}