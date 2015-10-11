#include "Graphics.h"


//Util
void LoadSprite(Sprite* sprite, const WCHAR* name) {

	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	Gdiplus::Bitmap bitmap(name);
	Gdiplus::Color pixel;

	assert(bitmap.GetHeight() >= 0);

	sprite->height = bitmap.GetHeight();
	sprite->width = bitmap.GetWidth();
	sprite->key = 0x0010202;
	sprite->surface = (D3DCOLOR*) malloc(sizeof(D3DCOLOR) * sprite-> height * sprite-> width);

	int bitHeight = bitmap.GetHeight();
	int bitWidth = bitmap.GetWidth();

	for( int y = 0; y < bitHeight; y++) {
		for( int x = 0; x < bitWidth; x++)	{
			//OutputDebugStringW(L"\n");

			bitmap.GetPixel(x,y, &pixel);
			if(pixel.GetAlpha() >= 0.9999) {
			sprite->surface[x + y * bitWidth] = 
				D3DCOLOR_XRGB(pixel.GetR(), pixel.GetG(), pixel.GetB());
				//D3DCOLOR_XRGB(0, 0, 0);
			} else {
				sprite->surface[x + y * bitWidth] = 
				D3DCOLOR_XRGB(100, 100, 100);
			}
		}
	}
}
void LoadFont(Font* font, D3DCOLOR* surface, const char* filename,
unsigned int charWidth, unsigned int charHeight, int nCharsPerRow){

		LoadBmp(filename, surface);
		font->charHeight = charHeight;
		font->charWidth = charWidth;
		font->nCharsPerRow = nCharsPerRow;
		font->surface = surface;

}

Sprite* LoadSprite(const WCHAR* name) {

	Sprite* sprite = new Sprite();

	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	Gdiplus::Bitmap bitmap(name);
	Gdiplus::Color pixel;

	assert(bitmap.GetHeight() >= 0);

	sprite->height = bitmap.GetHeight();
	sprite->width = bitmap.GetWidth();
	sprite->key = 0x0010202;
	sprite->surface = (D3DCOLOR*) malloc(sizeof(D3DCOLOR) * sprite-> height * sprite-> width);

	for( int y = 0; y < bitmap.GetHeight(); y++) {
		for( int x = 0; x < bitmap.GetWidth(); x++)	{

			bitmap.GetPixel(x,y, &pixel);
			if(pixel.GetAlpha() != 0.0) {
			sprite->surface[x + y * bitmap.GetWidth()] = 
				D3DCOLOR_XRGB(pixel.GetR(), pixel.GetG(), pixel.GetB());
			} else {
				sprite->surface[x + y * bitmap.GetWidth()] = 
				D3DCOLOR_XRGB(100, 100, 100);
			}
		}
	}
	return sprite;
}
void FreeSprite( Sprite* sprite )
{
	free( sprite->surface );
}


//Constructor
Graphics::Graphics(HWND windowHandle) {
	//There is something silly float
	HRESULT result;

	backRect.pBits = NULL;

	pD3DObject = Direct3DCreate9(D3D_SDK_VERSION);
	assert(pD3DObject != NULL);

	D3DPRESENT_PARAMETERS d3dpresent;
	memset(&d3dpresent, 0, sizeof(d3dpresent));

	d3dpresent.Windowed = TRUE;
	d3dpresent.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpresent.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpresent.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	d3dpresent.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;



	//Create device 
	result = pD3DObject->CreateDevice(
		D3DADAPTER_DEFAULT, 
		D3DDEVTYPE_HAL, 
		windowHandle, 
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE, 
		&d3dpresent, 
		&pDevice
	); assert (!FAILED(result));
	
	//Get a pointer to the backbuffer 
	//so we can write to it later
	result = pDevice->GetBackBuffer(
		0,
		0,
		D3DBACKBUFFER_TYPE_MONO,
		&pBackBuffer
	); assert(!FAILED(result));
}

//Control
void Graphics::BeginFrame() {
	HRESULT result;

	//clear the backbuffer to prepare
	//for a new scene
	result = pDevice->Clear(
		0,
		NULL,
		D3DCLEAR_TARGET,
		D3DCOLOR_XRGB(0,0,0),
		0.0f,
		0
	); assert (!FAILED(result));

	//prepare a surface to draw to ?
	result = pBackBuffer->LockRect(&backRect, NULL, NULL);
	assert(!FAILED(result));
}
void Graphics::EndFrame() {
	HRESULT result;

	result = pBackBuffer-> UnlockRect();
	assert(!FAILED(result));

	result = pDevice->Present(NULL, NULL, NULL, NULL);
	assert(!FAILED(result));
}

