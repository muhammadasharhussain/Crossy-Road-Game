#include <SFML/Graphics.hpp>
#include <optional>
#include <vector>
#include "lane.h"
using namespace std;
using namespace sf;
    
int main() {
    const float TILE = 50.f;
    const float width = 550.f;
    const float height = 750.f;
    const float PLAYER_SIZE = 50.f;

    RenderWindow window(VideoMode({(unsigned int)width,(unsigned int)height}), "Crossy Road");


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
        window.clear(Color(100, 200, 100));
        for (auto& lane : lanes)
            lane.draw(window);

        window.draw(player);
        window.display();
    }
    return 0;
}