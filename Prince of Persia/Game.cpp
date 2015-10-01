#include "Game.h"
#include "Graphics.h"
#include "Level.h"




char scene[4][12] = {
	 {' ','_','_','_','_','_','_','_','_','_','_'},
	 {'_',' ',' ','  ','_','_','_','_','[','I','I'},
	 {']','_','_','_','T',' ','[','I','I','I','I'},
	 {' ','I','I','I',']','_','T','_','_','_','['},
	};

	
#define BORDERS 1
Game::Game(HWND hwnd, Input* in) : 
		graphics(hwnd),
		input(in),
		TOP(30),
		BOTTOM(56),
		LEFT(20),
		RIGHT(0) {

	level = new Level();
	level->setGFX(&graphics);
	level->loadLevel(1);

	TOP = (SCREEN_Y - LEVEL_HEIGHT_PIX * LEVEL_HEIGHT_BLOCK) / 2;
	LEFT = (SCREEN_X - LEVEL_WIDTH_PIX * LEVEL_WIDTH_BLOCK) / 2;

	int currentFrame;

	LoadSprite(&block, L"Assets//block.png");
	LoadSprite(&blockCornerLeft, L"Assets//block_corner_left.png");
	LoadSprite(&blockCornerRight, L"Assets//block_corner_right.png");

	LoadSprite(&bricks, L"Assets//bricks.png");
	LoadSprite(&torch, L"Assets//torch.png");

	LoadSprite(&separator, L"Assets//separator.png");
	LoadSprite(&separatorCorner, L"Assets//separator_corner.png");

	LoadSprite(&floor, L"Assets//floor.png");

	LoadSprite(&healthFull, L"Assets//health-full.png");
	LoadSprite(&healthEmpty, L"Assets//health-empty.png");

	LoadSprite(&columnFront, L"Assets//column_front.png");
	LoadSprite(&columnBack, L"Assets//column_back.png");

	LoadSprite(&tileCornerLeft, L"Assets//tile_corner_left.png");

	LoadSprite(&torch,L"Assets//torch.png");


	LoadSprite(&test, L"Assets//prince//climbUp.png");

	dummy = 0;

	int x = LEFT + 2 * LEVEL_WIDTH_PIX + TORCH_FLOAT_LEFT;
	int y = TOP - torch.height - TORCH_FLOAT + LEVEL_HEIGHT_PIX * 2;

	torchEntity = new Entity (new Animation(L"Assets//torch.png", 5), x, y );

	torchEntity->getAnim()->setDisplayTime(80.0);
	torchEntity->getAnim()->setLoop(true);
	torchEntity->getAnim()->Play();
	
	//torchSheet = new SpriteSheet(torch, 23);

	//load prince
	prince = new Prince();

	prince->setX(LEFT + 2 * LEVEL_WIDTH_PIX);
	prince->setY(TOP - prince->getAnim()->getSheet()->getSprite()->height + LEVEL_HEIGHT_PIX * 2 - FOOT_FLOAT);
	prince->setX(0);
	prince->setY(0);
	//prince->setY(SCREEN_Y/2);
	//prince->setX(SCREEN_X/2);

	prince->getAnim()->setLoop(true);
	prince->getAnim()->Play();


	LoadFont(&fixedSys, fontSurf, "Assets//fixedsys16x28.bmp", 16, 28, 32);

}

Game::~Game() {}

void Game::Go() {

	//HandleInput();
	graphics.BeginFrame();
	

	//ControlAI
	UpdateFrame();



	OutputDebugStringA("In input\n");


	if(input->hasABeenPressed()) {
		level->changeScene(L);
	} 

	if(input->hasDBeenPressed()) {
		level->changeScene(R);

	}

	if(input->hasSBeenPressed()) {
		level->changeScene(D);
	} 

	if(input->hasWBeenPressed()) {
		level->changeScene(U);

	}




	prince->HandlePrince(input);
	CheckCollision();

	
	

	graphics.EndFrame();
}


//control


