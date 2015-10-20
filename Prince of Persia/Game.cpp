#include "Game.h"
#include "Graphics.h"
#include "Level.h"

#define BORDERS 0


hash_map<string, Sprite*>* Game::assets = new hash_map<string, Sprite*>();


Game::Game(HWND hwnd, Input* in) : 
		graphics(hwnd),
		input(in) {

	LoadFont(&fixedSys, fontSurf, "Assets//fixedsys16x28.bmp", 16, 28, 32);

	//assets
	RegisterSprite("block");
	RegisterSprite("blockCornerLeft");
	RegisterSprite("blockCornerRight");
	RegisterSprite("bricks");
	RegisterSprite("separator");
	RegisterSprite("separator_corner");
	RegisterSprite("floor");
	RegisterSprite("trap");
	RegisterSprite("activate");
	RegisterSprite("healthFull");
	RegisterSprite("healthEmpty");
	RegisterSprite("columnFront");
	RegisterSprite("columnBack");
	RegisterSprite("rubbleBack");
	RegisterSprite("rubbleFront");
	RegisterSprite("doorFrameBack");
	RegisterSprite("doorFrameFront");
	RegisterSprite("tileCornerLeft");
	RegisterSprite("deadSk");
	RegisterSprite("gate");
	RegisterSprite("holyFloor");

	//entities
	RegisterSprite("spikes");
	RegisterSprite("torch");
	RegisterSprite("healthPotion");
	RegisterSprite("guilotine");
	RegisterSprite("gate");
	RegisterSprite("sword");
	
	//sprite sheets
	RegisterSprite("idle"       , "Assets//prince//");
	RegisterSprite("turn"       , "Assets//prince//");
	RegisterSprite("running"    , "Assets//prince//");
	RegisterSprite("climbUp"    , "Assets//prince//");
	RegisterSprite("jumpGrab"   , "Assets//prince//");
	RegisterSprite("crouch"     , "Assets//prince//");
	RegisterSprite("staticJump" , "Assets//prince//");
	RegisterSprite("step"       , "Assets//prince//");
	RegisterSprite("hang"       , "Assets//prince//");
	RegisterSprite("runningJump", "Assets//prince//");
	RegisterSprite("runningTurn", "Assets//prince//");
	RegisterSprite("fall"       , "Assets//prince//");
	RegisterSprite("drop"       , "Assets//prince//");
	RegisterSprite("drink"      , "Assets//prince//");
	RegisterSprite("pickSword"  , "Assets//prince//");

	//fight sheets
	RegisterSprite("fightIdle"  , "Assets//prince//");
	RegisterSprite("fightStep"  , "Assets//prince//");
	RegisterSprite("fightParry" , "Assets//prince//");
	RegisterSprite("fightStart" , "Assets//prince//");
	RegisterSprite("fightStrike", "Assets//prince//");
	RegisterSprite("fightFinish", "Assets//prince//");
	


	//objects
	level = new Level();
	prince = new Prince();

	level->loadLevel(1);

	prince->setX(2 * Level::BLOCK_WIDTH_PX);
	prince->setY(prince->getAnim()->getSheet()->getFrameHeight() - Level::FOOT_FLOAT);

	prince->getAnim()->setLoop(true);
	prince->getAnim()->Play();

}

void Game::GameLoop() {	
	graphics.BeginFrame();
	
	//ControlAI
	DrawGraphics();

	HandleInput();
	CheckCollision();

	graphics.EndFrame();
}

void Game::RegisterSprite(string name, string path) {
	Sprite * sprite = new Sprite();
	std::wstring item = std::wstring(path.begin(), path.end()) + std::wstring(name.begin(), name.end()) + L".png";
	LoadSprite(sprite, item.c_str());
	assets->insert(make_pair(name, sprite));
}
Sprite* Game::getSprite(string name) {
	return (*assets)[name];
}

