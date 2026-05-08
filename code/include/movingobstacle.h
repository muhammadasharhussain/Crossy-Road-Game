#pragma once
#include "obstacle.h"

class Vehicle : public Obstacle {
public:
    Vehicle(float x, float y, float speed, int direction, float size);
    void update(float dt) override;
    void draw(RenderWindow& window) override;
private:
    float windowWidth = 550.f;
};
