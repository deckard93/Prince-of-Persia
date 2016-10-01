#include "Game.h"
#include "Platform.h"
#include "Graphics.h"
#include "Level.h"
#include "Guard.h"
#include "FinishDoor.h"
#include "GuardAI.h"


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
	RegisterSprite("blockRound");
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
	RegisterSprite("spikesBackground");
	RegisterSprite("spikesForeground");
	RegisterSprite("spikes");
	RegisterSprite("torch");
	RegisterSprite("guilotine");
	RegisterSprite("gate");
	RegisterSprite("sword");
	RegisterSprite("potionHealth");
	RegisterSprite("potionPoison");
	RegisterSprite("potionExtend");
	
	//prince sprite sheets
	RegisterSprite("idle"       , "Assets//prince//");
	RegisterSprite("turn"       , "Assets//prince//");
	RegisterSprite("running"    , "Assets//prince//");
	RegisterSprite("climbUp"    , "Assets//prince//");
	RegisterSprite("newClimbUp" , "Assets//prince//");
	RegisterSprite("jumpGrab"   , "Assets//prince//");
	RegisterSprite("newJumpGrab", "Assets//prince//");
	RegisterSprite("crouch"     , "Assets//prince//");
	RegisterSprite("staticJump" , "Assets//prince//");
	RegisterSprite("step"       , "Assets//prince//");
	RegisterSprite("missStep"   , "Assets//prince//");
	RegisterSprite("hang"       , "Assets//prince//");
	RegisterSprite("newHang"    , "Assets//prince//");
	RegisterSprite("runningJump", "Assets//prince//");
	RegisterSprite("runningTurn", "Assets//prince//");
	RegisterSprite("fall"       , "Assets//prince//");
	RegisterSprite("drop"       , "Assets//prince//");
	RegisterSprite("newDrop"    , "Assets//prince//");
	RegisterSprite("drink"      , "Assets//prince//");
	RegisterSprite("pickSword"  , "Assets//prince//");

	//prince sprite sheets
	RegisterSprite("guardIdle", "Assets//guard//");


	//fight sheets
	RegisterSprite("fightIdle"  , "Assets//prince//");
	RegisterSprite("fightStep"  , "Assets//prince//");
	RegisterSprite("fightParry" , "Assets//prince//");
	RegisterSprite("fightStart" , "Assets//prince//");
	RegisterSprite("fightStrike", "Assets//prince//");
	RegisterSprite("fightFinish", "Assets//prince//");
	RegisterSprite("fightInjure", "Assets//prince//");
	RegisterSprite("fightDying" , "Assets//prince//");
	RegisterSprite("swordDeath", "Assets//prince//");

	RegisterSprite("guardFightIdle"   , "Assets//guard//");
	RegisterSprite("guardFightStep"   , "Assets//guard//");
	RegisterSprite("guardFightParry"  , "Assets//guard//");
	RegisterSprite("guardFightParried", "Assets//guard//");
	RegisterSprite("guardFightStrike" , "Assets//guard//");
	RegisterSprite("guardFightInjure" , "Assets//guard//");
	RegisterSprite("guardFightDying"  , "Assets//guard//");
	RegisterSprite("guardSwordDeath"  , "Assets//guard//");
	//RegisterSprite("fightFinish", "Assets//guard//");
	//RegisterSprite("fightStart", "Assets//guard//");

	//death/kill sprites
	RegisterSprite("spikeKill"  , "Assets//prince//");
	RegisterSprite("fightKill"  , "Assets//prince//");


	//door sprites
	RegisterSprite("finishDoor"    , "Assets//finishDoor//");
	RegisterSprite("finishDoorBase", "Assets//finishDoor//");
	RegisterSprite("enterDoor"     , "Assets//finishDoor//");

	RegisterSprite("platformMove", "Assets//platform//");


	//objects
	level = new Level();
	prince = new Prince();
	engagedGuard = NULL;

	Reset();

	prince->getAnim()->setLoop(true);
	prince->getAnim()->Play();


	timer.StartWatch();
	timeSinceLastFrame = 0;
}

