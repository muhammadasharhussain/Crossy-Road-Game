#include "staticobstacle.h"

Tree::Tree(float x, float y)
    : Obstacle(x, y, 40.f, 40.f, "..\\assets\\images\\tree.png", 0.f) {
}

void Tree::update(float dt) {}

void Tree::draw(RenderWindow& window) {
    window.draw(*sprite);
}

Rock::Rock(float x, float y)
    : Obstacle(x, y, 40.f, 40.f, "..\\assets\\images\\rock.png", 0.f) {
}

void Rock::update(float dt) {}

void Rock::draw(RenderWindow& window) {
    window.draw(*sprite);
}
