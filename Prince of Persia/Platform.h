#pragma once

enum platformState { lodged, dislodged, dislodging, broken };
class Platform : public Entity {
private:
	platformState state;
	Animation* platformMove;
public:
	Platform(int x, int y);
	void Delete();
	void Move();
	void Drop();
	void Animate(Graphics* graphics);
	platformState getState();
};