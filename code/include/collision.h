#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "obstacle.h"
using namespace std;
using namespace sf;

class Collision {
public:
    static bool checkPlayerObstacle(RectangleShape& player, vector<Obstacle*>& obstacles);
};