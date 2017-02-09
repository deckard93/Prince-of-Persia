#pragma once

enum platformState { lodged, dislodged, dislodging, broken };
class Platform : public Entity {
private:
    double xOffset;
    double yOffset;
    platformState state;
    Animation* platformMove;
    static const int animDisplayTime = 160;
public:
    double getXOffset();
    double getYOffset();
    void setXOffset(double x);
    void setYOffset(double y);

    Platform(int x, int y);
    void Delete();
    void Move();
    void Drop();
    void Animate(Graphics* graphics);
    platformState getState();
    void setState(platformState state);
};