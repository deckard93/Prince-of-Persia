#include "Gate.h"
#include "Game.h"
#include "FinishDoor.h"

FinishDoor::FinishDoor(int x, int y, int levelX, int levelY) : Gate(x, y, levelX, levelY) 
{
    type = doorT;

    setX(x);
    setY(y);

    setLevelX(levelX);
    setLevelY(levelY);

    //gate->Stop();
    //gate->setReverse();
    //gate->setDisplayTime(slowTime);
    //gate->setCurrentFrame(gate->getLastFrameNr());
    //gate->Play();

    state = sClosed;
    timer = new Timer();
    

    door = Game::getSprite("finishDoor");
    doorBase = Game::getSprite("finishDoorBase");
    enter = new Animation(Game::getSprite("enterDoor"), 11);
    enter->setDisplayTime(80);


    yCut = 2;
    cutLimit = 88;
    frameTime = 40;

    doorOffsetX = 33;
    doorOffsetY = 30;

    setCurrentAnim(NULL);
}

void FinishDoor::Animate(Graphics* graphics, Audio* audio) 
{
    if (graphics == NULL) { return; }

    if (this->getAnim() != NULL) {
        if (this->getAnim()->isFinished()) {
            this->setCurrentAnim(NULL);
        }
        else {
            Entity::Animate(graphics);
            return;
        }
    }

    if (state == sOpening) {
        if (timer->GetTimeMilli() > frameTime) {
            yCut += 1;
            timer->StopWatch();
            timer->StartWatch();
        }

        if (yCut == cutLimit) {
            state = sOpened;
            timer->StopWatch();
        }
    }

    graphics->DrawSprite(getX(), getY(), doorBase);
    graphics->DrawSprite(getX() + doorOffsetX, getY() + doorOffsetY, door, 0, 0 + yCut, door->width, door->height - yCut);
}

void FinishDoor::Open() 
{
    if (state == sClosed) {
        state = sOpening;
        timer->StartWatch();
    }
}

bool FinishDoor::Finish() 
{
    if (state == sOpened) {
        setCurrentAnim(enter);
        return true;
    }
    return false;
}
