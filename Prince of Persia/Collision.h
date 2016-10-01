#pragma once

class Prince;

class Collision {

public:
	Collision(Level* level, Graphics* graphics, Input * input);

	void CheckCollision();
	void CheckCharacterCollision(Character* character);
	void CheckPrinceCollision(Prince* prince);
	

private:
	Level* level;
	Graphics* graphics;
	Timer* timer;
	Input* input;

	bool CheckCatchConditions(Prince* prince, double mY);
	void CheckLedgeClimb(Prince* prince, double *mX, double *mY);
	void CheckSceneChange(Prince* prince);
	void CheckFall(Prince* prince);
	void CheckStepDanger(Prince* prince, double* mX, double* mY);
	void CheckWallCollision(Prince* prince, double* mX, double* mY);

	void CheckGateCollision(Character* prince, double* mX);
	void CheckSpikeCollision(Character* prince);

};