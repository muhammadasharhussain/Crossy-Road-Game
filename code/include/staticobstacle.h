#pragma once
#include "obstacle.h"

class Tree : public Obstacle {
public:
    Tree(float x, float y);
    void update(float dt) override;
    void draw(RenderWindow& window) override;
    Obstacle* clone() const override { 
        return new Tree(*this); 
    }
};

class Rock : public Obstacle {
public:
    Rock(float x, float y);
    void update(float dt) override;
    void draw(RenderWindow& window) override;
    Obstacle* clone() const override { 
        return new Rock(*this); 
    }
};
