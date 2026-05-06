#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class Vehicle {
public:
    enum Size { SMALL, MEDIUM, LARGE };

    Vehicle(float x, float y, float speed, float tileSize, Size size);
    void update(float dt, float windowWidth);
    void draw(RenderWindow& window);
    FloatRect getBounds();

private:
    RectangleShape shape;
    float speed, startX, waitTimer, waitDuration;
    bool waiting;
};