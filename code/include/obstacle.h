#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class Obstacle {
public:
    Obstacle(float x, float y, float w, float h, Color color, float speed, int dir=1);

    virtual void update(float dt) = 0;       // pure virtual
    virtual void draw(RenderWindow& window) = 0; // pure virtual
    virtual Obstacle* clone() const = 0; 

    void setPosition(float x, float y);
    FloatRect getBounds();
    bool isOffScreen();
    float getSpeedX();
    void setSpeed(float s) { speed = s; }

protected:
    RectangleShape shape;
    float speed;
    int direction; // +1 right, -1 left
};
