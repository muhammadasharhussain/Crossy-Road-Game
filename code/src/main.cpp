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

    // before game loop — add game state
    enum class GameState { PLAYING, DEAD };
    GameState state = GameState::PLAYING;

    // death screen text
    Text deathText(font);
    deathText.setCharacterSize(48);
    deathText.setFillColor(sf::Color::Red);
    deathText.setString("GAME OVER");
    deathText.setPosition(sf::Vector2f(
        width / 2.f - deathText.getGlobalBounds().size.x / 2.f, 
        height / 2.f - 60.f));

    Text restartText(font);
    restartText.setCharacterSize(24);
    restartText.setFillColor(sf::Color::White);
    restartText.setString("Press R to restart");
    restartText.setPosition(sf::Vector2f(
        width / 2.f - restartText.getGlobalBounds().size.x / 2.f,
        height / 2.f + 10.f));

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
                    if (state == GameState::PLAYING){
                    Vector2f pos = player.getPosition();
                    float topEdge    = cameraY - height / 2.f;
                    float bottomEdge = cameraY + height / 2.f;
            
                    switch (keyEvent->code) {
                        case Keyboard::Key::Up:
                            if (pos.y - TILE >= topEdge){
                                sf::Vector2f move(0.f, -TILE);  // example for Up
                                if (!Collision::checkStaticBlocking(player, move, obstacles))
                                    player.move(move); break;
                            }
                        case Keyboard::Key::Down:
                            if (pos.y + PLAYER_SIZE <= bottomEdge){
                                sf::Vector2f move(0.f, TILE);  // example for Down
                                if (!Collision::checkStaticBlocking(player, move, obstacles))
                                    player.move(move); break;
                            }
                        case Keyboard::Key::Left:
                            if (pos.x - TILE >= 0){
                                sf::Vector2f move(-TILE, 0.f);  // example for Left
                                if (!Collision::checkStaticBlocking(player, move, obstacles))
                                    player.move(move); break;
                            }
                        case Keyboard::Key::Right:
                            if (pos.x + TILE + PLAYER_SIZE <= width){
                                sf::Vector2f move(TILE, 0.f);  // example for Right
                                if (!Collision::checkStaticBlocking(player, move, obstacles))
                                    player.move(move); break;
                            }
                        default: break;
                    }
                }

                if (state == GameState::DEAD) {
                    if (keyEvent->code == Keyboard::Key::R) {
                        // reset everything
                        state = GameState::PLAYING;
                        score = 0;
                        lanes.clear();
                        for (auto* o : obstacles) delete o;
                        obstacles.clear();
                        player.setPosition(sf::Vector2f(
                            (width / 2) - (PLAYER_SIZE / 2), height - TILE - 250.f));
                        cameraY = height / 2.f;
                        highestY = player.getPosition().y;
                        worldGen = WorldGenerator(width, height);
                        for (int i = 0; i < 20; i++)
                            worldGen.update(height / 2.f - i * TILE, lanes, obstacles);
                    }
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

    if (state == GameState::PLAYING) {
        // update obstacles
        for (auto* obs : obstacles)
            obs->update(dt);
    
        // log riding — move player with log
        Log* log = Collision::getLogUnderPlayer(player, obstacles);
        if (log != nullptr)
            player.move(sf::Vector2f(log->getSpeedX() * dt, 0.f));
    
        // death checks
        if (Collision::checkVehicleHit(player, obstacles) ||
            Collision::checkDrowning(player, lanes, obstacles)) {
            state = GameState::DEAD;
        }
    
        player.setFillColor(sf::Color::Blue);
    }

    window.clear(Color::Green);

    for (auto& lane : lanes)
        lane.draw(window);

    for (auto* obs : obstacles)
        obs->draw(window);

    window.draw(player);

    // drawing
window.setView(window.getDefaultView());
window.draw(scoreText);

if (state == GameState::DEAD) {
    // dark overlay
    sf::RectangleShape overlay(sf::Vector2f(width, height));
    overlay.setFillColor(sf::Color(0, 0, 0, 160));
    window.draw(overlay);
    window.draw(deathText);
    window.draw(restartText);
}

window.display();

    }
    for (auto* obs : obstacles)
        delete obs;

    return 0;
}
