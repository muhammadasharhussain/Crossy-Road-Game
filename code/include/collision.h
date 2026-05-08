#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "obstacle.h"
#include "movingobstacle.h"
#include "lane.h"

class Collision {
public:
    // returns true if blocked (static obstacle in the way)
    static bool checkStaticBlocking(sf::RectangleShape& player,
                                    sf::Vector2f attemptedMove,
                                    const std::vector<Obstacle*>& obstacles);

    // returns true if player touched a vehicle → death
    static bool checkVehicleHit(const sf::RectangleShape& player,
                                 const std::vector<Obstacle*>& obstacles);

    // returns log player is standing on, nullptr if none
    static Log* getLogUnderPlayer(const sf::RectangleShape& player,
                                   const std::vector<Obstacle*>& obstacles);

    // returns true if player is in river water (on RIVER lane but not on a log)
    static bool checkDrowning(const sf::RectangleShape& player,
                               const std::vector<Lane>& lanes,
                               const std::vector<Obstacle*>& obstacles);
};
