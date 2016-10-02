#include "Prince.h"
#include "Graphics.h"
#include "Level.h"
#include "Timer.h"
#include "FinishDoor.h"
#include "Platform.h"
#include "Collision.h"


Collision::Collision(Level* level, Graphics* graphics, Input* input) {
	this->level = level;
	this->graphics = graphics;
	this->input = input;

	this->timer = new Timer();
	this->timer->StartWatch();
}

void Collision::CheckCollision() {}

void Collision::CheckCharacterCollision(Character* character) {

	CheckFall(character);
	CheckWallCollision(character);
	CheckGateCollision(character);
	CheckSpikeCollision(character);

	double moveX = character->getDefferX();
	double moveY = character->getDefferY();

	character->setDefferX(0);
	character->setDefferY(0);

	// Move Character
	character->MoveX(moveX);
	character->MoveY(moveY);
}
void Collision::CheckPrinceCollision(Prince* prince) {

	this->timer->StopWatch();

	prince->setDefferX(prince->getDefferX() * this->timer->GetTimeMilli() / 14.5);
	prince->setDefferY(prince->getDefferY());

	this->timer->StartWatch();

	CheckLedgeClimb(prince);
	CheckSceneChange(prince);
	CheckFall(prince);
	CheckStepDanger(prince);
	CheckWallCollision(prince);
	CheckGateCollision(prince);
	CheckSpikeCollision(prince);

	if (input->isShiftPressed()) {
		if (CheckCatchConditions(prince)) {
			prince->Catch();
		}
	}

	double moveX = prince->getDefferX();
	double moveY = prince->getDefferY();

	prince->setDefferX(0);
	prince->setDefferY(0);

	prince->setAccX(prince->getAccX() * 0.94);
	prince->setAccY(prince->getAccY() * 1.01);

	if (prince->getAccY() > 10) { prince->setAccY(10); }
	if (prince->getAccX() > 20) { prince->setAccY(20); }

	// Move Prince
	prince->MoveX(moveX);
	prince->MoveY(moveY);
}

