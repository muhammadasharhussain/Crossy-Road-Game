#include <algorithm>  
#include <random>
#include "worldgenerator.h"

using namespace std;

// --- CONSTRUCTOR & DESTRUCTOR ---

WorldGenerator::WorldGenerator(float windowWidth, float windowHeight)
    : windowWidth(windowWidth), windowHeight(windowHeight),
      highestLaneY(windowHeight), lanesSinceLastSafe(0)
{
    rng.seed(random_device{}());
    registerPrototypes();
}

WorldGenerator::~WorldGenerator() {
    for (auto& pair : prototypes)
        delete pair.second;
}

// --- PROTOTYPE MANAGEMENT ---

void WorldGenerator::registerPrototypes() {
    for (auto& pair : prototypes)
        delete pair.second;
    prototypes.clear();

    prototypes["bike"]     = new Vehicle(0, 0, 100.f, 1, VehicleType::BIKE,     windowWidth);
    prototypes["rickshaw"] = new Vehicle(0, 0, 80.f,  1, VehicleType::RICKSHAW, windowWidth);
    prototypes["dumper"]   = new Vehicle(0, 0, 60.f,  1, VehicleType::DUMPER,   windowWidth);
    prototypes["tree"]     = new Tree(0, 0);
    prototypes["rock"]     = new Rock(0, 0);
    prototypes["log"]      = new Log(0, 0, 70.f, 1, windowWidth);
}

Obstacle* WorldGenerator::spawnFromPrototype(const string& key, float x, float y) {
    Obstacle* clone = prototypes[key]->clone();
    clone->setPosition(x, y); 
    return clone;
}

// --- CORE LOGIC ---

void WorldGenerator::update(float cameraY, vector<Lane>& lanes, vector<Obstacle*>& obstacles) {
    float spawnThreshold = cameraY - windowHeight / 2.f - TILE * 2;

    while (highestLaneY > spawnThreshold)
        spawnLane(lanes, obstacles);

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

// --- SPAWNING HELPERS ---

void WorldGenerator::spawnStaticObstacles(vector<Obstacle*>& obstacles, float laneY) {
    uniform_int_distribution<int> countDist(1, 2);
    uniform_real_distribution<float> xDist(0.f, windowWidth - TILE);
    uniform_int_distribution<int> typeDist(0, 1);

    int count = countDist(rng);
    for (int i = 0; i < count; i++) {
        float x = round(xDist(rng) / TILE) * TILE;
        string key = typeDist(rng) == 0 ? "tree" : "rock";
        obstacles.push_back(spawnFromPrototype(key, x, laneY));
    }
}

void WorldGenerator::spawnVehicles(vector<Obstacle*>& obstacles, float laneY) {
    uniform_int_distribution<int> dirDist(0, 1);
    uniform_int_distribution<int> countDist(1, 2);
    uniform_int_distribution<int> typeDist(0, 2);

    int direction = dirDist(rng) == 0 ? 1 : -1;
    int count = countDist(rng);

    string types[] = {"bike", "rickshaw", "dumper"};
    string key = types[typeDist(rng)]; 

    float spacing = windowWidth / count;
    if (spacing < TILE * 3.5f) { count = 1; spacing = windowWidth; }

    for (int i = 0; i < count; i++) {
        float x = direction == 1
            ? -TILE * 2 - i * spacing
            : windowWidth + i * spacing;

        Obstacle* v = spawnFromPrototype(key, x, laneY);
        static_cast<Vehicle*>(v)->setDirection(direction);
        obstacles.push_back(v);
    }
}

void WorldGenerator::spawnLogs(vector<Obstacle*>& obstacles, float laneY) {
    uniform_int_distribution<int> dirDist(0, 1);
    uniform_real_distribution<float> speedDist(50.f, 100.f);
    uniform_int_distribution<int> countDist(1, 2);

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
        static_cast<Log*>(log)->setSpeed(speed); 
        obstacles.push_back(log);
    }
}
