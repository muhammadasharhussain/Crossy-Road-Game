#include <SFML/Graphics.hpp>
#include <optional>
#include <vector>
#include "lane.h"
#include "obstacle.h"
#include "staticobstacle.h"
#include "movingobstacle.h"
#include "collision.h"
#include "constants.h"
#include "worldgenerator.h"

using namespace std;
using namespace sf;

int main() {

    RenderWindow window(VideoMode({(unsigned int)width, (unsigned int)height}), "Crossy Road");

    vector<Lane> lanes;
    vector<Obstacle*> obstacles;
    WorldGenerator worldGen(width, height);

    // player
    RectangleShape player({PLAYER_SIZE, PLAYER_SIZE});
    player.setFillColor(Color::Blue);
    player.setPosition(sf::Vector2f({(width / 2) - (PLAYER_SIZE / 2), (height - TILE)-250.f}));

    Clock clock;

    View camera = window.getDefaultView();
    float cameraSpeed = 15.f;
    float cameraY = height / 2.f;

    for (int i = 0; i < 20; i++)
        worldGen.update(height / 2.f - i * TILE, lanes, obstacles);

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        // move camera up automatically
        cameraY -= cameraSpeed * dt;

        // snap to player if player is ahead of camera
        float playerY = player.getPosition().y;
        if (playerY < cameraY - height / 4.f)
            cameraY = playerY + height / 4.f;

        camera.setCenter({width / 2.f, cameraY});
        window.setView(camera);

        worldGen.update(cameraY, lanes, obstacles);

        while (const std::optional event = window.pollEvent()) {
            if (event->is<Event::Closed>())
                window.close();

                if (const auto* keyEvent = event->getIf<Event::KeyPressed>()) {
                    Vector2f pos = player.getPosition();
                    float topEdge    = cameraY - height / 2.f;
                    float bottomEdge = cameraY + height / 2.f;
            
                    switch (keyEvent->code) {
                        case Keyboard::Key::Up:
                            if (pos.y - TILE >= topEdge)
                                player.move({0.f, -TILE}); break;
                        case Keyboard::Key::Down:
                            if (pos.y + PLAYER_SIZE <= bottomEdge)
                                player.move({0.f, TILE}); break;
                        case Keyboard::Key::Left:
                            if (pos.x - TILE >= 0)
                                player.move({-TILE, 0.f}); break;
                        case Keyboard::Key::Right:
                            if (pos.x + TILE + PLAYER_SIZE <= width)
                                player.move({TILE, 0.f}); break;
                        default: break;
                    }
                }
        }

    for (auto* obs : obstacles)
        obs->update(dt);

    if (Collision::checkPlayerObstacle(player, obstacles))
        player.setFillColor(Color::Red);
    else
        player.setFillColor(Color::Blue);

    window.clear(Color::Green);

    for (auto& lane : lanes)
        lane.draw(window);

    for (auto* obs : obstacles)
        obs->draw(window);

    window.draw(player);

    window.setView(window.getDefaultView()); // reset for HUD later
    window.display();
    }
    for (auto* obs : obstacles)
        delete obs;

    return 0;
}
