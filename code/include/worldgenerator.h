#pragma once
#include <vector>
#include <map>
#include <string>
#include <random>
#include "lane.h"
#include "obstacle.h"
#include "movingobstacle.h"
#include "staticobstacle.h"
#include "constants.h"

class WorldGenerator {
public:
    WorldGenerator(float windowWidth, float windowHeight);
    ~WorldGenerator();  // ← needed to delete prototypes

    void update(float cameraY, std::vector<Lane>& lanes, std::vector<Obstacle*>& obstacles);

private:
    float windowWidth;
    float windowHeight;
    float highestLaneY;
    int lanesSinceLastSafe;

    std::mt19937 rng;
    std::map<std::string, Obstacle*> prototypes;  // ← prototype registry

    void registerPrototypes();
    Obstacle* spawnFromPrototype(const std::string& key, float x, float y);

    void spawnLane(std::vector<Lane>& lanes, std::vector<Obstacle*>& obstacles);
    Lane::Type pickLaneType();
    void spawnStaticObstacles(std::vector<Obstacle*>& obstacles, float laneY);
    void spawnVehicles(std::vector<Obstacle*>& obstacles, float laneY);
    void spawnLogs(std::vector<Obstacle*>& obstacles, float laneY);
};