//Pixel Calls
void Graphics::PutPixel(int x, int y, D3DCOLOR color) {

	assert(x >= 0);
	assert(y >= 0);
	assert(x <= SCREEN_X);
	assert(y <= SCREEN_Y);

	int left = (1200 - 64 * 10) / 2;
	int top = (675 - 126 * 3) / 2 - 7;
	int right = left + 64 * 10 - 1;
	int bottom = top + 126 * 3 + 7 + 100;
	
	if(x < left) return;
	if(x > right) return;
	if(y < top) return;
	if(y > bottom) return;

	((D3DCOLOR*) backRect.pBits)[x + (backRect.Pitch >> 2) * y] = color;
}
void Graphics::PutPixel(int x, int y, int r, int g, int b) {

	assert(x >= 0);
	assert(y >= 0);
	assert(x <= SCREEN_X);
	assert(y <= SCREEN_Y);

	((D3DCOLOR*) backRect.pBits)[x + (backRect.Pitch >> 2) * y] = D3DCOLOR_XRGB(r, g, b);

}

//Shape Drawing
void Graphics::DrawDisc( int cx,int cy,int r,int rd,int g,int b )
{
	for( int x = cx - r; x < cx + r; x++ )
	{
		for( int y = cy - r; y < cy + r; y++ )
		{
			if( sqrt( (float)( (x - cx)*(x - cx) + (y - cy)*(y - cy) ) ) < r )
			{
				PutPixel( x,y,rd,g,b );
			}
		}
	}
}
void Graphics::DrawLine( int x1,int y1,int x2,int y2,int r,int g,int blu )
{
	int dx = x2 - x1;
	int dy = y2 - y1;

	if( dy == 0 && dx == 0 )
	{
		PutPixel( x1,y1,r,g,blu );
	}
	else if( abs( dy ) > abs( dx ) )
	{
		if( dy < 0 )
		{
			int temp = x1;
			x1 = x2;
			x2 = temp;
			temp = y1;
			y1 = y2;
			y2 = temp;
		}
		float m = (float)dx / (float)dy;
		float b = x1 - m*y1;
		for( int y = y1; y <= y2; y = y + 1 )
		{
			int x = (int)(m*y + b + 0.5f);
			PutPixel( x,y,r,g,blu );
		}
	}
	else
	{
		if( dx < 0 )
		{
			int temp = x1;
			x1 = x2;
			x2 = temp;
			temp = y1;
			y1 = y2;
			y2 = temp;
		}
		float m = (float)dy / (float)dx;
		float b = y1 - m*x1;
		for( int x = x1; x <= x2; x = x + 1 )
		{
			int y = (int)(m*x + b + 0.5f);
			PutPixel( x,y,r,g,blu );
		}
	}
}
void Graphics::DrawCircle( int centerX,int centerY,int radius,int r,int g,int b )
{
	int rSquared = radius*radius;
	int xPivot = (int)(radius * 0.707107f + 0.5f);
	for( int x = 0; x <= xPivot; x++ )
	{
		int y = (int)(sqrt( (float)( rSquared - x*x ) ) + 0.5f);
		PutPixel( centerX + x,centerY + y,r,g,b );
		PutPixel( centerX - x,centerY + y,r,g,b );
		PutPixel( centerX + x,centerY - y,r,g,b );
		PutPixel( centerX - x,centerY - y,r,g,b );
		PutPixel( centerX + y,centerY + x,r,g,b );
		PutPixel( centerX - y,centerY + x,r,g,b );
		PutPixel( centerX + y,centerY - x,r,g,b );
		PutPixel( centerX - y,centerY - x,r,g,b );
	}
}
void Graphics::DrawBlock(int x, int y, int width, int height, D3DCOLOR color) {

	for(int i = 0; i < width; i++ ) {
		for(int j = 0; j < height; j++) {
			PutPixel(i + x, j + y, color);
		}
	}
}

