#pragma once

#pragma comment(lib, "D3dx9.lib")
#pragma comment(lib, "gdiplus.lib")

#include <windows.h>
#include <d3d9.h>
#include <math.h>
#include <assert.h>
#include <GdiPlus.h>
#include <string>

#include "Font.h"
#include "Sprite.h"
#include "SpriteSheet.h"
#include "Bitmap.h"

using std::string;

Sprite* LoadSprite(const WCHAR* name);
void LoadSprite(Sprite* sprite, const WCHAR* name);
void FreeSprite( Sprite* sprite );
void LoadFont(Font* font, D3DCOLOR* surface, const char* filename,
    unsigned int charWidth, unsigned int charHeight, int nCharsPerRow);

class SpriteSheet;

class Graphics {

public:
    //Constructor
    Graphics(HWND hwnd);
    
    //Control
    void BeginFrame();
    void EndFrame();

    //Pixel Calls
    void PutPixel(int x, int y, D3DCOLOR color);
    void PutPixel(int x, int y, int r, int g, int b);
    
    //Shape Drawing
    void DrawLine( int x1,int y1,int x2,int y2,int r,int g,int b );
    void DrawCircle( int cx,int cy,int radius,int r,int g,int b );
    void DrawDisc( int cx,int cy,int r,int rd,int g,int b );
    void DrawBlock(int x, int y, int width, int height, D3DCOLOR color);
    
    //Text Drawing
    void DrawChar( char c,int x,int y,Font* font,D3DCOLOR color );
    void DrawString( const char* string,int x,int y,Font* font,D3DCOLOR color );

    //Sprite Drawing
    void DrawSprite( int x,int y,Sprite* sprite );
    void DrawSprite( int xoff,int yoff,Sprite* sprite, int fromX, int fromY, int sWidth, int sHeight );
    void DrawFlippedSprite( int xoff,int yoff,Sprite* sprite );
    void DrawFlippedSprite( int xoff,int yoff,Sprite* sprite, int fromX, int fromY, int sWidth, int sHeight );
    void DrawSpriteAlpha( int xoff,int yoff,Sprite* sprite );
    void DrawFrame(int x, int y, SpriteSheet* sheet, int index, bool flipped);

    //Destructor
    ~Graphics();

private:
    IDirect3D9*            pD3DObject;
    IDirect3DDevice9*    pDevice;
    IDirect3DSurface9*    pBackBuffer;
    D3DLOCKED_RECT        backRect;

public:

    static const int SCREENX = 1200;
    static const int SCREENY = 675;

    //static const int SCREENX = 640;
    //static const int SCREENY = 480;

    static const int OFFSET_Y = (SCREENY - 126 * 3) / 2;
    static const int OFFSET_X = (SCREENX - 64 * 10) / 2;

    //static const int OFFSET_X = 0;
    //static const int OFFSET_Y = 0;
};