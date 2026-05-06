#include "../include/vehicle.h"
#include <cstdlib>

Vehicle::Vehicle(float x, float y, float speed, float tileSize, Size size) {
    this->speed = speed;
    this->waiting = false;
    this->waitTimer = 0.f;
    this->waitDuration = 1.f + (rand() % 300) / 100.f;

    float w, h;
    if (size == SMALL) {
        w = tileSize * 1.2f; h = tileSize * 0.7f;
        shape.setFillColor(Color(200, 50, 50));
    } else if (size == MEDIUM) {
        w = tileSize * 2.0f; h = tileSize * 0.75f;
        shape.setFillColor(Color(50, 50, 200));
    } else {
        w = tileSize * 3.0f; h = tileSize * 0.8f;
        shape.setFillColor(Color(180, 120, 30));
    }

    shape.setSize({w, h});
    shape.setPosition({x, y + (tileSize - h) / 2.f});
    startX = (speed > 0) ? -w : 550.f; // respawn point
}

void Vehicle::update(float dt, float windowWidth) {
    if (waiting) {
        waitTimer -= dt;
        if (waitTimer <= 0.f) {
            waiting = false;
            shape.setPosition({startX, shape.getPosition().y});
        }
        return;
    }

    shape.move({speed * dt, 0.f});

    float x = shape.getPosition().x;
    float w = shape.getSize().x;

    if (speed > 0 && x > windowWidth) {
        waiting = true;
        waitTimer = waitDuration;
    } else if (speed < 0 && x + w < 0) {
        waiting = true;
        waitTimer = waitDuration;
    }
}

void Vehicle::draw(RenderWindow& window) {
    if (!waiting)
        window.draw(shape);
}

FloatRect Vehicle::getBounds() {
    return shape.getGlobalBounds();
}