void Collision::CheckGateCollision(Character * character) {

	//TODO clean this up

	// Calculate foot position
	int xFoot = character->getX() + character->getAnim()->getSheet()->getFrameWidth() / 2 - /*36*/ 16;
	int yFoot = character->getY() + character->getAnim()->getSheet()->getFrameHeight() - 9;

	int nBlockX = level->getSceneBlockXByCoord(xFoot);
	int nBlockY = level->getSceneBlockYByCoord(yFoot);

	char code = level->getSceneCodeByCoord(character->getMidX() - 22, character->getMidY());
	int absX = level->getLevelBlockX(level->getSceneBlockXByCoord(character->getMidX() - 22));
	int absY = level->getLevelBlockY(level->getSceneBlockYByCoord(character->getMidY()));
	std::map<std::pair<int, int>, Entity*>* entitites = level->getEntities();

	////================== Open/Close Gates ==================
	if ( (code == '-' || code == '=') && character->getState() != sJumping) {

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


	// ================== Gate Collision ==================
	 
	{
		int xOffset = 0;
		nBlockX = level->getSceneBlockXByCoord(character->getMidX() + xOffset);
		nBlockY = level->getSceneBlockYByCoord(character->getMidY());

		if (level->getSceneCodeByBlock(nBlockY, nBlockX) == 'G') {

			std::map<std::pair<int, int>, Entity*>* entitites = level->getEntities();
			int absY = level->getLevelBlockY(nBlockY);
			int absX = level->getLevelBlockX(nBlockX);

			std::pair<int, int> gateKey(absY, absX);
			Gate* g = dynamic_cast<Gate*>((*entitites)[gateKey]);

			if (g != NULL && !g->isOpen()) {
				int bar = (Level::BLOCK_WIDTH_PX * (nBlockX));

				if (DEBUG) graphics->DrawLine(bar, 0, bar, Graphics::SCREENY, 255, 255, 255);
				if (DEBUG) graphics->DrawLine(character->getMidX() + xOffset, 0, character->getMidX() + xOffset, Graphics::SCREENY, 0, 255, 0);

				if (character->getDefferX() > 0 && character->getMidX() + xOffset < bar && character->getMidX() + xOffset + character->getDefferX() >= bar) {
					character->setDefferX(0);
				}

				if (character->getDefferX() < 0 && character->getMidX() + xOffset > bar && character->getMidX() + xOffset + character->getDefferX() <= bar) {
					character->setDefferX(0);
				}
			}
		}
	}
	
	{
		int xOffset = -20;
		nBlockX = level->getSceneBlockXByCoord(character->getMidX() + xOffset);
		nBlockY = level->getSceneBlockYByCoord(character->getMidY());
		nBlockX--;

		if (level->getSceneCodeByBlock(nBlockY, nBlockX) == 'G') {

			std::map<std::pair<int, int>, Entity*>* entitites = level->getEntities();
			int absY = level->getLevelBlockY(nBlockY);
			int absX = level->getLevelBlockX(nBlockX);

			std::pair<int, int> gateKey(absY, absX);
			Gate* g = dynamic_cast<Gate*>((*entitites)[gateKey]);

			if (g != NULL && !g->isOpen()) {
				int bar = (Level::BLOCK_WIDTH_PX * (nBlockX));

				if (DEBUG) graphics->DrawLine(bar, 0, bar, Graphics::SCREENY, 255, 255, 255);
				if (DEBUG) graphics->DrawLine(character->getMidX() + xOffset, 0, character->getMidX() + xOffset, Graphics::SCREENY, 0, 255, 0);

				if (character->getDefferX() > 0 && character->getMidX() + xOffset < bar && character->getMidX() + xOffset + character->getDefferX() >= bar) {
					character->setDefferX(0);
				}

				if (character->getDefferX() < 0 && character->getMidX() + xOffset > bar && character->getMidX() + xOffset + character->getDefferX() <= bar) {
					character->setDefferX(0);
				}
			}
		}
	}
}
void Collision::CheckFall(Character * character) {

	// Calculate foot position
	int xFoot = character->getX() + character->getAnim()->getSheet()->getFrameWidth() / 2 - /*36*/ 16;
	int yFoot = character->getY() + character->getAnim()->getSheet()->getFrameHeight() - 9;

	int nBlockX = level->getSceneBlockXByCoord(xFoot);
	int nBlockY = level->getSceneBlockYByCoord(yFoot);

	char code = level->getSceneCodeByBlock(nBlockY, nBlockX);

	int bar = (Level::BLOCK_HEIGHT_PX * nBlockY) - 20;
	if (DEBUG) graphics->DrawLine(0, bar, Graphics::SCREENX, bar, 255, 255, 255);

	if (character->getState() != sFalling && !character->isMovingUp()) {
		character->setY(character->getY() - yFoot + bar);
	}

	if (character->getState() == sFalling || character->isIdle() && level->isLedge(code)) {

		int bar = (Level::BLOCK_HEIGHT_PX * nBlockY) - 20;
		if (DEBUG) graphics->DrawLine(0, bar, Graphics::SCREENX, bar, 255, 255, 255);

		if (yFoot >= bar) {
			//moveY = 0;
			character->Land(level->getLevelBlockYByCoord(yFoot));
		}
		else {
			character->setFall(level->getLevelBlockYByCoord(yFoot));
		}
	}

	if (code == '~') {
		std::map<std::pair<int, int>, Entity*>* entities = level->getEntities();
		std::pair<int, int> platformKey = std::make_pair(level->getLevelBlockY(nBlockY), level->getLevelBlockX(nBlockX));
		Entity* e = entities->at(platformKey);
		Platform* p = dynamic_cast<Platform*>(e);
		if (p != NULL && p->getState() == dislodged) {
			character->setFall(level->getLevelBlockYByCoord(yFoot));
		}
	}

	if (level->isEmptySpace(code)) {
		character->setFall(level->getLevelBlockYByCoord(yFoot));
	}
}
void Collision::CheckWallCollision(Character * character) {

	int xFoot = character->getX() + character->getAnim()->getSheet()->getFrameWidth() / 2 - /*36*/ 16;
	int yFoot = character->getY() + character->getAnim()->getSheet()->getFrameHeight() - 9;

	int nBlockX = level->getSceneBlockXByCoord(xFoot);
	int nBlockY = level->getSceneBlockYByCoord(yFoot);

	if (character->getState() != sClimbUp && character->getState() != sJumpGrab) {
		if (level->getSceneCodeByBlock(nBlockY, nBlockX - 1) == ']') {
			int threshold = (Level::BLOCK_WIDTH_PX * (nBlockX - 1) + 20);

			if (DEBUG) graphics->DrawLine(threshold, 0, threshold, Graphics::SCREENY, 255, 255, 255);

			if (xFoot + character->getDefferX()  < threshold) {
				character->setDefferX(0);
				character->setAccX(0);
				character->MoveX(threshold - xFoot);
			}
		}

		if (level->getSceneCodeByBlock(nBlockY, nBlockX + 1) == '[' ||
			level->getSceneCodeByBlock(nBlockY, nBlockX) == '[' ||
			level->getSceneCodeByBlock(nBlockY, nBlockX + 1) == 'O' ||
			level->getSceneCodeByBlock(nBlockY, nBlockX) == 'O') {

			int threshold = (Level::BLOCK_WIDTH_PX * (nBlockX)-10);
			if (DEBUG) graphics->DrawLine(threshold, yFoot, threshold, Level::BLOCK_HEIGHT_PX, 255, 255, 255);

			if (xFoot + character->getDefferX() > threshold) {
				character->setDefferX(0);
				character->setAccX(0);
				character->MoveX(threshold - xFoot);
			}
		}
	}
}
void Collision::CheckSpikeCollision(Character * character) {
	// Calculate foot position
	int xFoot = character->getX() + character->getAnim()->getSheet()->getFrameWidth() / 2 - /*36*/ 16;
	int yFoot = character->getY() + character->getAnim()->getSheet()->getFrameHeight() - 9;

	int nBlockX = level->getSceneBlockXByCoord(xFoot);
	int nBlockY = level->getSceneBlockYByCoord(yFoot);

	char code = level->getSceneCodeByCoord(character->getMidX() - 22, character->getMidY());
	
	int absX = level->getLevelBlockX(level->getSceneBlockXByCoord(character->getMidX() - 22));
	int absY = level->getLevelBlockY(level->getSceneBlockYByCoord(character->getMidY()));
	level->findSpikes(absX, absY); //TODO

	if (code == '/') {
		if (character->getState() == sRunning ||
			character->getState() == sFalling) {

			nBlockX = level->getSceneBlockXByCoord(character->getMidX());
			nBlockY = level->getSceneBlockYByCoord(character->getMidY());
			character->setX((nBlockX - 1) * Level::BLOCK_WIDTH_PX - 15);
			character->setY((nBlockY - 1)* Level::BLOCK_HEIGHT_PX);
			character->spikeKill();
		}
	}
}

void Collision::CheckLedgeClimb(Prince* prince) {

	// Calculate foot position
	int xFoot = prince->getX() + prince->getAnim()->getSheet()->getFrameWidth() / 2 - /*36*/ 16;
	int yFoot = prince->getY() + prince->getAnim()->getSheet()->getFrameHeight() - 9;

	int nBlockX = level->getSceneBlockXByCoord(xFoot);
	int nBlockY = level->getSceneBlockYByCoord(yFoot);

	{
		int checkLedgeX = nBlockX + 1;
		int checkLedgeY = nBlockY - 1;
		int checkLedgeXOp = nBlockX - 1;
		if (!prince->isFacingRight()) {
			checkLedgeX = nBlockX - 1;
			checkLedgeXOp = nBlockX + 1;
		}

		if (prince->getState() == sJumpGrab) {

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
					prince->setDefferX(18);
				}
				else {
					prince->setDefferX(-18);
				}
			}
		}
	}
}
void Collision::CheckSceneChange(Prince * prince) {
	if (prince->getMidY() > Level::BLOCK_HEIGHT_PX * 3) { // down
		level->changeScene(D);
		prince->setY(-60);
	}

	if (prince->getMidY() < 0) { // up
		level->changeScene(U);
		prince->setY(Level::BLOCK_HEIGHT_PX * 3 - 70);
	}

	//xFoot + 28
	if (prince->getMidX() > Level::BLOCK_WIDTH_PX * 10) { // right
		level->changeScene(R);
		prince->setX(-40);
	}


	//xFoot
	if (prince->getMidX() < -7) { // left
		level->changeScene(L);
		prince->setX(Level::BLOCK_WIDTH_PX * 9 - 20);
	}
}
void Collision::CheckStepDanger(Prince * prince) {

	int xFoot = prince->getX() + prince->getAnim()->getSheet()->getFrameWidth() / 2 - /*36*/ 16;
	int yFoot = prince->getY() + prince->getAnim()->getSheet()->getFrameHeight() - 9;

	int nBlockX = level->getSceneBlockXByCoord(xFoot + prince->getDefferX() + 2);
	if (!prince->isFacingRight()) { nBlockX = level->getSceneBlockXByCoord(xFoot + prince->getDefferX() - 5); }
	int nBlockY = level->getSceneBlockYByCoord(yFoot);

	if (prince->getState() == sStep) {
		if (level->isEmptySpace(level->getSceneCodeByBlock(nBlockY, nBlockX))) {
			if (prince->getAnim()->getCurrentFrame() == 5) {
				prince->getAnim()->Reset();
				prince->setMissStep();
			}
			if (prince->getAnim()->getCurrentFrame() > 5) {
				prince->setDefferX(0);
			}
		}
	}
}
bool Collision::CheckCatchConditions(Prince* prince) {
	if (prince->getState() != sFalling) { return false; }

	int yFoot = prince->getY() + prince->getAnim()->getSheet()->getFrameHeight() - 9;

	int blockX = level->getLevelBlockXByCoord(prince->getMidX());
	int blockY = level->getLevelBlockYByCoord(yFoot);

	int barY = (blockY - 1) * Level::BLOCK_HEIGHT_PX + 30;

	if (DEBUG) {
		graphics->DrawLine(0, barY, 800, barY, 0, 255, 0);
		graphics->DrawLine(0, prince->getMidY(), 800, prince->getMidY(), 255, 0, 0);
		graphics->DrawLine(0, prince->getMidY() + prince->getDefferY(), 800, prince->getMidY() + prince->getDefferY(), 255, 0, 0);
	}

	int iMax = max(prince->getDefferY() + prince->getMidY(), prince->getMidY());
	int iMin = min(prince->getDefferY() + prince->getMidY(), prince->getMidY());
	if (!(iMin < barY && barY < iMax || barY == iMax || barY == iMin)) { return false; }



	//prince->MoveY(barY - prince->getMidY());
	//prince->setDefferY(0);
	int ledgeX = blockX - 1;;
	if (prince->isFacingRight()) {
		ledgeX = blockX + 1;
	}

	blockY--; //look above

	if (level->isLedge(level->getLevelCodeByBlock(ledgeX, blockY)) &&
		level->isEmptySpace(level->getLevelCodeByBlock(blockX, blockY))) {
		prince->MoveY(barY - prince->getMidY());
		int offset = -30;
		if (prince->isFacingRight()) { offset = 0; }
		prince->setX((blockX - 1) * Level::BLOCK_WIDTH_PX + offset);
		return true;
	}


	return false;
}
