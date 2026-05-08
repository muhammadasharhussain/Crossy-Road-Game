#include <algorithm>  
#include "worldgenerator.h"
#include <random>

WorldGenerator::WorldGenerator(float windowWidth, float windowHeight)
    : windowWidth(windowWidth), windowHeight(windowHeight),
      highestLaneY(windowHeight), lanesSinceLastSafe(0)
{
    rng.seed(std::random_device{}());
}

void WorldGenerator::update(float cameraY, std::vector<Lane>& lanes, std::vector<Obstacle*>& obstacles) {
    // spawn new lanes when camera gets close to the top
    float spawnThreshold = cameraY - windowHeight / 2.f - TILE * 2;

    while (highestLaneY > spawnThreshold)
        spawnLane(lanes, obstacles);

    // remove lanes and obstacles that are far below camera
    float despawnY = cameraY + windowHeight;

    lanes.erase(std::remove_if(lanes.begin(), lanes.end(),
        [despawnY](const Lane& l) { return l.getY() > despawnY; }
    ), lanes.end());

    obstacles.erase(std::remove_if(obstacles.begin(), obstacles.end(),
        [despawnY](Obstacle* o) {
            bool gone = o->getBounds().position.y > despawnY;
            if (gone) delete o;
            return gone;
        }
    ), obstacles.end());
}

void WorldGenerator::spawnLane(std::vector<Lane>& lanes, std::vector<Obstacle*>& obstacles) {
    highestLaneY -= TILE;

    Lane::Type type = pickLaneType();
    lanes.push_back(Lane(0.f, highestLaneY, windowWidth, TILE, type));

    if (type == Lane::SAFE)
        spawnStaticObstacles(obstacles, highestLaneY);
    else if (type == Lane::ROAD)
        spawnVehicles(obstacles, highestLaneY);

    lanesSinceLastSafe++;
    if (type == Lane::SAFE)
        lanesSinceLastSafe = 0;
}

Lane::Type WorldGenerator::pickLaneType() {
    // force a safe lane every 4 rows max
    if (lanesSinceLastSafe >= 4)
        return Lane::SAFE;

    std::uniform_int_distribution<int> dist(0, 2);
    switch (dist(rng)) {
        case 0: return Lane::SAFE;
        case 1: return Lane::ROAD;
        case 2: return Lane::RIVER;
        default: return Lane::SAFE;
    }
}

void WorldGenerator::spawnStaticObstacles(std::vector<Obstacle*>& obstacles, float laneY) {
    std::uniform_int_distribution<int> countDist(1, 2);
    std::uniform_real_distribution<float> xDist(0.f, windowWidth - TILE);
    std::uniform_int_distribution<int> typeDist(0, 1);

    int count = countDist(rng);
    for (int i = 0; i < count; i++) {
        float x = std::round(xDist(rng) / TILE) * TILE; // snap to grid
        if (typeDist(rng) == 0)
            obstacles.push_back(new Tree(x, laneY));
        else
            obstacles.push_back(new Rock(x, laneY));
    }
}

void WorldGenerator::spawnVehicles(std::vector<Obstacle*>& obstacles, float laneY) {
    std::uniform_int_distribution<int> dirDist(0, 1);
    std::uniform_real_distribution<float> speedDist(80.f, 140.f);  // slower max
    std::uniform_int_distribution<int> countDist(1, 2);            // was 1-3, now 1-2

    int direction = dirDist(rng) == 0 ? 1 : -1;
    float speed = speedDist(rng);
    int count = countDist(rng);

    float spacing = windowWidth / count;
    // enforce minimum gap so player can always find a gap
    float minSpacing = TILE * 3.5f;
    if (spacing < minSpacing) {
        count = 1;
        spacing = windowWidth;
    }

    for (int i = 0; i < count; i++) {
        float x = direction == 1
            ? -TILE * 2 - i * spacing
            : windowWidth + i * spacing;
        obstacles.push_back(new Vehicle(x, laneY, speed, direction, TILE));
    }
}