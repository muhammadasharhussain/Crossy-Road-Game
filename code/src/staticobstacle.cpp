#include "staticobstacle.h"

Tree::Tree(float x, float y)
    : Obstacle(x, y, 40.f, 40.f, Color(0, 100, 0), 0.f) {
}

void Tree::update(float dt) {
    // static, does nothing
}

void Tree::draw(RenderWindow& window) {
    window.draw(shape);
}


Rock::Rock(float x, float y)
    : Obstacle(x, y, 40.f, 40.f, Color(105, 105, 105), 0.f) {
}

void Rock::update(float dt) {
    // static, does nothing
}

void Rock::draw(RenderWindow& window) {
    window.draw(shape);
}