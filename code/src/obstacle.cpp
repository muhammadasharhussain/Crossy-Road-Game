#include <SFML/Graphics.hpp>
#include "obstacle.h"

Obstacle::Obstacle(float x, float y, float w, float h, Color color, float speed, int dir) {
    this->speed = speed;
    direction = dir;
    shape.setSize({w, h-5.0f}); // slight padding to prevent perfect edge collisions
    shape.setPosition(sf::Vector2f({x, y}));
    shape.setFillColor(color);
}

FloatRect Obstacle::getBounds() {
    return shape.getGlobalBounds();
}

bool Obstacle::isOffScreen() {
    float x = shape.getPosition().x;
    return (x > 900.f || x < -200.f);
}

float Obstacle::getSpeedX() {
    return speed * direction;
}

void Obstacle::setPosition(float x, float y) {
    shape.setPosition(sf::Vector2f(x, y));
}
