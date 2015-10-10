#pragma once

#include <d3d9.h>

struct Font {
	int charWidth;
	int charHeight;
	int nCharsPerRow;
	D3DCOLOR* surface;
};