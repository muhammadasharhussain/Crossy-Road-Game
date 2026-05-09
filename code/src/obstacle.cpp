#include "obstacle.h"
#include <cmath>
#include <cstdio>

Obstacle::Obstacle(float x, float y, float w, float h, const std::string& texturePath, float speed, int dir) {
    this->speed = speed;
    this->direction = dir;

    bool loaded = texture.loadFromFile(texturePath);
    if (!loaded) {
        printf("MISSING: %s\n", texturePath.c_str());
        fflush(stdout);
        sf::Image img({1, 1}, sf::Color::White);
        texture.loadFromImage(img);
    }

    sprite = new Sprite(texture);
    Vector2u texSize = texture.getSize();
    sprite->setScale({w / (float)texSize.x, (h - 5.f) / (float)texSize.y});
    sprite->setPosition({x, y});
}

Obstacle::Obstacle(const Obstacle& other)
    : speed(other.speed), direction(other.direction), texture(other.texture) {
    sprite = new Sprite(texture);
    sprite->setScale(other.sprite->getScale());
    sprite->setPosition(other.sprite->getPosition());
}

Obstacle::~Obstacle() {
    delete sprite;
}

void Obstacle::applyFlip() {
    auto scale = sprite->getScale();
    if (direction == 1) {
        sprite->setScale({-std::abs(scale.x), scale.y});
        sprite->setOrigin({(float)texture.getSize().x, 0.f});
    } else {
        sprite->setScale({std::abs(scale.x), scale.y});
        sprite->setOrigin({0.f, 0.f});
    }
}

FloatRect Obstacle::getBounds() {
    return sprite->getGlobalBounds();
}

bool Obstacle::isOffScreen() {
    float x = sprite->getPosition().x;
    return (x > 900.f || x < -200.f);
}

float Obstacle::getSpeedX() {
    return speed * direction;
}

void Obstacle::setPosition(float x, float y) {
    sprite->setPosition({x, y});
}
