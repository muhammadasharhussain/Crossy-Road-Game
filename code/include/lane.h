#pragma once
#include <SFML/Graphics.hpp>
#include <string>
using namespace sf;

class Lane {
public:
    enum Type { SAFE, ROAD, RIVER };

    Lane(float x, float y, float width, float tileSize, Type t);
    Lane(const Lane& other);
    ~Lane();

    void draw(RenderWindow& window);
    Type getLaneType() const;
    float getY() const;
    FloatRect getBounds() const;
    Lane& operator=(const Lane& other);

private:
    Texture texture;
    Sprite* sprite;
    Type laneType;
    float y;
    float w;
    float h;

    static std::string textureFor(Type t);
};
