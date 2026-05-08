#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class Obstacle {
public:
    Obstacle(float x, float y, float w, float h, Color color, float speed, int dir=1);

    virtual void update(float dt) = 0;       // pure virtual
    virtual void draw(RenderWindow& window) = 0; // pure virtual

    FloatRect getBounds();
    bool isOffScreen();
    float getSpeedX();

protected:
    RectangleShape shape;
    float speed;
    int direction; // +1 right, -1 left
};