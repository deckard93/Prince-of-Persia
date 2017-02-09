#pragma once

#include "Entity.h"
#include "Audio.h"

enum gateState { sOpened, sClosed, sClosing, sOpening };

class Gate : public Entity {
//methods
public:
    int slowTime;
    int fastTime;
    int delay;

    Gate(int x, int y, int levelX, int levelY);
    ~Gate() {}
    void Open();
    void Close();
    bool isOpen();
    void Animate(Graphics* graphics, Audio* audio);

//attributes
protected:
    gateState state;
    Timer* timer;
    Animation* gate;
};