void Game::CheckCollision() {


	//calculate foot position
	//int xFoot = prince->getX() + prince->getAnim()->getSheet()->getSprite()->width / 2 - 32;
	//int yFoot = prince->getY() + prince->getAnim()->getSheet()->getSprite()->height - 9;
	int xFoot = prince->getX() + 120 / 2 - 32;
	int yFoot = prince->getY() + 120 - 9;

	//will store displacement coords
	int mX = prince->getDefferX();
	int mY = prince->getDefferY();

	int x_increment = 10;

	/*======== Handle Input ==========*/
	if(input->isLeftPressed()) {
		//mX -= 2;
	}
	
	if(input->isRightPressed()) {
		//mX += 2;	
	}

	if(input->hasUpBeenPressed() || input->isUpPressed()) {
		//mY -= 2;  //-LEVEL_HEIGHT_PIX
	}

	if(input->hasDownBeenPressed() || input->isDownPressed()) {
		//mY += 2;  //LEVEL_HEIGHT_PIX
	}

	//determine where in the level we are
	int nBlockX = (xFoot - LEFT + LEVEL_WIDTH_PIX) / LEVEL_WIDTH_PIX;
	int nBlockY = (yFoot - TOP + LEVEL_HEIGHT_PIX) / LEVEL_HEIGHT_PIX;

	
	/* fall */
	if(scene[nBlockY][nBlockX] == '_' || scene[nBlockY][nBlockX] == 'T') {
		int bar = (TOP + LEVEL_HEIGHT_PIX * nBlockY);

		graphics.DrawLine(0, bar, 1200, bar, 255, 255, 255);

		if(yFoot < bar - 20) {
			mY += prince->setFall();
		} else {
			prince->getAnim()->Play();
		}

		
	}

	if(mX < 0 && scene[nBlockY][nBlockX] == ']') {
		int bar = (LEFT + LEVEL_WIDTH_PIX * (nBlockX));

		//graphics.DrawLine(0, bar, 1200, bar, 255, 255, 255);

		if(xFoot + mX < bar) {
			mX = 0;
		}
	}

	if(mX > 0 && scene[nBlockY][nBlockX] == '[') {
		int bar = (LEFT + LEVEL_WIDTH_PIX * (nBlockX - 1));

		//graphics.DrawLine(0, bar, 1200, bar, 255, 255, 255);

		if(xFoot + mX > bar) {
			mX = 0;
		}
	}

	if( scene[nBlockY][nBlockX] == ' ') {
		mY += prince->setFall();
	}


	//move prince
	prince->MoveX(mX);
	prince->MoveY(mY);


	/* Debug information */
	if(DEBUG) {
		std::string s;
		s = "P: " + std::to_string((long double)xFoot) + '.' + std::to_string((long double)yFoot);
		s += " | ";
		s += "B:" + std::to_string((long double)nBlockX) + '.' + std::to_string((long double)nBlockY);
		s += " | ";

		if(nBlockY >= 0 && nBlockX >= 0) {
			std::string g(1,scene[nBlockY][nBlockX - 1]);
			s += "M: " + g;
			std::string h(1,scene[nBlockY][nBlockX]);
			s += h;
			std::string j(1,scene[nBlockY][nBlockX + 1]);
			s += j;
		}
		s += "\n";

		OutputDebugStringA(s.c_str());
	}

}

void Game::HandleInput() {
	
	


	prince->HandlePrince(input);

}

void Game::UpdateFrame() {

	//graphics.BeginFrame();

	ComposeFrame();

	//graphics.EndFrame();

}


