#include "Game.h"
#include "Graphics.h"
#include "Level.h"

#define BORDERS 0

Game::Game(HWND hwnd, Input* in) : 
		graphics(hwnd),
		input(in),
		TOP(30),
		BOTTOM(56),
		LEFT(20),
		RIGHT(0) {

	level = new Level();
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

	LoadSprite(&rubble_front, L"Assets//rubble_back.png");
	LoadSprite(&rubble_back, L"Assets//rubble_front.png");

	LoadSprite(&tileCornerLeft, L"Assets//tile_corner_left.png");

	LoadSprite(&torch,L"Assets//torch.png");

	LoadSprite(&deadSk,L"Assets//deadSk.png");

	LoadSprite(&gate, L"Assets//gate.png");


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


	prince->getAnim()->setLoop(true);
	prince->getAnim()->Play();


	LoadFont(&fixedSys, fontSurf, "Assets//fixedsys16x28.bmp", 16, 28, 32);

}

void Game::GameLoop() {
	
	graphics.BeginFrame();
	
	//ControlAI
	DrawGraphics();


	HandleInput();
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
	int nBlockX = level->getBlockXByCoord(xFoot - LEFT);
	int nBlockY = level->getBlockYByCoord(yFoot - TOP);

	
	/* fall */
	if(level->getCodeByBlock(nBlockY, nBlockX) == '_' || 
	   level->getCodeByBlock(nBlockY, nBlockX) == '#' ||
	   level->getCodeByBlock(nBlockY, nBlockX) == '^' ||
	   level->getCodeByBlock(nBlockY, nBlockX) == '$' ||
	   level->getCodeByBlock(nBlockY, nBlockX) == '_' ) {
		int bar = (TOP + LEVEL_HEIGHT_PIX * nBlockY);

		if(DEBUG) graphics.DrawLine(0, bar, 1200, bar, 255, 255, 255);
	

		if(yFoot < bar - 20) {
			mY += prince->setFall();
		} else {
			prince->getAnim()->Play();
		}
	}


	if(mX < 0 && level->getCodeByBlock(nBlockY, nBlockX) == ']') {
		int bar = (LEFT + LEVEL_WIDTH_PIX * (nBlockX));

		//graphics.DrawLine(0, bar, 1200, bar, 255, 255, 255);

		if(xFoot + mX < bar) {
			mX = 0;
		}
	}

	if(mX > 0 && level->getCodeByBlock(nBlockY,nBlockX + 1) == '[' ||
				 level->getCodeByBlock(nBlockY,nBlockX) == '[' ) {
		int bar = (LEFT + LEVEL_WIDTH_PIX * (nBlockX) - 27);

		//graphics.DrawLine(bar, yFoot, bar, 200, 255, 255, 255);

		if(xFoot + mX > bar ) {
			mX = 0;
		}
	}

	if( level->getCodeByBlock(nBlockY, nBlockX) == ' ' || 
		level->getCodeByBlock(nBlockY, nBlockX) == '*' ) {
		mY += prince->setFall();
	}


	//Change Scenes
	if(yFoot - 120 / 2 > TOP + LEVEL_HEIGHT_PIX * 3) {
		level->changeScene(D);
		prince->setY(TOP - 120 / 2);
	}

	if(yFoot < TOP) {
		level->changeScene(U);
		prince->setY(TOP + LEVEL_HEIGHT_PIX * 3);
	}

	if(xFoot + 60 > LEFT + LEVEL_WIDTH_PIX * 10 + 5) {
		level->changeScene(R);
		prince->setX(LEFT - 30);
	}

	if(xFoot < LEFT - 5) {
		level->changeScene(L);
		prince->setX(LEFT + LEVEL_WIDTH_PIX * 9 - 30);
	}


	//move prince
	prince->MoveX(mX);
	prince->MoveY(mY);
}
void Game::HandleInput() {
	
	if(input->hasBeenPressed('A')) {
		level->changeScene(L);
	} 

	if(input->hasBeenPressed('D')) {
		level->changeScene(R);
	}

	if(input->hasBeenPressed('S')) {
		level->changeScene(D);
	} 

	if(input->hasBeenPressed('W')) {
		level->changeScene(U);
	}


	if(input->isShiftPressed()) {

		std::string s = "Code: ";
		std::string g(1, level->getCodeByCoord(prince->getMidX(), prince->getMidY()));
		s += g + "\n";
		OutputDebugStringA(s.c_str());



		if(level->getCodeByCoord(prince->getMidX(), prince->getMidY()) == 'P') {
			
			
			if(prince->Drink()) {
				level->setCodeByCoord(prince->getMidX(), prince->getMidY(), '_');
				prince->Heal();
			}
		}
	}

	prince->HandlePrince(input);
}
void Game::DrawGraphics() {

	//graphics.BeginFrame();

	ComposeFrame();

	//graphics.EndFrame();

}


