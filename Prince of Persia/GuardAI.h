#pragma once

#include "Guard.h"

class GuardAI : public Guard 
{
// Variables
private:
    int chanceToParry;
    int chanceToStrike;
    bool engaged;

// Functions
public:
    GuardAI(Audio* audio, int x, int y);
    void Control(Prince & prince, Level & level);
};