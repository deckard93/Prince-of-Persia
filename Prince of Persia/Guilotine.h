#pragma once

enum guilotineState { sChomp, sSafe };

class Guilotine : public Entity
{

private:
    guilotineState state;


public:
    Guilotine(int x, int y);
    ~Guilotine();
};