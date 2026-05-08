#include "movingobstacle.h"
#include "constants.h"

// size and color per vehicle type
sf::Vector2f Vehicle::sizeFor(VehicleType t) {
    switch (t) {
        case VehicleType::BIKE:     return sf::Vector2f(TILE * 0.8f,  TILE * 0.5f);
        case VehicleType::RICKSHAW: return sf::Vector2f(TILE * 1.5f,  TILE * 0.7f);
        case VehicleType::DUMPER:   return sf::Vector2f(TILE * 2.5f,  TILE * 0.85f);
        default:                    return sf::Vector2f(TILE,          TILE);
    }
}

sf::Color Vehicle::colorFor(VehicleType t) {
    switch (t) {
        case VehicleType::BIKE:     return sf::Color(255, 100, 100);   // red
        case VehicleType::RICKSHAW: return sf::Color(255, 200, 0);     // yellow
        case VehicleType::DUMPER:   return sf::Color(100, 100, 255);   // blue
        default:                    return sf::Color::White;
    }
}

Vehicle::Vehicle(float x, float y, float speed, int direction, VehicleType type, float windowWidth)
    : Obstacle(x, y, sizeFor(type).x, sizeFor(type).y, colorFor(type), speed, direction),
      windowWidth(windowWidth), vehicleType(type)
{}

void Vehicle::update(float dt) {
    shape.move(sf::Vector2f(speed * direction * dt, 0.f));

    float x = shape.getPosition().x;
    if (direction == 1 && x > windowWidth + 10.f)
        shape.setPosition(sf::Vector2f(-shape.getSize().x, shape.getPosition().y));
    else if (direction == -1 && x < -shape.getSize().x - 10.f)
        shape.setPosition(sf::Vector2f(windowWidth + 10.f, shape.getPosition().y));
}

void Vehicle::draw(RenderWindow& window) {
    window.draw(shape);
}

Log::Log(float x, float y, float speed, int direction, float windowWidth)
    : Obstacle(x, y, TILE * 2.5f, TILE * 0.7f, sf::Color(139, 90, 43), speed, direction),
      windowWidth(windowWidth)
{}

void Log::update(float dt) {
    shape.move(sf::Vector2f(speed * direction * dt, 0.f));

    float x = shape.getPosition().x;
    if (direction == 1 && x > windowWidth + 10.f)
        shape.setPosition(sf::Vector2f(-shape.getSize().x, shape.getPosition().y));
    else if (direction == -1 && x < -shape.getSize().x - 10.f)
        shape.setPosition(sf::Vector2f(windowWidth + 10.f, shape.getPosition().y));
}

void Log::draw(sf::RenderWindow& window) {
    window.draw(shape);
}