//util
void Game::ComposeFrame() {

	//DrawLevelManualy();
	DrawLevelBySchematic();
	//level->drawLevel();

	//graphics.DrawFlippedSprite(princeX, princeY, currentAnim);

	//graphics.DrawSprite(120, 0, &princeRunning, dummy, 0, 120, 120);

	//graphics.DrawFlippedSprite(princeX, princeY, &princeRunning, dummy, 0, 120, 120);
	
	//graphics.DrawSprite(princeX, princeY, &princeIdle);

	//graphics.DrawSprite(LEFT, princeY, &torch, dummy, 0, 23, 68);

	//graphics.DrawSprite(LEFT + 2 * LEVEL_WIDTH_PIX + TORCH_FLOAT_LEFT, TOP - torch.height - TORCH_FLOAT + LEVEL_HEIGHT_PIX * 2, &torch, dummy, 0, 23, 68);


	//torchEntity->getAnim()->getSheet()->getSprite();


	//graphics.DrawFrame(torchEntity->getX(), torchEntity->getY(), torchEntity->getAnim()->getSheet(), 3, false); 

	torchEntity->Animate(&graphics);
	
	
	graphics.DrawSprite(LEFT + LEVEL_WIDTH_PIX + TORCH_FLOAT_LEFT, TOP - torch.height - TORCH_FLOAT + LEVEL_HEIGHT_PIX * 2, &torch, dummy, 0, 23, 68);

	dummy += 23;

	if(dummy >= 23 * 5) { dummy = 0; }

	int p = SCREEN_Y;

	//Black Borders - till I fix the cut out sprites
	
	if(BORDERS) {
	graphics.DrawBlock(0, 0, SCREEN_X, TOP - 7, D3DCOLOR_XRGB(0,0,0));	//top
	graphics.DrawBlock(0, TOP - LEVEL_HEIGHT_PIX , LEFT, SCREEN_Y - TOP * 2 + LEVEL_HEIGHT_PIX, D3DCOLOR_XRGB(0,0,0)); //left
	
	graphics.DrawBlock(LEFT + LEVEL_WIDTH_PIX * LEVEL_WIDTH_BLOCK, TOP - 7, LEFT, SCREEN_Y - TOP * 2, D3DCOLOR_XRGB(0,0,0)); //right
	//graphics.DrawBlock(0, TOP + LEVEL_HEIGHT_PIX * LEVEL_HEIGHT_BLOCK, SCREEN_X, TOP, D3DCOLOR_XRGB(0,0,0));	//bottom
	}

	//health
	for(int i = 0; i < prince->getHealth(); i++ ) {
		
		graphics.DrawSprite(LEFT + i * healthFull.width, TOP + LEVEL_HEIGHT_PIX * 3, &healthFull );

	}

	for(int i = prince->getHealth(); i < prince->getMaxHealth(); i++ ) {
		
		graphics.DrawSprite(LEFT + i * healthFull.width, TOP + LEVEL_HEIGHT_PIX * 3, &healthEmpty );

	}

	prince->Animate(&graphics);
}

