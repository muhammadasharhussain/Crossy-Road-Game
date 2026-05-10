#pragma once
#include "obstacle.h"
#include "constants.h"
#include <string>

enum class VehicleType { BIKE, CAR, DUMPER };

class Vehicle : public Obstacle {
public:
    Vehicle(float x, float y, float speed, int direction, VehicleType type, float windowWidth);
    void update(float dt) override;
    void draw(RenderWindow& window) override;
    Obstacle* clone() const override { return new Vehicle(*this); }
    void setDirection(int d) { direction = d; applyFlip(); }

private:
    float windowWidth;
    float objWidth;
    VehicleType vehicleType;

    static std::string textureFor(VehicleType t);
    static sf::Vector2f sizeFor(VehicleType t);
};

class Log : public Obstacle {
public:
    Log(float x, float y, float speed, int direction, float windowWidth);
    void update(float dt) override;
    void draw(RenderWindow& window) override;
    Obstacle* clone() const override { return new Log(*this); }
    void setDirection(int d) { direction = d; applyFlip(); }

private:
    float windowWidth;
    float objWidth;
};