//util
void Game::ComposeFrame() {
	DrawBackground();

	if(DEBUG) { graphics.DrawCircle(prince->getMidX(), prince->getMidY(), 5, 255, 255, 255);}

	prince->Animate(&graphics);

	DrawForeground();

	DrawHealth();
}
void Game::DrawHealth() {

	for(int i = 0; i < prince->getHealth(); i++ ) {
		graphics.DrawSprite(LEFT + i * healthFull.width, TOP + LEVEL_HEIGHT_PIX * 3, &healthFull );
	}

	for(int i = prince->getHealth(); i < prince->getMaxHealth(); i++ ) {
		graphics.DrawSprite(LEFT + i * healthFull.width, TOP + LEVEL_HEIGHT_PIX * 3, &healthEmpty );
	}
}
void Game::DrawBackground() {


	int yOff;
	int xOff;

	for(int i = 3; i >= 0; i--){
		for(int j = 0; j < 11; j++){

			yOff = TOP + LEVEL_HEIGHT_PIX * i;
			xOff = - LEVEL_WIDTH_PIX + LEFT + LEVEL_WIDTH_PIX * j;

			switch(level->getCodeByBlock(i, j)) {

			case 'I':
				//this is fine
				graphics.DrawSprite(xOff, yOff - block.height, &block);
				break;
			case 'T':
				graphics.DrawSprite(xOff, yOff - tileCornerLeft.height, &tileCornerLeft);
				graphics.DrawSprite(xOff, yOff - columnBack.height - 15, &columnBack);
				break;
			case ']':
				//this is fine
				graphics.DrawSprite(xOff, yOff - blockCornerRight.height, &blockCornerRight);
				break;
			case '_':
				//this is fine
				graphics.DrawSprite(xOff, yOff - tileCornerLeft.height, &tileCornerLeft);
				break;
			case 'S':
				graphics.DrawSprite(xOff, yOff - deadSk.height, &deadSk);
				break;
			case '^':
				graphics.DrawSprite(xOff, yOff - tileCornerLeft.height, &tileCornerLeft);
				break;
			case 'G':
				graphics.DrawSprite(xOff, yOff - gate.height, &gate);
				break;
			case '#':
				graphics.DrawSprite(xOff, yOff - tileCornerLeft.height, &tileCornerLeft);
				graphics.DrawSprite(xOff, yOff - bricks.height, &bricks);
				break;
			case 'P':
				graphics.DrawSprite(xOff, yOff - tileCornerLeft.height, &tileCornerLeft);
				break;
			case '$':
				graphics.DrawSprite(xOff, yOff - tileCornerLeft.height, &tileCornerLeft);
				graphics.DrawSprite(xOff, yOff - rubble_front.height, &rubble_front);
				break;
			case '*':
				graphics.DrawSprite(xOff, yOff - bricks.height, &bricks);
			case ' ':
				break;

			}
		}
	}

	std::list<Entity>* torches = level->getTorchEntities();
	for(std::list<Entity>::iterator i = torches->begin(); i != torches->end(); i++) {
		i->Animate(&graphics);	
	}
}
void Game::DrawForeground() {
	int yOff;
	int xOff;

	for(int i = 3; i >= 0; i--){
		for(int j = 0; j < 11; j++){

			yOff = TOP + LEVEL_HEIGHT_PIX * i;
			xOff = - LEVEL_WIDTH_PIX + LEFT + LEVEL_WIDTH_PIX * j;

			switch(level->getCodeByBlock(i, j)) {
			case '$':
				graphics.DrawSprite(xOff, yOff - rubble_back.height, &rubble_back);
				break;
			case 'T':
				graphics.DrawSprite(xOff, yOff - columnFront.height, &columnFront);
				break;
			case '[':
				//this is fine
				graphics.DrawSprite(xOff, yOff - blockCornerLeft.height, &blockCornerLeft);
				break;
			}
		}
	}

	std::list<Entity>* potions = level->getPotionEntities();
	for(std::list<Entity>::iterator i = potions->begin(); i != potions->end(); i++) {
		i->Animate(&graphics);	
	}
}

Game::~Game() {}





/* Debug information 
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

*/


/*
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
*/