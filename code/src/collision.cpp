#include "collision.h"

bool Collision::checkStaticBlocking(sf::RectangleShape& player,
                                     sf::Vector2f attemptedMove,
                                     const std::vector<Obstacle*>& obstacles) {
    // simulate where player would be after move
    sf::FloatRect future = player.getGlobalBounds();
    future.position.x += attemptedMove.x;
    future.position.y += attemptedMove.y;

    for (auto* obs : obstacles) {
        // only block on Tree and Rock, not Vehicle or Log
        if (dynamic_cast<Vehicle*>(obs) || dynamic_cast<Log*>(obs))
            continue;
        if (future.findIntersection(obs->getBounds()))
            return true;  // blocked
    }
    return false;
}

bool Collision::checkVehicleHit(const sf::RectangleShape& player,
                                  const std::vector<Obstacle*>& obstacles) {
    sf::FloatRect pb = player.getGlobalBounds();
    for (auto* obs : obstacles) {
        if (!dynamic_cast<Vehicle*>(obs)) continue;
        if (pb.findIntersection(obs->getBounds()))
            return true;
    }
    return false;
}

Log* Collision::getLogUnderPlayer(const sf::RectangleShape& player,
                                    const std::vector<Obstacle*>& obstacles) {
    sf::FloatRect pb = player.getGlobalBounds();
    for (auto* obs : obstacles) {
        Log* log = dynamic_cast<Log*>(obs);
        if (!log) continue;
        if (pb.findIntersection(obs->getBounds()))
            return log;
    }
    return nullptr;
}

bool Collision::checkDrowning(const sf::RectangleShape& player,
                                const std::vector<Lane>& lanes,
                                const std::vector<Obstacle*>& obstacles) {
    sf::FloatRect pb = player.getGlobalBounds();
    float playerCenterY = pb.position.y + pb.size.y / 2.f;

    // check if player is on a river lane
    bool onRiver = false;
    for (auto& lane : lanes) {
        if (lane.getLaneType() != Lane::RIVER) continue;
        sf::FloatRect lb = lane.getBounds();  // add getBounds() to Lane
        if (pb.findIntersection(lb)) { onRiver = true; break; }
    }
    if (!onRiver) return false;

    // on river — safe only if on a log
    return getLogUnderPlayer(player, obstacles) == nullptr;
}
