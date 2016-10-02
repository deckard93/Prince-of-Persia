#pragma once


class NormalInput {
private:
	Input* input;

public:
	NormalInput(Input* input);
	bool doLeftStep();
	bool doRightStep();
	bool doRightJump();
	bool doLeftJump();
	bool doClimb();
	bool goDown();
	bool goLeft();
	bool goRight();
	bool Strike();
	bool Hang();
};