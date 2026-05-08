#include <algorithm>  
#include "worldgenerator.h"
#include <random>
using namespace std;

WorldGenerator::WorldGenerator(float windowWidth, float windowHeight)
    : windowWidth(windowWidth), windowHeight(windowHeight),
      highestLaneY(windowHeight), lanesSinceLastSafe(0)
{
    rng.seed(random_device{}());
    registerPrototypes();   // ← register on startup
}

WorldGenerator::~WorldGenerator() {
    for (auto& pair : prototypes)
        delete pair.second;
}

void WorldGenerator::registerPrototypes() {
    // register one master copy of each type at dummy position (0,0)
    // position gets overwritten by spawnFromPrototype()
    prototypes["bike"]     = new Vehicle(0, 0, 100.f, 1, VehicleType::BIKE,     windowWidth);
    prototypes["rickshaw"] = new Vehicle(0, 0, 80.f,  1, VehicleType::RICKSHAW, windowWidth);
    prototypes["dumper"]   = new Vehicle(0, 0, 60.f,  1, VehicleType::DUMPER,   windowWidth);
    prototypes["tree"]     = new Tree(0, 0);
    prototypes["rock"]     = new Rock(0, 0);
    prototypes["log"]      = new Log(0, 0, 70.f, 1, windowWidth);
}

Obstacle* WorldGenerator::spawnFromPrototype(const string& key, float x, float y) {
    Obstacle* clone = prototypes[key]->clone();  // copy the prototype
    clone->setPosition(x, y);                    // place it in the world
    return clone;
}

void WorldGenerator::update(float cameraY, vector<Lane>& lanes, vector<Obstacle*>& obstacles) {
    // spawn new lanes when camera gets close to the top
    float spawnThreshold = cameraY - windowHeight / 2.f - TILE * 2;

    while (highestLaneY > spawnThreshold)
        spawnLane(lanes, obstacles);

    // remove lanes and obstacles that are far below camera
    float despawnY = cameraY + windowHeight;

    lanes.erase(remove_if(lanes.begin(), lanes.end(),
        [despawnY](const Lane& l) { return l.getY() > despawnY; }
    ), lanes.end());

    obstacles.erase(remove_if(obstacles.begin(), obstacles.end(),
        [despawnY](Obstacle* o) {
            bool gone = o->getBounds().position.y > despawnY;
            if (gone) delete o;
            return gone;
        }
    ), obstacles.end());
}

void WorldGenerator::spawnLane(vector<Lane>& lanes, vector<Obstacle*>& obstacles) {
    highestLaneY -= TILE;

    Lane::Type type = pickLaneType();
    lanes.push_back(Lane(0.f, highestLaneY, windowWidth, TILE, type));

    if (type == Lane::SAFE)
        spawnStaticObstacles(obstacles, highestLaneY);
    else if (type == Lane::ROAD)
        spawnVehicles(obstacles, highestLaneY);
    else if (type == Lane::RIVER)
        spawnLogs(obstacles, highestLaneY);

    lanesSinceLastSafe++;
    if (type == Lane::SAFE)
        lanesSinceLastSafe = 0;
}

Lane::Type WorldGenerator::pickLaneType() {
    // force a safe lane every 4 rows max
    if (lanesSinceLastSafe >= 4)
        return Lane::SAFE;

    uniform_int_distribution<int> dist(0, 2);
    switch (dist(rng)) {
        case 0: return Lane::SAFE;
        case 1: return Lane::ROAD;
        case 2: return Lane::RIVER;
        default: return Lane::SAFE;
    }
}

void WorldGenerator::spawnStaticObstacles(vector<Obstacle*>& obstacles, float laneY) {
    uniform_int_distribution<int> countDist(1, 2);
    uniform_real_distribution<float> xDist(0.f, windowWidth - TILE);
    uniform_int_distribution<int> typeDist(0, 1);

    int count = countDist(rng);
    for (int i = 0; i < count; i++) {
        float x = round(xDist(rng) / TILE) * TILE;
        string key = typeDist(rng) == 0 ? "tree" : "rock";
        obstacles.push_back(spawnFromPrototype(key, x, laneY));  // ← no more new Tree/Rock
    }
}

void WorldGenerator::spawnVehicles(vector<Obstacle*>& obstacles, float laneY) {
    uniform_int_distribution<int> dirDist(0, 1);
    uniform_int_distribution<int> countDist(1, 2);
    uniform_int_distribution<int> typeDist(0, 2);

    // all vehicles on one lane share direction
    int direction = dirDist(rng) == 0 ? 1 : -1;
    int count = countDist(rng);

    string types[] = {"bike", "rickshaw", "dumper"};
    string key = types[typeDist(rng)];  // one vehicle type per lane

    float spacing = windowWidth / count;
    if (spacing < TILE * 3.5f) { count = 1; spacing = windowWidth; }

    for (int i = 0; i < count; i++) {
        float x = direction == 1
            ? -TILE * 2 - i * spacing
            : windowWidth + i * spacing;

        Obstacle* v = spawnFromPrototype(key, x, laneY);
        // fix direction since prototype always stores direction=1
        static_cast<Vehicle*>(v)->setDirection(direction);
        obstacles.push_back(v);
    }
}

void WorldGenerator::spawnLogs(std::vector<Obstacle*>& obstacles, float laneY) {
    std::uniform_int_distribution<int> dirDist(0, 1);
    std::uniform_real_distribution<float> speedDist(50.f, 100.f);
    std::uniform_int_distribution<int> countDist(1, 2);

    int direction = dirDist(rng) == 0 ? 1 : -1;
    float speed = speedDist(rng);
    int count = countDist(rng);

    float spacing = windowWidth / count;
    if (spacing < TILE * 4.f) { count = 1; spacing = windowWidth; }

    for (int i = 0; i < count; i++) {
        float x = direction == 1
            ? -TILE * 3 - i * spacing
            : windowWidth + i * spacing;

        Obstacle* log = spawnFromPrototype("log", x, laneY);
        static_cast<Log*>(log)->setDirection(direction);
        static_cast<Log*>(log)->setSpeed(speed);   // add setSpeed to obstacle.h
        obstacles.push_back(log);
    }
}