void Game::DrawLevelManualy() {
	int yOff;

	//level 1
	yOff = TOP + LEVEL_HEIGHT_PIX * 3;

		//blocks
		for(int i = 0; i< 3; i++) {
			graphics.DrawSprite(i  * LEVEL_WIDTH_PIX, yOff - block.height, &block);
		}
		graphics.DrawSprite(3 * LEVEL_WIDTH_PIX - 1, yOff - blockCornerRight.height + 2, &blockCornerRight);

		//separatrs
		for(int i = 0; i< 5; i++) {
			graphics.DrawSprite((4 + i) * LEVEL_WIDTH_PIX - 2, yOff - separator.height, &separator);
		}

		//floors
		for(int i = 0; i< 5; i++) {
			graphics.DrawSprite((4+ i) * LEVEL_WIDTH_PIX, yOff + 1 - separator.height - floor.height, &floor);
		}

		//columns
		graphics.DrawSprite(5 * LEVEL_WIDTH_PIX + 15, yOff - columnFront.height - separator.height + 1, &columnFront);
		graphics.DrawSprite(5 * LEVEL_WIDTH_PIX + 14 + columnFront.width, yOff - columnBack.height - separator.height - 7 + 1, &columnBack);

		//blocks
		graphics.DrawSprite(9 * LEVEL_WIDTH_PIX, yOff - blockCornerLeft.height, &blockCornerLeft);


	//level 2
	yOff = TOP + LEVEL_HEIGHT_PIX * 2;

		//blocks
		for(int i = 0; i < 4; i++) {
			graphics.DrawSprite(0 + (9 - i) * LEVEL_WIDTH_PIX, yOff - block.height, &block);
		}

		graphics.DrawSprite(0 + 5 * LEVEL_WIDTH_PIX, yOff - blockCornerLeft.height, &blockCornerLeft);
		graphics.DrawSprite(0  - LEVEL_WIDTH_PIX - 1, yOff - blockCornerRight.height, &blockCornerRight);

		//separators
		for(int i = 0; i< 3; i++) {
			graphics.DrawSprite(i * LEVEL_WIDTH_PIX - 2, yOff - separator.height, &separator);
		}
		graphics.DrawSprite(3 * LEVEL_WIDTH_PIX - 4, yOff - separatorCorner.height + 2, &separatorCorner);
		
		//floors
		for(int i = 0; i< 3; i++) {
			graphics.DrawSprite(i * LEVEL_WIDTH_PIX, yOff + 1 - separator.height - floor.height, &floor);
		}
		graphics.DrawSprite(3 * LEVEL_WIDTH_PIX, yOff + 1 - separator.height - floor.height, &floor);
		
		//columns
		graphics.DrawSprite(3 * LEVEL_WIDTH_PIX + 15, yOff - columnFront.height - separator.height + 1, &columnFront);
		graphics.DrawSprite(3 * LEVEL_WIDTH_PIX + 14 + columnFront.width, yOff - columnBack.height - separator.height - 7 + 1, &columnBack);


	//level 3
	yOff = TOP + LEVEL_HEIGHT_PIX;

		//separators
		graphics.DrawSprite(0  - LEVEL_WIDTH_PIX, yOff + 1 - separator.height - floor.height, &floor);
		graphics.DrawSprite(0  - LEVEL_WIDTH_PIX - 4, yOff - separatorCorner.height + 2, &separatorCorner);

		for(int i = 0; i< 5; i++) {
			graphics.DrawSprite(0 + (7 - i) * LEVEL_WIDTH_PIX, yOff - separator.height, &separator);
			
		}

		//floors
		for(int i = 0; i< 5; i++) {
			graphics.DrawSprite(1 + (i + 3) * LEVEL_WIDTH_PIX, yOff + 1 - separator.height - floor.height, &floor);
		}

		//blocks
		graphics.DrawSprite(0 + 9 * LEVEL_WIDTH_PIX, yOff - block.height, &block);
		graphics.DrawSprite(0 + 8 * LEVEL_WIDTH_PIX, yOff - blockCornerLeft.height, &blockCornerLeft);


	//Level 4

		//separators
		for(int i =0; i < LEVEL_WIDTH_BLOCK; i++) {
			graphics.DrawSprite(i * LEVEL_WIDTH_PIX, TOP - separator.height, &separator);
		}

}

void Game::DrawLevelBySchematic() {
	
	/*
	___________
	_   _____[I
	]___T [IIII
	 III]_T___[
	*/

	int yOff;
	int xOff;

	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 11; j++){

			yOff = TOP +  LEVEL_HEIGHT_PIX * i;
			xOff = - LEVEL_WIDTH_PIX + LEFT + LEVEL_WIDTH_PIX * j;

			switch(level->getCode(i, j)) {

			case 'I':
				//this is fine
				graphics.DrawSprite(xOff, yOff - block.height, &block);
				break;
			case 'T':
				graphics.DrawSprite(xOff, yOff - tileCornerLeft.height, &tileCornerLeft);
				graphics.DrawSprite(xOff, yOff - columnFront.height, &columnFront);
				graphics.DrawSprite(xOff, yOff - columnBack.height, &columnBack);
				break;
			case ']':
				//this is fine
				graphics.DrawSprite(xOff, yOff - blockCornerRight.height, &blockCornerRight);
				break;
			case '[':
				//this is fine
				graphics.DrawSprite(xOff, yOff - blockCornerLeft.height, &blockCornerLeft);
				break;
			case '_':
				//this is fine
				graphics.DrawSprite(xOff, yOff - tileCornerLeft.height, &tileCornerLeft);
				break;
			case ' ':
				break;

			}
		}
	}


	if(DEBUG) {
		for(int i = 0; i < 5; i++){
			for(int j = 0; j < 12; j++){

				yOff = TOP +  LEVEL_HEIGHT_PIX * i;
				xOff = - LEVEL_WIDTH_PIX + LEFT + LEVEL_WIDTH_PIX * j;
			
			
				std::string s = std::to_string((long double)j) + "." + std::to_string((long double)i);
				graphics.DrawString(s.c_str(), xOff, yOff - 25, &fixedSys, D3DCOLOR_XRGB(255,255,255 ));
		
			}
		}
	}






}