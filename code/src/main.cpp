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
    player.setPosition(sf::Vector2f((width / 2) - (PLAYER_SIZE / 2), (height - TILE)-250.f));

    Clock clock;

    View camera = window.getDefaultView();
    float cameraSpeed = 15.f;
    float cameraY = height / 2.f;

    for (int i = 0; i < 20; i++)
        worldGen.update(height / 2.f - i * TILE, lanes, obstacles);

    Font font;
    if (!font.openFromFile("C:\\Users\\Hp EliteBook 840 G5\\Documents\\OOP\\CCP\\code\\assets\\fonts\\pixelpurl.ttf"))
        return -1;  // make sure font file exists
        
    Text scoreText(font);
    scoreText.setCharacterSize(50);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(sf::Vector2f(10.f, 10.f));
        
    int score = 0;
    float highestY = player.getPosition().y;  // track how far up player has gone

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        // move camera up automatically
        cameraY -= cameraSpeed * dt;

        // snap to player if player is ahead of camera
        float playerY = player.getPosition().y;
        if (playerY < cameraY - height / 4.f)
            cameraY = playerY + height / 4.f;

        camera.setCenter(sf::Vector2f(width / 2.f, cameraY));
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
        // score increases as player moves up
    float currentY = player.getPosition().y;
    if (currentY < highestY) {
        score ++;
        highestY = currentY;
    }

    scoreText.setString(to_string(score));

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
    window.draw(scoreText);
    window.display();

    }
    for (auto* obs : obstacles)
        delete obs;

    return 0;
}