//Text Drawing
void Graphics::DrawChar( char c,int xoff,int yoff,Font* font,D3DCOLOR color )
{
	if( c < ' ' || c > '~' )
		return;

	const int sheetIndex = c - ' ';
	const int sheetCol = sheetIndex % font->nCharsPerRow;
	const int sheetRow = sheetIndex / font->nCharsPerRow;
	const int xStart = sheetCol * font->charWidth;
	const int yStart = sheetRow * font->charHeight;
	const int xEnd = xStart + font->charWidth;
	const int yEnd = yStart + font->charHeight;
	const int surfWidth = font->charWidth * font->nCharsPerRow;

	for( int y = yStart; y < yEnd; y++ )
	{
		for( int x = xStart; x < xEnd; x++ )
		{
			if( font->surface[ x + y * surfWidth ] == D3DCOLOR_XRGB( 0,0,0 ) )
			{
				PutPixel( x + xoff - xStart,y + yoff - yStart,color );
			}
		}
	}
}
void Graphics::DrawString( const char* string,int xoff,int yoff,Font* font,D3DCOLOR color )
{
	for( int index = 0; string[ index ] != '\0'; index++ )
	{
		DrawChar( string[ index ],xoff + index * font->charWidth,yoff,font,color );
	}
}

//Sprite Drawing
void Graphics::DrawSprite( int xoff,int yoff,Sprite* sprite )
{
	for( int y = 0; y < sprite->height; y++ )
	{
		for( int x = 0; x < sprite->width; x++ )
		{
			D3DCOLOR c = sprite->surface[ x + y * sprite->width ];
			if( c != D3DCOLOR_XRGB(100,100,100) )
			{
				if(x + xoff > 0 && x + xoff < SCREEN_X && 
					y + yoff > 0 && y + yoff < SCREEN_Y )
				PutPixel( x + xoff,y + yoff,c );
			}
		}
	}
}
void Graphics::DrawSprite( int xoff,int yoff,Sprite* sprite, int fromX, int fromY, int sWidth, int sHeight )
{
	
	for( int y = fromY; y < fromY + sHeight; y++ )
	{
		for( int x = fromX; x < fromX + sWidth; x++ )
		{
			D3DCOLOR c = sprite->surface[ x + y * sprite->width ];
			if( c != D3DCOLOR_XRGB(100,100,100) )
			{
				int xF = x - fromX + xoff;
				int yF = y - fromY + yoff;
				if( xF > 0 && xF < SCREEN_X && 
					yF > 0 && yF < SCREEN_Y )
				PutPixel( xF, yF,c );
			}
		}
	}
}
void Graphics::DrawFlippedSprite( int xoff,int yoff,Sprite* sprite, int fromX, int fromY, int sWidth, int sHeight )
{

		
	for( int y = 0; y < sHeight; y++ )
	{
		for( int x = 0; x < sWidth; x++ )
		{
			D3DCOLOR c = sprite->surface[ x + fromX + (y + fromY)* sprite->width ];
			if( c != D3DCOLOR_XRGB(100,100,100) )
			{
				int xF = sWidth - x + xoff;
				int yF = y + yoff;
				if( xF > 0 && xF < SCREEN_X && 
					 yF > 0 && yF < SCREEN_Y )
				PutPixel(xF, yF,c );
			}
		}
	}
}
void Graphics::DrawFlippedSprite( int xoff,int yoff,Sprite* sprite )
{
	for( int y = 0; y < sprite->height; y++ )
	{
		for( int x = 0; x < sprite->width; x++ )
		{
			D3DCOLOR c = sprite->surface[ x + y * sprite->width ];
			if( c != D3DCOLOR_XRGB(100,100,100) )
			{
				if(x + xoff > 0 && x + xoff < SCREEN_X && 
					y + yoff > 0 && y + yoff < SCREEN_Y )
					PutPixel(xoff + sprite->width - x,y + yoff,c );
			}
		}
	}
}
void Graphics::DrawFrame(int xoff, int yoff, SpriteSheet* sheet, int index, bool flipped) {

	assert(index >= 0);
	assert(index < sheet->getFrameCount());

	int fromX = index * sheet->getFrameWidth();
	int fromY = 0;

	Sprite* s = sheet->getSprite();
	int sWidth = sheet->getFrameWidth();
	int sHeight = s->height;

	if(flipped) {
		DrawFlippedSprite(xoff, yoff, sheet->getSprite(), fromX, fromY, sWidth, sHeight );
	} else {
		DrawSprite(xoff, yoff, sheet->getSprite(), fromX, fromY, sWidth, sHeight );
	}
}

//Destructor
Graphics::~Graphics() {
	if(pDevice) {
		pDevice->Release();
		pDevice = NULL;
	}

	if(pD3DObject) {
		pD3DObject->Release();
		pD3DObject = NULL;
	}

	if(pBackBuffer) {
		pBackBuffer->Release();
		pBackBuffer = NULL;
	}
}