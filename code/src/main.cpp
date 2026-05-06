#include <SFML/Graphics.hpp>
#include <optional>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "../include/lane.h"
#include "../include/vehicle.h"
using namespace std;
using namespace sf;

int main() {
    const float TILE = 50.f;
    const float width = 550.f;
    const float height = 750.f;
    const float PLAYER_SIZE = 50.f;

    srand(static_cast<unsigned>(time(nullptr)));

    RenderWindow window(VideoMode({(unsigned int)width, (unsigned int)height}), "Crossy Road");

    vector<Lane> lanes;
    vector<Lane::Type> pattern = {
        Lane::SAFE,
        Lane::ROAD, Lane::ROAD,
        Lane::SAFE,
        Lane::RIVER, Lane::RIVER,
        Lane::SAFE,
        Lane::ROAD, Lane::ROAD,
        Lane::SAFE,
        Lane::ROAD,
        Lane::SAFE
    };
    for (int i = 0; i < (int)pattern.size(); i++)
        lanes.push_back(Lane(0.f, height - (i + 1) * TILE, width, TILE, pattern[i]));

    RectangleShape player({PLAYER_SIZE, PLAYER_SIZE});
    player.setFillColor(Color::Blue);
    player.setPosition({(width / 2) - (PLAYER_SIZE / 2), height - TILE});

    Clock clock;
    vector<Vehicle> vehicles;

    int roadIndex = 0;
    for (int i = 0; i < (int)lanes.size(); i++) {
        if (lanes[i].getLaneType() == Lane::ROAD) {
            float y = height - (i + 1) * TILE;
            float dir = (rand() % 2 == 0) ? 1.f : -1.f;
            float speed = (100.f + rand() % 50) * dir;

            // one size per lane, cycling SMALL -> MEDIUM -> LARGE
            Vehicle::Size size;
            if      (roadIndex % 3 == 0) size = Vehicle::SMALL;
            else if (roadIndex % 3 == 1) size = Vehicle::MEDIUM;
            else                         size = Vehicle::LARGE;

            // spacing based on size so they don't overlap
            float vehicleWidth = (size == Vehicle::SMALL)  ? TILE * 1.2f :
                                 (size == Vehicle::MEDIUM) ? TILE * 2.0f :
                                                             TILE * 3.0f;
            float gap = 100.f;
            float spacing = vehicleWidth + gap;

            int count = (int)(width / (spacing+50)); // fit as many as possible
            for (int j = 0; j < count; j++)
                vehicles.push_back(Vehicle(j * spacing, y, speed, TILE, size));

            roadIndex++;
        }
    }

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<Event::Closed>())
                window.close();

            if (const auto* keyEvent = event->getIf<Event::KeyPressed>()) {
                Vector2f pos = player.getPosition();
                switch (keyEvent->code) {
                    case Keyboard::Key::Up:
                        if (pos.y - TILE >= 0)
                            player.move({0.f, -TILE});
                        break;
                    case Keyboard::Key::Down:
                        if (pos.y + TILE + PLAYER_SIZE <= height)
                            player.move({0.f, TILE});
                        break;
                    case Keyboard::Key::Left:
                        if (pos.x - TILE >= 0)
                            player.move({-TILE, 0.f});
                        break;
                    case Keyboard::Key::Right:
                        if (pos.x + TILE + PLAYER_SIZE <= width)
                            player.move({TILE, 0.f});
                        break;
                    default: break;
                }
            }
        }

        float dt = clock.restart().asSeconds();
        for (auto& v : vehicles)
            v.update(dt, width);

        window.clear(Color(100, 200, 100));
        for (auto& lane : lanes)
            lane.draw(window);
        for (auto& v : vehicles)
            v.draw(window);
        window.draw(player);

        window.display();
    }
    return 0;
}
