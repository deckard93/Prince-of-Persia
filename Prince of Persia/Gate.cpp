#include "Game.h"
#include "Timer.h"
#include "Graphics.h"

#include "Gate.h"


Gate::Gate(int x, int y, int levelX, int levelY) {
    slowTime = 30;
    fastTime = 50;
    delay = 4000;

    type = gateT;

    setX(x);
    setY(y);

    setLevelX(levelX);
    setLevelY(levelY);

    gate = new Animation(Game::getSprite("gate"), 7);
    setCurrentAnim(gate);
    gate->Stop();

    gate->setReverse();
    gate->setDisplayTime(slowTime);
    gate->setCurrentFrame(gate->getLastFrameNr());

    //gate->Play();

    state = sClosed;
    timer = new Timer();
}

void Gate::Open() {

    if (state == sClosed || state == sClosing) {
        state = sOpening;

        slowTime = 1000;

        timer->StopWatch();
        timer->StartWatch();

        this->getAnim()->setReverse();
        this->getAnim()->setDisplayTime(fastTime);
        this->getAnim()->Play();

    }
    else if (state == sOpened) {
        timer->StopWatch();
        timer->StartWatch();
    }
}

void Gate::Close() {
    if (state == sOpened || state == sClosing) {
        state = sClosing;

        this->getAnim()->setForward();
        this->getAnim()->setDisplayTime(fastTime);
        this->getAnim()->Play();
    }
}

void Gate::Animate(Graphics* graphics, Audio* audio) {

    if (getAnim() == NULL) { //TODO shouldn't happen
        return;
    }

    if (getAnim()->getCurrentFrame() == 0 && state == sOpening) {
        getAnim()->Stop();
        state = sOpened;
    }

    if (getAnim()->getCurrentFrame() == getAnim()->getLastFrameNr() && state == sClosing) {
        getAnim()->Stop();
        state = sClosed;
        audio->PlaySound(Audio::gate_close);
    }

    if (state == sOpened) {
        if (timer->GetTimeMilli() > delay) {
            state = sClosing;

            this->getAnim()->setForward();
            this->getAnim()->setDisplayTime(slowTime);
            this->getAnim()->Play();
        }
    }

    if (state == sClosing && getAnim()->isEffectPending()) {
        audio->PlaySound(Audio::gate_down);
        getAnim()->setEffectDone();
    }

    if (state == sOpening && getAnim()->isEffectPending()) {
        audio->PlaySound(Audio::gate_up);
        getAnim()->setEffectDone();
    }

    Entity::Animate(graphics);
}

bool Gate::isOpen() {
    if (state == sOpened) {
        return true;
    }
    else if (state == sClosing) {
        if (this->currentAnim->getCurrentFrame() <= 1) {
            return true;
        }
    }
    return false;
}