#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class Lane {
public:
    enum Type { SAFE, ROAD, RIVER };

    Lane(float x, float y, float width, float tileSize, Type t);
    void draw(RenderWindow& window);
    Type getLaneType() const ;
    float getY() const;
    FloatRect getBounds() const { return shape.getGlobalBounds(); }

private:
    RectangleShape shape;
    Type laneType;
};
