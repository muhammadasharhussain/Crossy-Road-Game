#include "collision.h"
#include <vector>
using namespace std;
using namespace sf;

bool Collision::checkPlayerObstacle(RectangleShape& player, vector<Obstacle*>& obstacles) {
    FloatRect playerBounds = player.getGlobalBounds();

    for (auto* obs : obstacles) {
        if (playerBounds.findIntersection(obs->getBounds()))
            return true;
    }
    return false;
}