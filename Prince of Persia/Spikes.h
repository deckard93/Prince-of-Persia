#pragma once

#include "Entity.h"


class Spikes : public Entity 
{
protected:
    Animation* background;
    Animation* foreground;

private:
    bool on;
    Timer* timer;
    int miliUntilOff;

public:
    Spikes(int x, int y, int levelX, int levelY);
    ~Spikes();

public:
    void On(Audio* audio);
    void Off();
    void Kill();

    void Animate(Graphics* graphics);
    void CustomAnimate(Graphics* graphics, Animation* anim);
    void AnimateForeground(Graphics* graphics);
    void AnimateBackground(Graphics* graphics);

};