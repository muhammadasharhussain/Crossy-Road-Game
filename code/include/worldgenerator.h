#pragma once
#include <vector>
#include <random>
#include "lane.h"
#include "obstacle.h"
#include "movingobstacle.h"  // your vehicle header
#include "staticobstacle.h"
#include "constants.h"

class WorldGenerator {
public:
    WorldGenerator(float windowWidth, float windowHeight);

    void update(float cameraY, std::vector<Lane>& lanes, std::vector<Obstacle*>& obstacles);

private:
    float windowWidth;
    float windowHeight;
    float highestLaneY;   // topmost lane spawned so far
    int lanesSinceLastSafe;

    std::mt19937 rng;

    void spawnLane(std::vector<Lane>& lanes, std::vector<Obstacle*>& obstacles);
    Lane::Type pickLaneType();
    void spawnStaticObstacles(std::vector<Obstacle*>& obstacles, float laneY);
    void spawnVehicles(std::vector<Obstacle*>& obstacles, float laneY);
};