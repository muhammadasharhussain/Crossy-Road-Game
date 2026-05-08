#pragma once
#include "obstacle.h"

enum class VehicleType { BIKE, RICKSHAW, DUMPER };

class Vehicle : public Obstacle {
public:
    Vehicle(float x, float y, float speed, int direction, VehicleType type, float windowWidth);
    void update(float dt) override;
    void draw(RenderWindow& window) override;
    Obstacle* clone() const override { 
        return new Vehicle(*this); 
    }
    // movingobstacle.h — add inside Vehicle class
    void setDirection(int d) { direction = d; }

private:
    float windowWidth;
    VehicleType vehicleType;

    static sf::Color colorFor(VehicleType t);
    static sf::Vector2f sizeFor(VehicleType t);
};

class Log : public Obstacle {
    public:
        Log(float x, float y, float speed, int direction, float windowWidth);
        void update(float dt) override;
        void draw(RenderWindow& window) override;
        Obstacle* clone() const override { return new Log(*this); }
        void setDirection(int d) { direction = d; }
    
    private:
        float windowWidth;
};