//control
void Game::CheckCollision() {

	//calculate foot position
	int xFoot = prince->getX() + prince->getAnim()->getSheet()->getFrameWidth() / 2 - 36;
	int yFoot = prince->getY() + prince->getAnim()->getSheet()->getFrameHeight() - 9;

	if (DEBUG) { graphics.DrawCircle(xFoot, yFoot, 5, 255, 255, 255); }

	int mX = prince->getDefferX();
	int mY = prince->getDefferY();

	int nBlockX = level->getBlockXByCoord(xFoot);
	int nBlockY = level->getBlockYByCoord(yFoot);

	
	//===================== Fall ==========================
	if(level->getCodeByBlock(nBlockY, nBlockX) == '_' || 
	   level->getCodeByBlock(nBlockY, nBlockX) == '#' ||
	   level->getCodeByBlock(nBlockY, nBlockX) == '/' ||
	   level->getCodeByBlock(nBlockY, nBlockX) == '^' ||
	   level->getCodeByBlock(nBlockY, nBlockX) == '$' ||
	   level->getCodeByBlock(nBlockY, nBlockX) == '|' ||
	   level->getCodeByBlock(nBlockY, nBlockX) == '_' ) {

		int bar = (Level::BLOCK_HEIGHT_PX * nBlockY);
		if(DEBUG) graphics.DrawLine(0, bar, Graphics::SCREENX, bar, 255, 255, 255);
	
		if(yFoot < bar - 20) {
			mY += prince->setFall();
		} else {
			prince->getAnim()->Play();
		}
	}
	if (level->getCodeByBlock(nBlockY, nBlockX) == ' ' ||
		level->getCodeByBlock(nBlockY, nBlockX) == '*') {
		mY += prince->setFall();
	}


	//=============== Collision With Walls =====================
	if(mX < 0 && level->getCodeByBlock(nBlockY, nBlockX) == ']') {
		int bar = (Level::BLOCK_WIDTH_PX * (nBlockX));

		if (DEBUG) graphics.DrawLine(0, bar, Graphics::SCREENX, bar, 255, 255, 255);

		if(xFoot + mX < bar) {
			mX = 0;
		}
	}

	if(mX > 0 && level->getCodeByBlock(nBlockY,nBlockX + 1) == '[' ||
				 level->getCodeByBlock(nBlockY,nBlockX) == '[' ) {

		int bar = (Level::BLOCK_WIDTH_PX * (nBlockX) - 27);
		if (DEBUG) graphics.DrawLine(bar, yFoot, bar, Level::BLOCK_HEIGHT_PX, 255, 255, 255);

		if(xFoot + mX > bar ) {
			mX = 0;
		}
	}

	//===================== Change Scenes =====================
	if(yFoot - 20 > Level::BLOCK_HEIGHT_PX * 3) {
		level->changeScene(D);
		prince->setY(-60);
	}

	if(yFoot < 0) {
		//level->changeScene(U);
		//prince->setY(Level::BLOCK_HEIGHT_PX * 3);
	}

	if(xFoot + 60 > Level::BLOCK_WIDTH_PX * 10 + 5) {
		level->changeScene(R);
		prince->setX( -30);
	}

	if(xFoot < -5) {
		level->changeScene(L);
		prince->setX(Level::BLOCK_WIDTH_PX * 9 - 30);
	}


	//===================== Move Prince =====================
	prince->MoveX(mX);
	prince->MoveY(mY);

	char code = level->getCodeByCoord(prince->getMidX(), prince->getMidY());

	////================== Open/Close Gates ==================
	if (code == '-' || 
		code == '=') {

		std::map<std::pair<int, int>, Entity*>* entitites = level->getEntities();
		std::map<std::pair<int, int>, std::pair<int, int> >* mechanism = level->getMec();

		int absI = level->getAbsBlockX(level->getBlockXByCoord(prince->getMidX()));
		int absJ = level->getAbsBlockY(level->getBlockYByCoord(prince->getMidY()));

		std::pair<int, int> p(absI, absJ - 1);

		std::pair<int, int> k = (*mechanism)[p];

		if (entitites->find(k) == entitites->end()) {
			return;
		}

		Entity* e = (*entitites)[k];

		Gate* g = dynamic_cast<Gate*>(e);

		if (code == '-') {
			g->Open();
		} else {
			g->Close();
		}
	}
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

		if (level->getCodeByCoord(prince->getMidX(), prince->getMidY()) == '|') {

			if (prince->PickUpSword()) {
				level->setCodeByCoord(prince->getMidX(), prince->getMidY(), '_');
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
	
	std::map<std::pair<int,int>, Entity*>* entitites = level->getEntities();
	for (std::map<std::pair<int,int>, Entity*>::iterator i = entitites->begin(); i != entitites->end(); i++) {
		i->second->Animate(&graphics);
	}
	
	prince->Animate(&graphics);
	
	DrawForeground();

	DrawHealth();
}
void Game::DrawHealth() {

	for(int i = 0; i < prince->getHealth(); i++ ) {
		//graphics.DrawSprite(i * healthFull.width, Level::BLOCK_HEIGHT_PX * 3, &healthFull );
		graphics.DrawSprite(i * getSprite("healthFull")->width, Level::BLOCK_HEIGHT_PX * 3, getSprite("healthFull"));

	}

	for(int i = prince->getHealth(); i < prince->getMaxHealth(); i++ ) {
		//graphics.DrawSprite(i * healthFull.width,  Level::BLOCK_HEIGHT_PX * 3, &healthEmpty );
		graphics.DrawSprite(i * getSprite("healthEmpty")->width, Level::BLOCK_HEIGHT_PX * 3, getSprite("healthEmpty"));
	}
}
void Game::DrawBackground() {
	int yOff = 0;
	int xOff = 0;

	for(int j = Level::SCENE_HEIGHT_BLK; j >= 0; j--){
		for(int i = 0; i <= Level::SCENE_WIDTH_BLK; i++){

			yOff = Level::BLOCK_HEIGHT_PX * j;
			xOff = - Level::BLOCK_WIDTH_PX + Level::BLOCK_WIDTH_PX * i;

			switch(level->getCodeByBlock(j, i)) {
			case '|':
				graphics.DrawSprite(xOff, yOff - getSprite("tileCornerLeft")->height, getSprite("tileCornerLeft"));
				break;
			case 'T':
				graphics.DrawSprite(xOff, yOff - getSprite("tileCornerLeft")->height, getSprite("tileCornerLeft"));
				graphics.DrawSprite(xOff - 1, yOff - getSprite("columnBack")->height - 15, getSprite("columnBack"));
				break;
			case ']':
				graphics.DrawSprite(xOff, yOff - getSprite("blockCornerRight")->height, getSprite("blockCornerRight"));
				break;
			case '_':
				graphics.DrawSprite(xOff, yOff - getSprite("tileCornerLeft")->height, getSprite("tileCornerLeft"));
				break;
			case '-':
				if(level->getCodeByCoord(prince->getMidX(), prince->getMidY()) == '-') {

					/*TODO: this needs to be fixed */
					//if(level->getBlockXByCoord(prince->getMidX()) == j && level->getBlockYByCoord(prince->getMidY() == i) ) {
					graphics.DrawSprite(xOff, yOff - getSprite("tileCornerLeft")->height, getSprite("tileCornerLeft"));
				} else {
					graphics.DrawSprite(xOff, yOff - getSprite("activate")->height, getSprite("activate"));
				}
				break;
			case '=':
				graphics.DrawSprite(xOff, yOff - getSprite("trap")->height, getSprite("trap"));
				break;
			case 'S':
				graphics.DrawSprite(xOff, yOff - getSprite("deadSk")->height, getSprite("deadSk"));
				break;
			case '^':
				graphics.DrawSprite(xOff, yOff - getSprite("tileCornerLeft")->height, getSprite("tileCornerLeft"));
				break;
			case 'G':
				graphics.DrawSprite(xOff, yOff - getSprite("doorFrameBack")->height, getSprite("doorFrameBack"));
				break;
			case '#':
				graphics.DrawSprite(xOff, yOff - getSprite("tileCornerLeft")->height, getSprite("tileCornerLeft"));
				graphics.DrawSprite(xOff, yOff - getSprite("bricks")->height, getSprite("bricks"));
				break;
			case 'P':
				graphics.DrawSprite(xOff, yOff - getSprite("tileCornerLeft")->height, getSprite("tileCornerLeft"));
				break;
			case '$':
				graphics.DrawSprite(xOff, yOff - getSprite("tileCornerLeft")->height, getSprite("tileCornerLeft"));
				graphics.DrawSprite(xOff, yOff - getSprite("rubbleBack")->height, getSprite("rubbleBack"));
				break;
			case '*':
				graphics.DrawSprite(xOff, yOff - getSprite("bricks")->height, getSprite("bricks"));
				break;
			case '/':
				graphics.DrawSprite(xOff, yOff - getSprite("holyFloor")->height, getSprite("holyFloor"));
				break;
			case '!':
				graphics.DrawSprite(xOff, yOff - getSprite("tileCornerLeft")->height, getSprite("tileCornerLeft"));
				break;
			case ' ':
				break;

			}
		}
	}
}
void Game::DrawForeground() {
	int yOff;
	int xOff;

	for(int i = 3; i >= 0; i--){
		for(int j = 0; j < 11; j++){

			yOff = Level::BLOCK_HEIGHT_PX * i;
			xOff = - Level::BLOCK_WIDTH_PX + Level::BLOCK_WIDTH_PX * j;

			switch(level->getCodeByBlock(i, j)) {
		

			case '$':
				graphics.DrawSprite(xOff, yOff - getSprite("rubbleFront")->height, getSprite("rubbleFront"));
				break;
			case 'T':
				graphics.DrawSprite(xOff, yOff - getSprite("columnFront")->height, getSprite("columnFront"));
				break;
			case '[':
				graphics.DrawSprite(xOff, yOff - getSprite("blockCornerLeft")->height, getSprite("blockCornerLeft"));
				break;
			case 'G':
				graphics.DrawSprite(xOff, yOff - getSprite("doorFrameFront")->height, getSprite("doorFrameFront"));
				break;
			case 'I':
				graphics.DrawSprite(xOff, yOff - getSprite("block")->height, getSprite("block"));
				break;
			}
		}
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
	yOff = TOP_MARGIN + BLOCK_HEIGHT_PX * 3;

		//blocks
		for(int i = 0; i< 3; i++) {
			graphics.DrawSprite(i  * BLOCK_WIDTH_PX, yOff - block.height, &block);
		}
		graphics.DrawSprite(3 * BLOCK_WIDTH_PX - 1, yOff - blockCornerRight.height + 2, &blockCornerRight);

		//separatrs
		for(int i = 0; i< 5; i++) {
			graphics.DrawSprite((4 + i) * BLOCK_WIDTH_PX - 2, yOff - separator.height, &separator);
		}

		//floors
		for(int i = 0; i< 5; i++) {
			graphics.DrawSprite((4+ i) * BLOCK_WIDTH_PX, yOff + 1 - separator.height - floor.height, &floor);
		}

		//columns
		graphics.DrawSprite(5 * BLOCK_WIDTH_PX + 15, yOff - columnFront.height - separator.height + 1, &columnFront);
		graphics.DrawSprite(5 * BLOCK_WIDTH_PX + 14 + columnFront.width, yOff - columnBack.height - separator.height - 7 + 1, &columnBack);

		//blocks
		graphics.DrawSprite(9 * BLOCK_WIDTH_PX, yOff - blockCornerLeft.height, &blockCornerLeft);


	//level 2
	yOff = TOP_MARGIN + BLOCK_HEIGHT_PX * 2;

		//blocks
		for(int i = 0; i < 4; i++) {
			graphics.DrawSprite(0 + (9 - i) * BLOCK_WIDTH_PX, yOff - block.height, &block);
		}

		graphics.DrawSprite(0 + 5 * BLOCK_WIDTH_PX, yOff - blockCornerLeft.height, &blockCornerLeft);
		graphics.DrawSprite(0  - BLOCK_WIDTH_PX - 1, yOff - blockCornerRight.height, &blockCornerRight);

		//separators
		for(int i = 0; i< 3; i++) {
			graphics.DrawSprite(i * BLOCK_WIDTH_PX - 2, yOff - separator.height, &separator);
		}
		graphics.DrawSprite(3 * BLOCK_WIDTH_PX - 4, yOff - separatorCorner.height + 2, &separatorCorner);
		
		//floors
		for(int i = 0; i< 3; i++) {
			graphics.DrawSprite(i * BLOCK_WIDTH_PX, yOff + 1 - separator.height - floor.height, &floor);
		}
		graphics.DrawSprite(3 * BLOCK_WIDTH_PX, yOff + 1 - separator.height - floor.height, &floor);
		
		//columns
		graphics.DrawSprite(3 * BLOCK_WIDTH_PX + 15, yOff - columnFront.height - separator.height + 1, &columnFront);
		graphics.DrawSprite(3 * BLOCK_WIDTH_PX + 14 + columnFront.width, yOff - columnBack.height - separator.height - 7 + 1, &columnBack);


	//level 3
	yOff = TOP_MARGIN + BLOCK_HEIGHT_PX;

		//separators
		graphics.DrawSprite(0  - BLOCK_WIDTH_PX, yOff + 1 - separator.height - floor.height, &floor);
		graphics.DrawSprite(0  - BLOCK_WIDTH_PX - 4, yOff - separatorCorner.height + 2, &separatorCorner);

		for(int i = 0; i< 5; i++) {
			graphics.DrawSprite(0 + (7 - i) * BLOCK_WIDTH_PX, yOff - separator.height, &separator);
			
		}

		//floors
		for(int i = 0; i< 5; i++) {
			graphics.DrawSprite(1 + (i + 3) * BLOCK_WIDTH_PX, yOff + 1 - separator.height - floor.height, &floor);
		}

		//blocks
		graphics.DrawSprite(0 + 9 * BLOCK_WIDTH_PX, yOff - block.height, &block);
		graphics.DrawSprite(0 + 8 * BLOCK_WIDTH_PX, yOff - blockCornerLeft.height, &blockCornerLeft);


	//Level 4

		//separators
		for(int i =0; i < SCENE_WIDTH_BLK; i++) {
			graphics.DrawSprite(i * BLOCK_WIDTH_PX, TOP_MARGIN - separator.height, &separator);
		}

}
*/
/*
LoadSprite(&block,            L"Assets//block.png"             );
LoadSprite(&blockCornerLeft,  L"Assets//blockCornerLeft.png"   );
LoadSprite(&blockCornerRight, L"Assets//blockCornerRight.png"  );
LoadSprite(&bricks,           L"Assets//bricks.png"            );
LoadSprite(&separator,        L"Assets//separator.png"         );
LoadSprite(&separatorCorner,  L"Assets//separator_corner.png"  );
LoadSprite(&floor,            L"Assets//floor.png"             );
LoadSprite(&trap,             L"Assets//trap.png"              );
LoadSprite(&activate,         L"Assets//activate.png"          );
LoadSprite(&healthFull,       L"Assets//healthFull.png"        );
LoadSprite(&healthEmpty,      L"Assets//healthEmpty.png"       );
LoadSprite(&columnFront,      L"Assets//columnFront.png"       );
LoadSprite(&columnBack,       L"Assets//columnBack.png"        );
LoadSprite(&rubbleFront,      L"Assets//rubbleBack.png"        );
LoadSprite(&rubbleBack,       L"Assets//rubbleFront.png"       );
LoadSprite(&gateFrameBack,    L"Assets//doorFrameBack.png"     );
LoadSprite(&gateFrameFront,   L"Assets//doorFrameFront.png"    );
LoadSprite(&tileCornerLeft,   L"Assets//tileCornerLeft.png"    );
LoadSprite(&deadSk,           L"Assets//deadSk.png"            );
LoadSprite(&gate,             L"Assets//gate.png"              );
LoadSprite(&holyFloor,        L"Assets//holyFloor.png"         );
*/