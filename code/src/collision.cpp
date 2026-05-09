#include "collision.h"

bool Collision::checkStaticBlocking(sf::FloatRect playerBounds,
                                     sf::Vector2f attemptedMove,
                                     const std::vector<Obstacle*>& obstacles) {
    playerBounds.position.x += attemptedMove.x;
    playerBounds.position.y += attemptedMove.y;

    for (auto* obs : obstacles) {
        if (dynamic_cast<Vehicle*>(obs) || dynamic_cast<Log*>(obs))
            continue;
        if (playerBounds.findIntersection(obs->getBounds()))
            return true;
    }
    return false;
}

bool Collision::checkVehicleHit(sf::FloatRect playerBounds,
                                  const std::vector<Obstacle*>& obstacles) {
    for (auto* obs : obstacles) {
        if (!dynamic_cast<Vehicle*>(obs)) continue;
        if (playerBounds.findIntersection(obs->getBounds()))
            return true;
    }
    return false;
}

Log* Collision::getLogUnderPlayer(sf::FloatRect playerBounds,
                                    const std::vector<Obstacle*>& obstacles) {
    for (auto* obs : obstacles) {
        Log* log = dynamic_cast<Log*>(obs);
        if (!log) continue;
        if (playerBounds.findIntersection(obs->getBounds()))
            return log;
    }
    return nullptr;
}

bool Collision::checkDrowning(sf::FloatRect playerBounds,
                                const std::vector<Lane>& lanes,
                                const std::vector<Obstacle*>& obstacles) {
    bool onRiver = false;
    for (auto& lane : lanes) {
        if (lane.getLaneType() != Lane::RIVER) continue;
        if (playerBounds.findIntersection(lane.getBounds())) {
            onRiver = true; break;
        }
    }
    if (!onRiver) return false;
    return getLogUnderPlayer(playerBounds, obstacles) == nullptr;
}
