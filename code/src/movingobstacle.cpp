#include "movingobstacle.h"

Vehicle::Vehicle(float x, float y, float speed, int direction, float size)
    : Obstacle(x, y, size * 2, size, Color::Yellow, speed) {
    this->direction = direction;
}

void Vehicle::update(float dt) {
    shape.move({speed * direction * dt, 0.f});

    // wraparound when off screen
    float x = shape.getPosition().x;
    if (direction == 1 && x > windowWidth + 10.f)
        shape.setPosition(sf::Vector2f(-shape.getSize().x, shape.getPosition().y));
    else if (direction == -1 && x < -shape.getSize().x - 10.f)
        shape.setPosition(sf::Vector2f(windowWidth + 10.f, shape.getPosition().y));
}

void Vehicle::draw(RenderWindow& window) {
    window.draw(shape);
}
