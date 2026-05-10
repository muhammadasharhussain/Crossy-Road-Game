#include "movingobstacle.h"
#include "constants.h"

sf::Vector2f Vehicle::sizeFor(VehicleType t) {
    switch (t) {
        case VehicleType::BIKE:     return {TILE * 0.8f,  TILE * 0.5f};
        case VehicleType::CAR:      return {TILE * 1.5f,  TILE * 0.7f};
        case VehicleType::DUMPER:   return {TILE * 2.5f,  TILE * 0.85f};
        default:                    return {TILE, TILE};
    }
}

std::string Vehicle::textureFor(VehicleType t) {
    switch (t) {
        case VehicleType::BIKE:     return "..\\assets\\images\\bike.png";
        case VehicleType::CAR:      return "..\\assets\\images\\car.png";
        case VehicleType::DUMPER:   return "..\\assets\\images\\dumper.png";
        default:                    return "..\\assets\\images\\bike.png";
    }
}

Vehicle::Vehicle(float x, float y, float speed, int direction, VehicleType type, float windowWidth)
    : Obstacle(x, y, sizeFor(type).x, sizeFor(type).y, textureFor(type), speed, direction),
      windowWidth(windowWidth), vehicleType(type) {
    objWidth = sizeFor(type).x;
    applyFlip();
}

void Vehicle::update(float dt) {
    sprite->move({speed * direction * dt, 0.f});
    float x = sprite->getPosition().x;
    if (direction == 1 && x > windowWidth + 10.f)
        sprite->setPosition({-objWidth, sprite->getPosition().y});
    else if (direction == -1 && x < -objWidth - 10.f)
        sprite->setPosition({windowWidth + 10.f, sprite->getPosition().y});
}

void Vehicle::draw(RenderWindow& window) {
    window.draw(*sprite);
}

Log::Log(float x, float y, float speed, int direction, float windowWidth)
    : Obstacle(x, y, TILE * 2.5f, TILE * 0.85f,
        "..\\assets\\images\\log.png", speed, direction),
      windowWidth(windowWidth) {
    objWidth = TILE * 2.5f;
    applyFlip();
}

void Log::update(float dt) {
    sprite->move({speed * direction * dt, 0.f});
    float x = sprite->getPosition().x;
    if (direction == 1 && x > windowWidth + 10.f)
        sprite->setPosition({-objWidth, sprite->getPosition().y});
    else if (direction == -1 && x < -objWidth - 10.f)
        sprite->setPosition({windowWidth + 10.f, sprite->getPosition().y});
}

void Log::draw(sf::RenderWindow& window) {
    window.draw(*sprite);
}
