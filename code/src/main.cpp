#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <optional>
#include <vector>
#include <string>
// Custom Headers
#include "lane.h"
#include "obstacle.h"
#include "staticobstacle.h"
#include "movingobstacle.h"
#include "collision.h"
#include "constants.h"
#include "worldgenerator.h"
#include "audio.h" 

using namespace std;
using namespace sf;

int main() {
    // ---------------------------------------------------------
    // 1. INITIALIZATION & WINDOW SETUP
    // ---------------------------------------------------------
    RenderWindow window(VideoMode({(unsigned int)width, (unsigned int)height}), "Crossy Road");
    
    vector<Lane> lanes;
    vector<Obstacle*> obstacles;
    WorldGenerator worldGen(width, height);
    AudioManager audio; 

    // Player Setup
    RectangleShape player({PLAYER_SIZE, PLAYER_SIZE});
    player.setFillColor(Color::Blue);
    player.setPosition(Vector2f((width / 2) - (PLAYER_SIZE / 2), (height - TILE) - 250.f));

    Clock clock;
    View camera = window.getDefaultView();
    float cameraSpeed = 15.f;
    float cameraY = height / 2.f;

    // Initial World Generation
    for (int i = 0; i < 20; i++)
        worldGen.update(height / 2.f - i * TILE, lanes, obstacles);

    // ---------------------------------------------------------
    // 2. UI & TEXT SETUP
    // ---------------------------------------------------------
    Font font;
    if (!font.openFromFile("..\\assets\\fonts\\pixelpurl.ttf"))
        return -1; 
        
    Text scoreText(font);
    scoreText.setCharacterSize(50);
    scoreText.setFillColor(Color::White);
    scoreText.setPosition(Vector2f(10.f, 10.f));
        
    int score = 0;
    float highestY = player.getPosition().y;

    // Game States
    enum class GameState { PLAYING, DEAD };
    GameState state = GameState::PLAYING;

    // Death Screen UI
    Text deathText(font);
    deathText.setCharacterSize(48);
    deathText.setFillColor(Color::Red);
    deathText.setString("GAME OVER");
    
    Text restartText(font);
    restartText.setCharacterSize(24);
    restartText.setFillColor(Color::White);
    restartText.setString("Press R to restart");

    // Start Background Music
    audio.playMusic("..\\assets\\sounds\\bg_music.ogg");

    // ---------------------------------------------------------
    // 3. MAIN GAME LOOP
    // ---------------------------------------------------------
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        // --- CAMERA LOGIC ---
        if (state == GameState::PLAYING) {
            cameraY -= cameraSpeed * dt;
            float playerY = player.getPosition().y;
            // Snap camera to player if they advance too quickly
            if (playerY < cameraY - height / 4.f)
                cameraY = playerY + height / 4.f;
        }

        camera.setCenter(Vector2f(width / 2.f, cameraY));
        window.setView(camera);
        worldGen.update(cameraY, lanes, obstacles);

        // --- EVENT PROCESSING ---
        while (const std::optional event = window.pollEvent()) {
            if (event->is<Event::Closed>())
                window.close();

            if (const auto* keyEvent = event->getIf<Event::KeyPressed>()) {
                
                // Player Movement
                if (state == GameState::PLAYING) {
                    Vector2f pos = player.getPosition();
                    float topEdge = cameraY - height / 2.f;
                    bool moved = false;
                    Vector2f moveVec(0.f, 0.f);

                    switch (keyEvent->code) {
                        case Keyboard::Key::Up:
                            if (pos.y - TILE >= topEdge) {
                                moveVec = {0.f, -TILE};
                                if (!Collision::checkStaticBlocking(player, moveVec, obstacles)) {
                                    player.move(moveVec);
                                    moved = true;
                                }
                            }
                            break;
                        case Keyboard::Key::Down:
                            // Lower boundary removed: player can move off-camera to die
                            moveVec = {0.f, TILE};
                            if (!Collision::checkStaticBlocking(player, moveVec, obstacles)) {
                                player.move(moveVec);
                                moved = true;
                            }
                            break;
                        case Keyboard::Key::Left:
                            if (pos.x - TILE >= 0) {
                                moveVec = {-TILE, 0.f};
                                if (!Collision::checkStaticBlocking(player, moveVec, obstacles)) {
                                    player.move(moveVec);
                                    moved = true;
                                }
                            }
                            break;
                        case Keyboard::Key::Right:
                            if (pos.x + TILE + PLAYER_SIZE <= width) {
                                moveVec = {TILE, 0.f};
                                if (!Collision::checkStaticBlocking(player, moveVec, obstacles)) {
                                    player.move(moveVec);
                                    moved = true;
                                }
                            }
                            break;
                        default: break;
                    }

                    if (moved) audio.playSound("hop");
                }

                // Restart Logic
                if (state == GameState::DEAD && keyEvent->code == Keyboard::Key::R) {
                    state = GameState::PLAYING;
                    score = 0;
                    lanes.clear();
                    for (auto* o : obstacles) delete o;
                    obstacles.clear();
                    player.setPosition(Vector2f((width / 2) - (PLAYER_SIZE / 2), (height - TILE) - 250.f));
                    cameraY = height / 2.f;
                    highestY = player.getPosition().y;
                    worldGen = WorldGenerator(width, height);
                    for (int i = 0; i < 20; i++)
                        worldGen.update(height / 2.f - i * TILE, lanes, obstacles);
                }
            }
        }

        // --- GAME LOGIC & UPDATES ---
        if (state == GameState::PLAYING) {
            // Scoring
            float currentY = player.getPosition().y;
            if (currentY < highestY) {
                score++;
                highestY = currentY;
            }

            // Update Obstacles
            for (auto* obs : obstacles) obs->update(dt);

            // Log Riding
            Log* log = Collision::getLogUnderPlayer(player, obstacles);
            if (log != nullptr) player.move(Vector2f(log->getSpeedX() * dt, 0.f));

            // Death Checks (Vehicles, Water, or Falling off bottom of Camera)
            float bottomEdge = cameraY + height / 2.f;
            if (player.getPosition().y > bottomEdge || 
                Collision::checkVehicleHit(player, obstacles) || 
                Collision::checkDrowning(player, lanes, obstacles)) {
                
                state = GameState::DEAD;
                audio.playSound("death");
            }
        }

        // --- RENDERING ---
        window.clear(Color::Green);

        // Draw World Objects
        for (auto& lane : lanes) lane.draw(window);
        for (auto* obs : obstacles) obs->draw(window);
        window.draw(player);

        // Draw UI (Switch to Static/Default View so UI doesn't move with camera)
        window.setView(window.getDefaultView());
        scoreText.setString(to_string(score));
        window.draw(scoreText);

        if (state == GameState::DEAD) {
            RectangleShape overlay(Vector2f((float)width, (float)height));
            overlay.setFillColor(Color(0, 0, 0, 160));
            window.draw(overlay);

            // Center death UI
            deathText.setPosition(Vector2f(width/2.f - deathText.getGlobalBounds().size.x/2.f, height/2.f - 60.f));
            restartText.setPosition(Vector2f(width/2.f - restartText.getGlobalBounds().size.x/2.f, height/2.f + 10.f));
            
            window.draw(deathText);
            window.draw(restartText);
        }

        window.display();
    }

    // Cleanup
    for (auto* obs : obstacles) delete obs;
    return 0;
}
