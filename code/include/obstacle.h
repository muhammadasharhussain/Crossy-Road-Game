#pragma once
#include <SFML/Graphics.hpp>
#include <string>
using namespace sf;

class Obstacle {
public:
    Obstacle(float x, float y, float w, float h, const std::string& texturePath, float speed, int dir=1);
    Obstacle(const Obstacle& other);
    virtual ~Obstacle();

    virtual void update(float dt) = 0;
    virtual void draw(RenderWindow& window) = 0;
    virtual Obstacle* clone() const = 0;

    void setPosition(float x, float y);
    FloatRect getBounds();
    bool isOffScreen();
    float getSpeedX();
    void setSpeed(float s) { speed = s; }
    void applyFlip();

protected:
    Texture texture;
    Sprite* sprite;
    float speed;
    int direction;
};
