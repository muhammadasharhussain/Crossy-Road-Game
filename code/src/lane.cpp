#include "lane.h"
#include <cstdio>

std::string Lane::textureFor(Type t) {
    switch (t) {
        case SAFE:  return "..\\assets\\images\\safe.png";
        case ROAD:  return "..\\assets\\images\\road.png";
        case RIVER: return "..\\assets\\images\\river.png";
        default:    return "..\\assets\\images\\safe.png";
    }
}

Lane::Lane(float x, float y, float width, float tileSize, Type t)
    : laneType(t), y(y), w(width), h(tileSize) {

    bool loaded = texture.loadFromFile(textureFor(t));
    if (!loaded) {
        printf("LANE MISSING: %s\n", textureFor(t).c_str());
        fflush(stdout);
        sf::Image img({1, 1}, sf::Color::White);
        texture.loadFromImage(img);
    }

    sprite = new Sprite(texture);
    Vector2u texSize = texture.getSize();
    if (texSize.x > 0 && texSize.y > 0)
        sprite->setScale({width / (float)texSize.x, tileSize / (float)texSize.y});
    sprite->setPosition({x, y});
}

Lane::Lane(const Lane& other)
    : laneType(other.laneType), y(other.y), w(other.w), h(other.h),
      texture(other.texture) {
    sprite = new Sprite(texture);
    sprite->setScale(other.sprite->getScale());
    sprite->setPosition(other.sprite->getPosition());
}

Lane::~Lane() {
    delete sprite;
}

void Lane::draw(RenderWindow& window) {
    window.draw(*sprite);
}

Lane::Type Lane::getLaneType() const {
    return laneType;
}

float Lane::getY() const {
    return y;
}
Lane& Lane::operator=(const Lane& other) {
    if (this != &other) {
        delete sprite;
        laneType = other.laneType;
        y = other.y;
        w = other.w;
        h = other.h;
        texture = other.texture;
        sprite = new Sprite(texture);
        sprite->setScale(other.sprite->getScale());
        sprite->setPosition(other.sprite->getPosition());
    }
    return *this;
}

FloatRect Lane::getBounds() const {
    return sprite->getGlobalBounds();
}