void Game::GameLoop() {

	graphics.BeginFrame();
	
	HandleInput();
	if (!prince->isDead()) {
		ControlAI();
		CheckCollision();
	}

	DrawGraphics();

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
void Game::ControlAI() {
	std::map<std::pair<int, int>, Entity*>* entitites = level->getEntities();
	for (std::map<std::pair<int, int>, Entity*>::iterator i = entitites->begin(); i != entitites->end(); i++) {
		if (i->second == NULL) { continue; } // TODO: should not happen check load entities
		if (i->second->getType() == guardT) {
			if (level->inScene(i->first.second, i->first.first)) {
				Entity* guard = i->second;
				GuardAI* guardAI = dynamic_cast<GuardAI*>(guard);
				assert(guardAI != 0);
				guardAI->Control(*prince, *level);
				Character* character = dynamic_cast<Character*>(guard);
				assert(character != 0);
				CheckCharacterCollision(*character);
			}
		}
	}
}


void Game::CheckCharacterCollision(Character& character) {
	

	//calculate foot position
	int xFoot = character.getX() + character.getAnim()->getSheet()->getFrameWidth() / 2 - 36;
	int yFoot = character.getY() + character.getAnim()->getSheet()->getFrameHeight() - 9;

	int xFootReal = character.getX() + character.getAnim()->getSheet()->getFrameWidth() / 2;
	int yFootReal = character.getY() + character.getAnim()->getSheet()->getFrameHeight() - 9;

	int mX = character.getDefferX();
	int mY = character.getDefferY();

	int nBlockX = level->getSceneBlockXByCoord(xFoot);
	int nBlockY = level->getSceneBlockYByCoord(yFoot);
	//nBlockX = level->getSceneBlockXByCoord(xFootReal - 23);


	//===================== Fall ==========================
	if (level->getSceneCodeByBlock(nBlockY, nBlockX) == '_' ||
		level->getSceneCodeByBlock(nBlockY, nBlockX) == '#' ||
		level->getSceneCodeByBlock(nBlockY, nBlockX) == '/' ||
		level->getSceneCodeByBlock(nBlockY, nBlockX) == '^' ||
		level->getSceneCodeByBlock(nBlockY, nBlockX) == '$' ||
		level->getSceneCodeByBlock(nBlockY, nBlockX) == '|' ||
		level->getSceneCodeByBlock(nBlockY, nBlockX) == '_') {

		int bar = (Level::BLOCK_HEIGHT_PX * nBlockY);
		if (yFoot < bar - 20) {
			//mY += prince->setFall(); //TODO make part of Character
		}
		else {
			//prince->Land(); //TODO make part of character
			//prince->setY(bar);
		}
	}

	if (level->getSceneCodeByBlock(nBlockY, nBlockX) == ' ' ||
		level->getSceneCodeByBlock(nBlockY, nBlockX) == '*') {
		//mY += prince->setFall(); TODO make this part of the character Class
	}

	//=============== Collision With Walls =====================
	if (mX < 0 && level->getSceneCodeByBlock(nBlockY, nBlockX) == ']') {
		int bar = (Level::BLOCK_WIDTH_PX * (nBlockX));

		if (DEBUG) graphics.DrawLine(0, bar, Graphics::SCREENX, bar, 255, 255, 255);

		if (xFoot + mX < bar) {
			mX = 0;
		}
	}

	if (mX > 0 && level->getSceneCodeByBlock(nBlockY, nBlockX + 1) == '[' ||
		level->getSceneCodeByBlock(nBlockY, nBlockX) == '[') {

		int bar = (Level::BLOCK_WIDTH_PX * (nBlockX)-27);
		if (DEBUG) graphics.DrawLine(bar, yFoot, bar, Level::BLOCK_HEIGHT_PX, 255, 255, 255);

		if (xFoot + mX > bar) {
			mX = 0;
		}
	}

	

	//===================== Move Prince =====================
	character.MoveX(mX);
	character.MoveY(mY);

	char code = level->getSceneCodeByCoord(prince->getMidX(), prince->getMidY());
	int absX = level->getLevelBlockX(level->getSceneBlockXByCoord(prince->getMidX()));
	int absY = level->getLevelBlockY(level->getSceneBlockYByCoord(prince->getMidY()));
	std::map<std::pair<int, int>, Entity*>* entitites = level->getEntities();

	////================== Open/Close Gates ==================
	if (code == '-' ||
		code == '=') {


		std::map<std::pair<int, int>, std::pair<int, int> >* mechanism = level->getMec();

		std::pair<int, int> platKey(absY, absX);
		std::pair<int, int> gateKey = (*mechanism)[platKey];

		//gateKey = make_pair(50, 4);

		if (entitites->find(gateKey) == entitites->end()) {
			return;
		}

		Entity* e = (*entitites)[gateKey];
		if (e->getType() == doorT) {
			FinishDoor* d = dynamic_cast<FinishDoor*>(e);
			if (code == '-') {
				d->Open();
			}
		} else if (e->getType() == gateT) {
			Gate* g = dynamic_cast<Gate*>(e);
			if (code == '-') {
				g->Open();
			}
			else {
				g->Close();
			}
		}
	}

}
void Game::CheckPrinceCollision() {
	//calculate foot position
	int xFoot = prince->getX() + prince->getAnim()->getSheet()->getFrameWidth() / 2 - /*36*/ 16;
	int yFoot = prince->getY() + prince->getAnim()->getSheet()->getFrameHeight() - 9;

	int xFootReal = xFoot + timer.GetTimeMilli();
	int yFootReal = yFoot;

	if (DEBUG) { graphics.DrawCircle(xFoot, yFoot, 20, 255, 255, 255); }

	timer.StopWatch();
	double mX = prince->getDefferX() * timer.GetTimeMilli() / 14.5;
	double mY = prince->getDefferY();

	prince->setAccX(prince->getAccX() * 0.94);
	prince->setAccY(prince->getAccY() * 1.01);
	if (prince->getAccY() > 10) { prince->setAccY(10); }
	
	timer.StartWatch();

	int nBlockX = level->getSceneBlockXByCoord(xFoot);
	int nBlockY = level->getSceneBlockYByCoord(yFoot);

	

	//===================== Ledge Climb ==========================
	/*
	{
		std::string s = "Code: ";
		std::string g(1, level->getSceneCodeByBlock(checkLedgeY, checkLedgeX));
		s += g + "\n";
		OutputDebugStringA(s.c_str());
	}
	*/

	{
		int nBlockX = level->getSceneBlockXByCoord(xFoot);
		int nBlockY = level->getSceneBlockYByCoord(yFoot);

		int checkLedgeX = nBlockX + 1;
		int checkLedgeY = nBlockY - 1;
		int checkLedgeXOp = nBlockX - 1;
		if (!prince->isFacingRight()) { 
			checkLedgeX = nBlockX - 1; 
			checkLedgeXOp = nBlockX + 1;
		}

		if (prince->isJumpGrab()) {
			
			if (level->isLedge(level->getSceneCodeByBlock(checkLedgeY, checkLedgeX)) &&
				level->isEmptySpace(level->getSceneCodeByBlock(nBlockY - 1, nBlockX))
				)
			{
				// Initiate climbUp
				int offset = -25;
				if (prince->isFacingRight()) { offset = 15; }
				prince->setX((nBlockX - 1) * Level::BLOCK_WIDTH_PX + offset);
			}
			else if (level->isLedge(level->getSceneCodeByBlock(nBlockY - 1, nBlockX)) &&
					 level->isEmptySpace(level->getSceneCodeByBlock(nBlockY - 1, checkLedgeXOp)) &&
					 level->isEmptySpace(level->getSceneCodeByBlock(nBlockY, checkLedgeXOp))
					) 
			{
				// Initiate climbUp - Same Level
				int offset = 14;
				if (prince->isFacingRight()) { offset = -36; }
				prince->setX((nBlockX - 1) * Level::BLOCK_WIDTH_PX + offset);

				if (prince->getAnim()->getCurrentFrame() == 3) {
					if (prince->isFacingRight()) {
						prince->MoveX(-20);
					}
					else {
						prince->MoveX(7);
					}
				}
			}
			else {
				// Cancel cimbUp (no ledge)
				if (prince->getAnim()->getCurrentFrame() == 2) {
					prince->getAnim()->setCurrentDisplayTime(80);
					if (prince->getAnim()->isEffectPending()) {
						prince->MoveY(-16);
						prince->getAnim()->setEffectDone();
					}
				}
				else if (prince->getAnim()->getCurrentFrame() < 2) {
					prince->getAnim()->Reset();
					prince->setDrop();
				}
			}
		}
	}


	{
		int nBlockX = level->getSceneBlockXByCoord(xFoot - 40);
		if (prince->isFacingRight()) { nBlockX = level->getSceneBlockXByCoord(xFoot + 40); }
		int nBlockY = level->getSceneBlockYByCoord(yFoot);

		int checkLedgeX = nBlockX + 1;
		int checkLedgeY = nBlockY - 1;
		if (prince->isFacingRight()) { checkLedgeX = nBlockX - 1; }

		if (prince->isGoingDown()) {
			{
				std::string s = "Code: ";
				std::string g(1, level->getSceneCodeByBlock(checkLedgeY, checkLedgeX));
				s += g + "\n";
				OutputDebugStringA(s.c_str());
			}

			if (level->isLedge(level->getSceneCodeByBlock(nBlockY, nBlockX)) &&
				level->isEmptySpace(level->getSceneCodeByBlock(nBlockY, checkLedgeX)))
			{
				int offset = -30;
				if (!prince->isFacingRight()) { offset = 10; }
				prince->setX((nBlockX - 1)* Level::BLOCK_WIDTH_PX + offset);
				prince->setClimbDown();
			}
		}

		if (prince->isDrop()) {
			if (level->isLedge(level->getSceneCodeByBlock(nBlockY, nBlockX)) &&
				level->isEmptySpace(level->getSceneCodeByBlock(nBlockY, checkLedgeX)))
			{
				int offset = 15;
				if (prince->isFacingRight()) { offset = -40; }
				prince->setX((nBlockX - 1)* Level::BLOCK_WIDTH_PX + offset);
			}
		}
	}

	if (prince->isDrop()) {
		int nBlockX = level->getSceneBlockXByCoord(xFoot);
		if (!prince->isFacingRight()) { nBlockX = level->getSceneBlockXByCoord(xFoot); }
		int nBlockY = level->getSceneBlockYByCoord(yFoot);

		int checkLedgeX = nBlockX - 1;
		int checkLedgeY = nBlockY;
		if (!prince->isFacingRight()) { checkLedgeX = nBlockX - 1; }

		if (!level->isLedge(level->getSceneCodeByBlock(checkLedgeY, checkLedgeX)) ||
			!level->isEmptySpace(level->getSceneCodeByBlock(nBlockY + 1, nBlockX)))
		{
			if (prince->getAnim()->getCurrentFrame() == 4 && prince->getAnim()->isEffectPending() && !prince->getAnim()->isFinished()) {
				if (prince->isFacingRight()) {
					mX = 18;
				}
				else {
					mX = -18;
				}
				
				//prince->getAnim()->setEffectDone();
			}
		}
	}

	//===================== Shift Danger Detection ==========================
	{
		int nBlockX = level->getSceneBlockXByCoord(xFoot + mX + 2);
		if (!prince->isFacingRight()) { nBlockX = level->getSceneBlockXByCoord(xFoot + mX - 5); }
		int nBlockY = level->getSceneBlockYByCoord(yFoot);

		if (prince->isStep()) {
			if (level->getSceneCodeByBlock(nBlockY, nBlockX) == ' ' ||
				level->getSceneCodeByBlock(nBlockY, nBlockX) == '*' ) {
				if (prince->getAnim()->getCurrentFrame() == 5) {
					prince->getAnim()->Reset();
					prince->setMissStep();
				}
				if (prince->getAnim()->getCurrentFrame() > 5) {
					mX = 0;
				}
			}
		}
	}


	//===================== Fall ==========================
	{
		int xFoot = prince->getX() + prince->getAnim()->getSheet()->getFrameWidth() / 2 - /*36*/ 16;
		int yFoot = prince->getY() + prince->getAnim()->getSheet()->getFrameHeight() - 9;

		int nBlockX = level->getSceneBlockXByCoord(xFoot);
		int nBlockY = level->getSceneBlockYByCoord(yFoot);

		
		int bar = (Level::BLOCK_HEIGHT_PX * nBlockY) - 20;
		if (DEBUG) graphics.DrawLine(0, bar, Graphics::SCREENX, bar, 255, 255, 255);

		if (!prince->isFalling() && !prince->isMovingUp()) {
			prince->setY(prince->getY() - yFoot + bar);
		}
	}


	{
		int xFoot = prince->getX() + prince->getAnim()->getSheet()->getFrameWidth() / 2 - /*36*/ 16;
		int yFoot = prince->getY() + prince->getAnim()->getSheet()->getFrameHeight() - 9;

		int nBlockX = level->getSceneBlockXByCoord(xFoot);
		int nBlockY = level->getSceneBlockYByCoord(yFoot);

		if (prince->isFalling() || prince->isIdle() && (level->getSceneCodeByBlock(nBlockY, nBlockX) == '-' ||
			level->getSceneCodeByBlock(nBlockY, nBlockX) == '#' ||
			level->getSceneCodeByBlock(nBlockY, nBlockX) == '/' ||
			level->getSceneCodeByBlock(nBlockY, nBlockX) == '^' ||
			level->getSceneCodeByBlock(nBlockY, nBlockX) == '$' ||
			level->getSceneCodeByBlock(nBlockY, nBlockX) == '|' ||
			level->getSceneCodeByBlock(nBlockY, nBlockX) == '=' ||
			level->getSceneCodeByBlock(nBlockY, nBlockX) == '~' ||
			level->getSceneCodeByBlock(nBlockY, nBlockX) == '_')
			) {

			int bar = (Level::BLOCK_HEIGHT_PX * nBlockY) - 20;
			if (DEBUG) graphics.DrawLine(0, bar, Graphics::SCREENX, bar, 255, 255, 255);

			if (yFoot >= bar) {
				//mY = 0;
				prince->Land(level->getLevelBlockYByCoord(yFoot));
			}
			else {
				prince->setFall(level->getLevelBlockYByCoord(yFoot));
			}
		}
	}
	
	
	if (level->getSceneCodeByBlock(nBlockY, nBlockX) == '~') {
		std::map<std::pair<int, int>, Entity*>* entities = level->getEntities();
		std::pair<int, int> platformKey = std::make_pair(level->getLevelBlockY(nBlockY), level->getLevelBlockX(nBlockX));
		Entity* e = entities->at(platformKey);
		Platform* p = dynamic_cast<Platform*>(e);
		if (p != NULL && p->getState() == dislodged) {
			prince->setFall(level->getLevelBlockYByCoord(yFoot));
		}	
	}
	


	if (level->getSceneCodeByBlock(nBlockY, nBlockX) == ' ' ||
		level->getSceneCodeByBlock(nBlockY, nBlockX) == '*') {
		prince->setFall(level->getLevelBlockYByCoord(yFoot));
	}

	//=============== Collision With Walls =====================
	if (!prince->isClimbUp() && !prince->isJumpGrab()) {
		if (level->getSceneCodeByBlock(nBlockY, nBlockX - 1) == ']') {
			int bar = (Level::BLOCK_WIDTH_PX * (nBlockX - 1) + 20);

			if (DEBUG) graphics.DrawLine(bar, 0, bar, Graphics::SCREENY, 255, 255, 255);

			if (xFoot + mX < bar) {
				mX = 0;
				prince->setAccX(0);
				prince->MoveX(bar - xFoot);
			}
		}

		if (level->getSceneCodeByBlock(nBlockY, nBlockX + 1) == '[' ||
			level->getSceneCodeByBlock(nBlockY, nBlockX) == '[' ||
			level->getSceneCodeByBlock(nBlockY, nBlockX + 1) == 'O' ||
			level->getSceneCodeByBlock(nBlockY, nBlockX) == 'O') {

			int bar = (Level::BLOCK_WIDTH_PX * (nBlockX)-10);
			if (DEBUG) graphics.DrawLine(bar, yFoot, bar, Level::BLOCK_HEIGHT_PX, 255, 255, 255);

			if (xFoot + mX > bar) {
				mX = 0;
				prince->setAccX(0);
				prince->MoveX(bar - xFoot);
			}
		}
	}


	//=============== Collision With Gates =====================

	//TODO clean this up
	//TODO running Turn Bug
	{
		//int xOffset = -22;
		int xOffset = 0;
		int nBlockX = level->getSceneBlockXByCoord(prince->getMidX() + xOffset);
		int nBlockY = level->getSceneBlockYByCoord(prince->getMidY());
		if (level->getSceneCodeByBlock(nBlockY, nBlockX) == 'G' && prince->isFacingRight()) {

			std::map<std::pair<int, int>, Entity*>* entitites = level->getEntities();
			int absY = level->getLevelBlockY(nBlockY);
			int absX = level->getLevelBlockY(nBlockX);

			std::pair<int, int> gateKey(absY, absX);
			Gate* g = dynamic_cast<Gate*>((*entitites)[gateKey]);

			if (g != NULL && !g->isOpen()) {
				int bar = (Level::BLOCK_WIDTH_PX * (nBlockX));

				if (DEBUG) graphics.DrawLine(bar, 0, bar, Graphics::SCREENY, 255, 255, 255);
				if (DEBUG) graphics.DrawLine(prince->getMidX() + xOffset, 0, prince->getMidX() + xOffset, Graphics::SCREENY, 0, 255, 0);

				if (prince->getMidX() + xOffset + mX >= bar) {
					mX = 0;
				}
			}
		}

		xOffset -= 20;
		nBlockX = level->getSceneBlockXByCoord(prince->getMidX() + xOffset);
		nBlockY = level->getSceneBlockYByCoord(prince->getMidY());
		nBlockX--;
		if (level->getSceneCodeByBlock(nBlockY, nBlockX) == 'G' && !prince->isFacingRight()) {

			std::map<std::pair<int, int>, Entity*>* entitites = level->getEntities();
			int absY = level->getLevelBlockY(nBlockY);
			int absX = level->getLevelBlockY(nBlockX);

			std::pair<int, int> gateKey(absY, absX);
			Gate* g = dynamic_cast<Gate*>((*entitites)[gateKey]);


			if (g != NULL && !g->isOpen()) {
				int bar = (Level::BLOCK_WIDTH_PX * (nBlockX));

				if (DEBUG) graphics.DrawLine(bar, 0, bar, Graphics::SCREENY, 255, 255, 255);
				if (DEBUG) graphics.DrawLine(prince->getMidX() + xOffset, 0, prince->getMidX() + xOffset, Graphics::SCREENY, 0, 255, 0);

				if (prince->getMidX() + xOffset + mX <= bar) {
					mX = 0;
				}
			}
		}
	}



	//===================== Change Scenes =====================

	if (prince->getMidY() > Level::BLOCK_HEIGHT_PX * 3) { // down
		level->changeScene(D);
		prince->setY(-60);
	}

	if (prince->getMidY() < 0) { // up
		level->changeScene(U);
		prince->setY(Level::BLOCK_HEIGHT_PX * 3 - 70);
	}

	
	if (xFoot + 28 > Level::BLOCK_WIDTH_PX * 10) { // right
		level->changeScene(R);
		prince->setX(-40);
	}
	

	if (xFoot < -7) { // left
		level->changeScene(L);
		prince->setX(Level::BLOCK_WIDTH_PX * 9 - 20);
	}


	if (input->isShiftPressed()) {

		if (CheckCatchConditions(mY)) {
			prince->Catch();
		}
	}

	

	char code = level->getSceneCodeByCoord(prince->getMidX() - 22, prince->getMidY());
	int absX = level->getLevelBlockX(level->getSceneBlockXByCoord(prince->getMidX() - 22));
	int absY = level->getLevelBlockY(level->getSceneBlockYByCoord(prince->getMidY()));
	std::map<std::pair<int, int>, Entity*>* entitites = level->getEntities();

	////================== Open/Close Gates ==================
	if ( (code == '-' || code == '=' ) &&
		 !prince->isJumping()
		) {


		std::map<std::pair<int, int>, std::pair<int, int> >* mechanism = level->getMec();

		std::pair<int, int> platKey(absY, absX);
		std::pair<int, int> gateKey = (*mechanism)[platKey];

		//gateKey = make_pair(50, 4);

		if (entitites->find(gateKey) == entitites->end()) {
			return;
		}

		Entity* e = (*entitites)[gateKey];
		if (e->getType() == doorT) {
			FinishDoor* d = dynamic_cast<FinishDoor*>(e);
			if (code == '-') {
				d->Open();
			}
		}
		else if (e->getType() == gateT) {
			Gate* g = dynamic_cast<Gate*>(e);
			if (code == '-') {
				g->Open();
			}
			else {
				g->Close();
			}
		}
	}

	////============== Spikes ==============
	/*
	for (int y = 0; nBlockY + y < 5; y++) {
	char c = level->getSceneCodeByBlock(nBlockY + y, nBlockX);
	if (c == '/') {

	std::pair<int, int> platKey(absY + y, absX);
	Entity* e = (*entitites)[platKey];
	Spikes* spikes = dynamic_cast<Spikes*>(e);
	spikes->On();
	}
	}
	*/



	if (level->findSpikes(absX, absY)) {

	}

	if (code == '/') {
		if (prince->getPrinceState() == sRunning ||
			prince->getPrinceState() == sFalling) {

			nBlockX = level->getSceneBlockXByCoord(prince->getMidX());
			nBlockY = level->getSceneBlockYByCoord(prince->getMidY());
			prince->setX((nBlockX - 1) * Level::BLOCK_WIDTH_PX - 15);
			prince->setY((nBlockY - 1)* Level::BLOCK_HEIGHT_PX);
			prince->spikeKill();
		}
	}


	////============== Guilotine ==============


	//===================== Move Prince =====================
	/*
	if (mX > 0.0 && mX < 1.0) {
	prince->MoveX(mX);
	prince->MoveY(mY);
	}
	*/

	prince->MoveX(mX);
	prince->MoveY(mY);
}
void Game::CheckCombatCollision() {

	if (engagedGuard != NULL && engagedGuard->isDead()) {
		prince->Disengage();
	}
	engagedGuard = NULL;
	std::list<Character*>* guards = level->getGuards();

	for (std::list<Character*>::iterator i = guards->begin(); i != guards->end(); i++) {
		Character* guard = *i;
		if (guard->isDead()) { continue; }
		EngageFight(prince, guard, level);
	}


	for (std::list<Character*>::iterator i = guards->begin(); i != guards->end(); i++) {
		Character* guard = *i;
		if (guard->isDead()) { continue; }
		guard->checkParryBy(prince);
		if (guard->isHitting(prince, *level)) {
			prince->Hurt();
		}
	}

	for (std::list<Character*>::iterator i = guards->begin(); i != guards->end(); i++) {
		Character* guard = *i;
		if (guard->isDead()) { continue; }
		//if (!prince->checkParryBy(guard));
		if (prince->isHitting(guard, *level)) {
			guard->Hurt();
		}
	}
	if (engagedGuard == NULL) { return;  }
	double distance = abs(prince->getX() - engagedGuard->getX());
	if (distance > 50 && prince->isFighting()) {
		prince->FaceCharacter(*engagedGuard, *level);
	}
}
void Game::CheckCollision() {
	CheckPrinceCollision();
	CheckCombatCollision();
}
bool Game::CheckCatchConditions(int defferY) {
	if (prince->getPrinceState() != sFalling) { return false; }


	int yFoot = prince->getY() + prince->getAnim()->getSheet()->getFrameHeight() - 9;

	int blockX = level->getLevelBlockXByCoord(prince->getMidX());
	int blockY = level->getLevelBlockYByCoord(yFoot);

	int barY = (blockY - 1) * Level::BLOCK_HEIGHT_PX + 30;	

	graphics.DrawLine(0, barY, 800, barY, 0, 255, 0);
	graphics.DrawLine(0, prince->getMidY(), 800, prince->getMidY(), 255, 0, 0);
	graphics.DrawLine(0, prince->getMidY() + defferY, 800, prince->getMidY() + defferY, 255, 0, 0);

	int iMax = max(defferY + prince->getMidY(), prince->getMidY());
	int iMin = min(defferY + prince->getMidY(), prince->getMidY());
	if (!(iMin < barY && barY < iMax || barY == iMax || barY == iMin)) { return false; }

	

	//prince->MoveY(barY - prince->getMidY());
	//prince->defferMoveY(0);
	int ledgeX = blockX - 1;;
	if (prince->isFacingRight()) {
		ledgeX = blockX + 1;
	} 

	blockY--; //look above

	if (level->isLedge(level->getLevelCodeByBlock(ledgeX, blockY) ) &&
		level->isEmptySpace(level->getLevelCodeByBlock(blockX, blockY))) {
		prince->MoveY(barY - prince->getMidY());
		int offset = -30;
		if (prince->isFacingRight()) { offset = 0; }
		prince->setX((blockX - 1) * Level::BLOCK_WIDTH_PX + offset);
		return true;
	}


	return false;
}

void Game::HandleInput()  {
	
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

	if (input->hasBeenPressed('R')) {
		Reset();
	}

	if (input->getKeyStatus('P')) {
		prince->defferMoveX(1);
	}

	if (input->getKeyStatus('O')) {
		prince->defferMoveX(-1);
	}


	if (input->getKeyStatus('G')) {
		prince->Heal();
		std::list<Character*>* guards = level->getGuards();
		std::list<Character*>::iterator i = guards->begin();
		Character* guard = *i;
		guard->Heal();
	}

	if (input->getKeyStatus('F')) {
		prince->Hurt();
	}

	if (input->hasBeenPressed('H')) {
		std::map<std::pair<int, int>, Entity*>* entitites = level->getEntities();
		for (std::map<std::pair<int, int>, Entity*>::iterator i = entitites->begin(); i != entitites->end(); i++) {
			Entity* entity = i->second;
			if (entity->getType() == spikeT) {
				Spikes* spikes = dynamic_cast<Spikes*>(entity);
				if (spikes != NULL) { spikes->On(); }
			}
		}
	}

	if (input->hasBeenPressed('J')) {
		std::map<std::pair<int, int>, Entity*>* entitites = level->getEntities();
		for (std::map<std::pair<int, int>, Entity*>::iterator i = entitites->begin(); i != entitites->end(); i++) {
			Entity* entity = i->second;
			if (entity->getType() == spikeT) {
				Spikes* spikes = dynamic_cast<Spikes*>(entity);
				if (spikes != NULL) { spikes->Off(); }
			}
		}
	}

	if (input->hasBeenPressed('V')) {
		std::map<std::pair<int, int>, Entity*>* entitites = level->getEntities();
		for (std::map<std::pair<int, int>, Entity*>::iterator i = entitites->begin(); i != entitites->end(); i++) {
			Entity* entity = i->second;
			if (entity->getType() == doorT) {
				FinishDoor* door = dynamic_cast<FinishDoor*>(entity);
				if (door != NULL) { door->Finish(); }
			}
		}
	}

	if(input->isShiftPressed()) {

		


		std::string s = "Code: ";
		std::string g(1, level->getSceneCodeByCoord(prince->getMidX(), prince->getMidY()));
		s += g + "\n";
		OutputDebugStringA(s.c_str());

		if(level->getSceneCodeByCoord(prince->getMidX(), prince->getMidY()) == 'P') {
			if(prince->Drink()) {
				level->setCodeByCoord(prince->getMidX(), prince->getMidY(), '_');
				prince->Heal();
			}
		}

		if (level->getSceneCodeByCoord(prince->getMidX(), prince->getMidY()) == 'K') {
			if (prince->Drink()) {
				level->setCodeByCoord(prince->getMidX(), prince->getMidY(), '_');
				prince->Hurt();
			}
		}

		if (level->getSceneCodeByCoord(prince->getMidX(), prince->getMidY()) == 'E') {
			if (prince->Drink()) {
				level->setCodeByCoord(prince->getMidX(), prince->getMidY(), '_');
				prince->increaseMaxHealth();
			}
		}

		if (level->getSceneCodeByCoord(prince->getMidX(), prince->getMidY()) == '|') {
			if (prince->PickUpSword()) {
				level->setCodeByCoord(prince->getMidX(), prince->getMidY(), '_');
			}
		}


		
	}

	if (input->isUpPressed()) {

		//std::string s = "Code: ";
		//std::string g(1, level->getSceneCodeByCoord(prince->getMidX(), prince->getMidY()));
		//s += g + "\n";
		//OutputDebugStringA(s.c_str());

		if (level->getSceneCodeByCoord(prince->getMidX(), prince->getMidY()) == 'H') {
			std::map<std::pair<int, int>, std::pair<int, int> >* mechanism = level->getMec();
			std::pair<int, int> gateKey = make_pair(level->getLevelBlockYByCoord(prince->getMidY()), level->getLevelBlockXByCoord(prince->getMidX()));
		
			std::map<std::pair<int, int>, Entity*>* entitites = level->getEntities();

			if (entitites->find(gateKey) == entitites->end()) {
				return;
			}

			Entity* e = (*entitites)[gateKey];
			if (e->getType() == doorT) {
				FinishDoor* d = dynamic_cast<FinishDoor*>(e);
			
				if (d->Finish()) {
					prince->setState(sFinish);
				}
			}			
		}
	}


	prince->HandlePrince(input);
}

void Game::DrawGraphics() {

	//graphics.BeginFrame();

	//Sprite* myDoor = getSprite("finishDoor");
	//int yCut = 100;
	//graphics.DrawSprite(100,100, myDoor, 0, 0 + yCut, myDoor->width, myDoor->height - yCut);

	std::map<std::pair<int, int>, Entity*>* entitites = level->getEntities();

	DrawBackground();
	for (std::map<std::pair<int, int>, Entity*>::iterator i = entitites->begin(); i != entitites->end(); i++) {
		Entity* entity = i->second;
		Spikes* spikes = dynamic_cast<Spikes*>(entity);
		if (spikes != NULL && level->inScene(i->first.second, i->first.first)) {
			spikes->AnimateBackground(&graphics);
		}
		else {
			//i->second->Animate(NULL);
		}
	}
	if (DEBUG) { graphics.DrawCircle(prince->getMidX(), prince->getMidY(), 5, 255, 255, 255); }
	if (DEBUG) { graphics.DrawCircle(prince->getX(), prince->getY(), 5, 255, 255, 255); }
	
	for (std::map<std::pair<int, int>, Entity*>::iterator i = entitites->begin(); i != entitites->end(); i++) {
		if (i->second == NULL) { continue;  } // TODO: should not happen check load entities
		if (i->second->getType() == spikeT || i->second->getType() == platformT) { continue; }

		if (level->inScene(i->first.second, i->first.first)) {
			i->second->Animate(&graphics);
		}
		else {
			i->second->Animate(NULL);
		}
	}

	prince->Animate(&graphics);

	DrawForeground();
	for (std::map<std::pair<int, int>, Entity*>::iterator i = entitites->begin(); i != entitites->end(); i++) {
		Entity* entity = i->second;
		Spikes* spikes = dynamic_cast<Spikes*>(entity);
		if(spikes != NULL && level->inScene(i->first.second, i->first.first)) {
			spikes->AnimateForeground(&graphics);
		}
		else {
			//i->second->Animate(NULL);
		}
	}

	DrawHealth();

	//graphics.EndFrame();

}

bool Game::isGuardInScene(Character* guard_in) {

	std::map<std::pair<int, int>, Entity*>* entitites = level->getEntities();
	for (std::map<std::pair<int, int>, Entity*>::iterator i = entitites->begin(); i != entitites->end(); i++) {
		Entity* entity = i->second;
		Character* guard = dynamic_cast<Character*>(entity);
		if (guard != NULL && level->inScene(i->first.second, i->first.first) && guard_in == guard) {
			return true;
		}
	}
	return false;
}

void Game::EngageFight(Character* prince, Character* guard, Level* level) {

	if (level->getCharLevelBlockY(prince) != level->getCharLevelBlockY(guard)) { return; }
	if (std::abs(level->getCharLevelBlockX(prince) - level->getCharLevelBlockX(guard)) > 9) { return; }  //TODO: This doesnt work guard is calculated relative to scene
	if (!isGuardInScene(guard)) { return; }

	SetEngagedGuard(guard);
	if (!prince->isIdle()) { return; }

	if (prince->isFighting()) {
		prince->FaceCharacter(*guard, *level);
		return;
	}

	if (prince->getX() < guard->getX() && !prince->isFacingRight()) { return; }
	if (prince->getX() >= guard->getX() && prince->isFacingRight()) { return; }

	prince->EngageEnemy(*guard);
}

void Game::SetEngagedGuard(Character* guard) {
	if (engagedGuard == NULL) {
		engagedGuard = guard;
	} else {
		int currentGuardDistance = std::sqrt((std::pow(guard->getX() - prince->getX(), 2.0) + std::pow(guard->getY() - prince->getY(), 2.0)));
		int engagedGuardDistance = std::sqrt((std::pow(engagedGuard->getX() - prince->getX(), 2.0) + std::pow(engagedGuard->getY() - prince->getY(), 2.0)));
		
		if (currentGuardDistance < engagedGuardDistance) {
			engagedGuard = guard;
		}
	}
}

//util
void Game::ComposeFrame() {

}
void Game::DrawHealth() {

	for(int i = 0; i < prince->getHealth(); i++ ) {
		//graphics.DrawSprite(y * healthFull.width, Level::BLOCK_HEIGHT_PX * 3, &healthFull );
		graphics.DrawSprite(i * getSprite("healthFull")->width, Level::BLOCK_HEIGHT_PX * 3, getSprite("healthFull"));

	}

	for(int i = prince->getHealth(); i < prince->getMaxHealth(); i++ ) {
		//graphics.DrawSprite(y * healthFull.width,  Level::BLOCK_HEIGHT_PX * 3, &healthEmpty );
		graphics.DrawSprite(i * getSprite("healthEmpty")->width, Level::BLOCK_HEIGHT_PX * 3, getSprite("healthEmpty"));
	}




	//std::list<Character*>* guards = level->getGuards();
	//std::list<Character*>::iterator i = guards->begin();
	//engagedGuard = *i;
	if (engagedGuard == NULL) { return;  };
	for (int i = 0; i < engagedGuard->getHealth(); i++) {
		//graphics.DrawSprite(y * healthFull.width, Level::BLOCK_HEIGHT_PX * 3, &healthFull );
		graphics.DrawSprite(Level::BLOCK_WIDTH_PX * 10 - (i + 1) * ( getSprite("healthFull")->width), Level::BLOCK_HEIGHT_PX * 3, getSprite("healthFull"));

	}


}
void Game::DrawBackground() {
	int yOff = 0;
	int xOff = 0;

	//TODO this value is being reused in check collision move it in a common location or eliminate collision detection when drawing completely
	int princeXOffset = 22; 

	for(int y = Level::SCENE_HEIGHT_BLK; y >= 0; y--){
		for(int x = 0; x <= Level::SCENE_WIDTH_BLK; x++){

			yOff = Level::BLOCK_HEIGHT_PX * y;
			xOff = - Level::BLOCK_WIDTH_PX + Level::BLOCK_WIDTH_PX * x;

			switch(level->getSceneCodeByBlock(y, x)) {
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
			case '1':
				graphics.DrawSprite(xOff, yOff - getSprite("tileCornerLeft")->height, getSprite("tileCornerLeft"));
				break;
			case '~': {
					std::map<std::pair<int, int>, Entity*>* entities = level->getEntities();
					int blockX = level->getLevelBlockX(x);
					int blockY = level->getLevelBlockY(y);
					std::pair<int, int> platformKey = std::make_pair(blockY, blockX);
					Entity* e = entities->at(platformKey);
					Platform* p = dynamic_cast<Platform*>(e);
					if (p != NULL) {
						p->Animate(&graphics);
						if (p->getState() == dislodged) {
							p->setY(p->getY() + 5); //TODO needs to be a variable
							if (level->getSceneCodeByCoord(p->getX(), p->getY() - 70) == '_') {
								level->newSetCodeByCoord(p->getX(), p->getY(), '$');
								level->setSceneCodeByBlock(x, y, ' ');
							}
						}
						
					}
					if (level->getSceneBlockXByCoord(prince->getMidX() - princeXOffset) == x &&
						level->getSceneBlockYByCoord(prince->getMidY()) == y) {
						p->Drop();
					}
				} break;

			case 'H':
				graphics.DrawSprite(xOff, yOff - getSprite("tileCornerLeft")->height, getSprite("tileCornerLeft"));
				break;
			case '-':
				if(level->getSceneCodeByCoord(prince->getMidX() - princeXOffset, prince->getMidY()) == '-') {
					if (level->getSceneBlockXByCoord(prince->getMidX() - princeXOffset) == x &&
						level->getSceneBlockYByCoord(prince->getMidY() == y) &&
						!prince->isJumping() ) {
						graphics.DrawSprite(xOff, yOff - getSprite("tileCornerLeft")->height, getSprite("tileCornerLeft"));
					}
					else {
						graphics.DrawSprite(xOff, yOff - getSprite("activate")->height, getSprite("activate"));
					}
				} else {
					graphics.DrawSprite(xOff, yOff - getSprite("activate")->height, getSprite("activate"));
				}
				break;
			case '=':
				if (level->getSceneCodeByCoord(prince->getMidX() - princeXOffset, prince->getMidY()) == '=') {
					if (level->getSceneBlockXByCoord(prince->getMidX() - princeXOffset) == x && 
						level->getSceneBlockYByCoord(prince->getMidY() == y) &&
						!prince->isJumping()
						) {
						graphics.DrawSprite(xOff, yOff + 2 - getSprite("trap")->height, getSprite("trap"));
					}
					else {
						graphics.DrawSprite(xOff, yOff - getSprite("trap")->height, getSprite("trap"));
					}
				}
				else {
					graphics.DrawSprite(xOff, yOff - getSprite("trap")->height, getSprite("trap"));
				}
				break;
			case '*':
				graphics.DrawSprite(xOff, yOff - getSprite("bricks")->height, getSprite("bricks"));
				break;
			case '/':
				graphics.DrawSprite(xOff, yOff - getSprite("holyFloor")->height, getSprite("holyFloor"));
				break;
			case 'S':
				graphics.DrawSprite(xOff, yOff - getSprite("deadSk")->height, getSprite("deadSk"));
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
			case 'K':
				graphics.DrawSprite(xOff, yOff - getSprite("tileCornerLeft")->height, getSprite("tileCornerLeft"));
				break;
			case 'E':
				graphics.DrawSprite(xOff, yOff - getSprite("tileCornerLeft")->height, getSprite("tileCornerLeft"));
				break;
			case '$':
				graphics.DrawSprite(xOff, yOff - getSprite("tileCornerLeft")->height, getSprite("tileCornerLeft"));
				graphics.DrawSprite(xOff, yOff - getSprite("rubbleBack")->height, getSprite("rubbleBack"));
				break;
			case '^':
				graphics.DrawSprite(xOff, yOff - getSprite("tileCornerLeft")->height, getSprite("tileCornerLeft"));
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


	int mask = 20;
	for(int i = 3; i >= 0; i--){
		for(int j = 0; j < 11; j++){

			yOff = Level::BLOCK_HEIGHT_PX * i;
			xOff = - Level::BLOCK_WIDTH_PX + Level::BLOCK_WIDTH_PX * j;

			switch(level->getSceneCodeByBlock(i, j)) {			
			case '_': //or
			case '#': //or
			case 'P': //or
			case 'K': //or				
			case 'E': //or
			case '^': //or
			case '!':
				if (level->isEmptySpace(level->getSceneCodeByBlock(i, j - 1))) {
					graphics.DrawSprite(xOff, yOff - getSprite("tileCornerLeft")->height + mask, getSprite("tileCornerLeft"), 0, mask, getSprite("tileCornerLeft")->width - 60, getSprite("tileCornerLeft")->height - mask);
				}
				break;
			case 'T':
				//graphics.DrawSprite(xOff, yOff - getSprite("tileCornerLeft")->height + mask, getSprite("tileCornerLeft"), 0, mask, getSprite("tileCornerLeft")->width, getSprite("tileCornerLeft")->height - mask);
				graphics.DrawSprite(xOff, yOff - getSprite("columnFront")->height, getSprite("columnFront"));
				break;
			case '$':
				graphics.DrawSprite(xOff, yOff - getSprite("rubbleFront")->height, getSprite("rubbleFront"));
				break;
			case '[':
				graphics.DrawSprite(xOff, yOff - getSprite("blockCornerLeft")->height, getSprite("blockCornerLeft"));
				break;
			case 'O':
				graphics.DrawSprite(xOff, yOff - getSprite("blockRound")->height, getSprite("blockRound"));
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
void Game::Reset() {
	level->loadLevel(1);

	prince = new Prince();
	//prince->setX(10 * Level::BLOCK_WIDTH_PX - 50);
	prince->setX(7 * Level::BLOCK_WIDTH_PX - 70);
	prince->setY(prince->getAnim()->getSheet()->getFrameHeight() - Level::FOOT_FLOAT - Level::BLOCK_HEIGHT_PX + 12); // Magic Number should be size of ledge?
	prince->setState(sIdle);
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
		std::string y(1,scene[nBlockY][nBlockX + 1]);
		s += y;
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
		for(int y = 0; y< 3; y++) {
			graphics.DrawSprite(y  * BLOCK_WIDTH_PX, yOff - block.height, &block);
		}
		graphics.DrawSprite(3 * BLOCK_WIDTH_PX - 1, yOff - blockCornerRight.height + 2, &blockCornerRight);

		//separatrs
		for(int y = 0; y< 5; y++) {
			graphics.DrawSprite((4 + y) * BLOCK_WIDTH_PX - 2, yOff - separator.height, &separator);
		}

		//floors
		for(int y = 0; y< 5; y++) {
			graphics.DrawSprite((4+ y) * BLOCK_WIDTH_PX, yOff + 1 - separator.height - floor.height, &floor);
		}

		//columns
		graphics.DrawSprite(5 * BLOCK_WIDTH_PX + 15, yOff - columnFront.height - separator.height + 1, &columnFront);
		graphics.DrawSprite(5 * BLOCK_WIDTH_PX + 14 + columnFront.width, yOff - columnBack.height - separator.height - 7 + 1, &columnBack);

		//blocks
		graphics.DrawSprite(9 * BLOCK_WIDTH_PX, yOff - blockCornerLeft.height, &blockCornerLeft);


	//level 2
	yOff = TOP_MARGIN + BLOCK_HEIGHT_PX * 2;

		//blocks
		for(int y = 0; y < 4; y++) {
			graphics.DrawSprite(0 + (9 - y) * BLOCK_WIDTH_PX, yOff - block.height, &block);
		}

		graphics.DrawSprite(0 + 5 * BLOCK_WIDTH_PX, yOff - blockCornerLeft.height, &blockCornerLeft);
		graphics.DrawSprite(0  - BLOCK_WIDTH_PX - 1, yOff - blockCornerRight.height, &blockCornerRight);

		//separators
		for(int y = 0; y< 3; y++) {
			graphics.DrawSprite(y * BLOCK_WIDTH_PX - 2, yOff - separator.height, &separator);
		}
		graphics.DrawSprite(3 * BLOCK_WIDTH_PX - 4, yOff - separatorCorner.height + 2, &separatorCorner);
		
		//floors
		for(int y = 0; y< 3; y++) {
			graphics.DrawSprite(y * BLOCK_WIDTH_PX, yOff + 1 - separator.height - floor.height, &floor);
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

		for(int y = 0; y< 5; y++) {
			graphics.DrawSprite(0 + (7 - y) * BLOCK_WIDTH_PX, yOff - separator.height, &separator);
			
		}

		//floors
		for(int y = 0; y< 5; y++) {
			graphics.DrawSprite(1 + (y + 3) * BLOCK_WIDTH_PX, yOff + 1 - separator.height - floor.height, &floor);
		}

		//blocks
		graphics.DrawSprite(0 + 9 * BLOCK_WIDTH_PX, yOff - block.height, &block);
		graphics.DrawSprite(0 + 8 * BLOCK_WIDTH_PX, yOff - blockCornerLeft.height, &blockCornerLeft);


	//Level 4

		//separators
		for(int y =0; y < SCENE_WIDTH_BLK; y++) {
			graphics.DrawSprite(y * BLOCK_WIDTH_PX, TOP_MARGIN - separator.height, &separator);
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