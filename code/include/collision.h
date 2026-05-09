#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "obstacle.h"
#include "movingobstacle.h"
#include "lane.h"

class Collision {
public:
    static bool checkStaticBlocking(sf::FloatRect playerBounds,
                                    sf::Vector2f attemptedMove,
                                    const std::vector<Obstacle*>& obstacles);

    static bool checkVehicleHit(sf::FloatRect playerBounds,
                                 const std::vector<Obstacle*>& obstacles);

    static Log* getLogUnderPlayer(sf::FloatRect playerBounds,
                                   const std::vector<Obstacle*>& obstacles);

    static bool checkDrowning(sf::FloatRect playerBounds,
                               const std::vector<Lane>& lanes,
                               const std::vector<Obstacle*>& obstacles);
};
