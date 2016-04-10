#pragma once

class FinishDoor : public Gate {
public:
	FinishDoor(int x, int y, int levelX, int levelY);
	void Animate(Graphics* graphics);
	void Open();
	bool Finish();
private:
	Sprite* door;
	Sprite* doorBase;
	Animation* enter;

	int doorOffsetX;
	int doorOffsetY;

	int yCut;
	int cutLimit;
	int frameTime;
};