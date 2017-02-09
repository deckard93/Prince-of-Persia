#pragma once

#include <d3d9.h>

class Sprite
{
public:
    int width;
    int height;
    D3DCOLOR key;
    D3DCOLOR* surface;
};