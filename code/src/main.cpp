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
#include "scoreboard.h"
#include "audio.h"

using namespace std;
using namespace sf;

int main() {
    // --- INITIALIZATION ---
    RenderWindow window(VideoMode({(unsigned int)width, (unsigned int)height}), "Crossy Road");

    // Game Objects & Managers
    vector<Lane> lanes;
    vector<Obstacle*> obstacles;
    WorldGenerator worldGen(width, height);
    AudioManager audio;

    // Player Setup
    Texture playerTexture;
    playerTexture.loadFromFile("..\\assets\\images\\player.png");
    Sprite player(playerTexture);
    Vector2u ps = playerTexture.getSize();
    player.setScale({PLAYER_SIZE / (float)ps.x, PLAYER_SIZE / (float)ps.y});
    player.setPosition(sf::Vector2f((width / 2) - (PLAYER_SIZE / 2), (height - TILE) - 250.f));

    // Camera & Timing
    Clock clock;
    View camera = window.getDefaultView();
    float cameraSpeed = 40.f;
    float cameraY = height / 2.f;

    // --- UI & ASSETS ---
    Font font;
    if (!font.openFromFile("..\\assets\\fonts\\pixelpurl.ttf"))
        return -1;

    // HUD Text (Score and High Score)
    Text scoreText(font);
    scoreText.setCharacterSize(50);
    scoreText.setFillColor(Color::White);
    scoreText.setPosition(sf::Vector2f(10.f, 10.f));

    Text highScoreText(font);
    highScoreText.setCharacterSize(32);
    highScoreText.setFillColor(Color::Yellow);
    highScoreText.setPosition(sf::Vector2f(10.f, 60.f));

    // Score Tracking
    int score = 0;
    float highestY = player.getPosition().y;
    const string SCORE_FILE = "highscore.txt";
    int highScore = loadHighScore(SCORE_FILE);

    // Game States
    enum class GameState { MENU, PLAYING, DEAD };
    GameState state = GameState::MENU;

    // Menu Background
    Texture bgTexture;
    if (!bgTexture.loadFromFile("..\\assets\\images\\bg.png"))
        return -1;

    Sprite bgSprite(bgTexture);
    bgSprite.setScale(sf::Vector2f(
        width  / (float)bgTexture.getSize().x,
        height / (float)bgTexture.getSize().y
    ));

    // UI Prompts
    Text promptText(font);
    promptText.setCharacterSize(32);
    promptText.setFillColor(sf::Color::White);
    promptText.setString("Press ENTER to Play");
    FloatRect tb = promptText.getGlobalBounds();
    promptText.setPosition(sf::Vector2f(
        width / 2.f - tb.size.x / 2.f,
        height - 100.f
    ));

    // Audio Start
    audio.playMusic("..\\assets\\sounds\\bg_music.ogg");

    // Pre-seed the world with 20 lanes
    for (int i = 0; i < 20; i++)
        worldGen.update(height / 2.f - i * TILE, lanes, obstacles);

    // --- MAIN GAME LOOP ---
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        // ─── EVENTS ───────────────────────────────────────
        while (const std::optional event = window.pollEvent()) {
            if (event->is<Event::Closed>()) {
                window.close();
            }

            if (const auto* keyEvent = event->getIf<Event::KeyPressed>()) {
                // Restart logic for Menu or Death screens
                if (state == GameState::MENU || state == GameState::DEAD) {
                    if (keyEvent->code == Keyboard::Key::Enter) {
                        state = GameState::PLAYING;
                        score = 0;
                        lanes.clear();
                        
                        // Clean up old obstacle pointers
                        for (auto* o : obstacles) delete o;
                        obstacles.clear();

                        // Reset Player and Camera
                        player.setPosition(sf::Vector2f(
                            (width / 2) - (PLAYER_SIZE / 2), height - TILE - 250.f));
                        cameraY = height / 2.f;
                        highestY = player.getPosition().y;
                        
                        // Re-generate initial world
                        worldGen = WorldGenerator(width, height);
                        for (int i = 0; i < 20; i++)
                            worldGen.update(height / 2.f - i * TILE, lanes, obstacles);
                    }
                }

                // Gameplay Input
                if (state == GameState::PLAYING) {
                    Vector2f pos = player.getPosition();
                    float topEdge = cameraY - height / 2.f;
                    bool moved = false;
                    Vector2f moveVec(0.f, 0.f);

                    switch (keyEvent->code) {
                        case Keyboard::Key::Up:
                            if (pos.y - TILE >= topEdge) {
                                moveVec = {0.f, -TILE};
                                if (!Collision::checkStaticBlocking(player.getGlobalBounds(), moveVec, obstacles)) {
                                    player.move(moveVec);
                                    moved = true;
                                }
                            }
                            break;
                        case Keyboard::Key::Down:
                            moveVec = {0.f, TILE};
                            if (!Collision::checkStaticBlocking(player.getGlobalBounds(), moveVec, obstacles)) {
                                player.move(moveVec);
                                moved = true;
                            }
                            break;
                        case Keyboard::Key::Left:
                            moveVec = {-TILE, 0.f};
                            if (!Collision::checkStaticBlocking(player.getGlobalBounds(), moveVec, obstacles)) {
                                player.move(moveVec);
                                moved = true;
                            }
                            else {
                                // try smaller step to fill the gap
                                float gap = pos.x - player.getGlobalBounds().position.x;
                                for (float step = TILE - 1; step >= 5.f; step -= 5.f) {
                                    sf::Vector2f small(-step, 0.f);
                                    if (!Collision::checkStaticBlocking(player.getGlobalBounds(), small, obstacles)) {
                                        player.move(small);
                                        moved = true;
                                        player.setPosition(player.getPosition());
                                        break;
                                    }
                                }
                            }
                            break;
                        case Keyboard::Key::Right:
                            moveVec = {TILE, 0.f};
                            if (!Collision::checkStaticBlocking(player.getGlobalBounds(), moveVec, obstacles)) {
                                player.move(moveVec);
                                moved = true;
                            }
                            else {
                                // try smaller step to fill the gap
                                for (float step = TILE - 1; step >= 5.f; step -= 5.f) {
                                    sf::Vector2f small(step, 0.f);
                                    if (!Collision::checkStaticBlocking(player.getGlobalBounds(), small, obstacles)) {
                                        player.move(small);
                                        moved = true;
                                        player.setPosition(player.getPosition());
                                        break;
                                    }
                                }
                            }
                            break;
                        default: break;
                    }
                    if (moved)
                        audio.playSound("hop");
                }
            }
        }

        // ─── UPDATE ───────────────────────────────────────
        if (state == GameState::PLAYING) {
            // Constant Camera Movement
            cameraY -= cameraSpeed * dt;
            
            // Camera follow: speed up if player is too far forward
            float playerY = player.getPosition().y;
            if (playerY < cameraY - height / 4.f)
                cameraY = playerY + height / 4.f;

            camera.setCenter(sf::Vector2f(width / 2.f, cameraY));
            window.setView(camera);

            // Generate new lanes based on camera progress
            worldGen.update(cameraY, lanes, obstacles);

            // Update all moving obstacles (Cars, Logs)
            for (auto* obs : obstacles)
                obs->update(dt);

            // Log Platform Logic: Move player along with the log
            Log* log = Collision::getLogUnderPlayer(player.getGlobalBounds(), obstacles);
            if (log != nullptr)
                player.move(sf::Vector2f(log->getSpeedX() * dt, 0.f));

            // Scoring Logic: Increase if player moves to a new "highest" Y position
            float currentY = player.getPosition().y;
            if (currentY < highestY) {
                score++;
                highestY = currentY;
            }

            // Death Checks (Vehicles, Water, or Falling off camera edges)
            float cameraLeft   = 0.f;
            float cameraRight  = (float)width;
            float bottomEdge = cameraY + height / 2.f;

            if (Collision::checkVehicleHit(player.getGlobalBounds(), obstacles) ||
                Collision::checkDrowning(player.getGlobalBounds(), lanes, obstacles) ||
                player.getPosition().y > bottomEdge ||
                player.getPosition().x < cameraLeft   || 
                player.getPosition().x + PLAYER_SIZE > cameraRight
                ) {
                // Handle High Score persistence
                if (score > highScore) {
                    highScore = score;
                    saveHighScore(SCORE_FILE, highScore);
                }
                state = GameState::DEAD;
                audio.playSound("death");
            }

            scoreText.setString(to_string(score));
            highScoreText.setString(to_string(highScore));
        }

        // ─── DRAW ─────────────────────────────────────────
        window.clear(Color(34, 139, 34)); // Grass Green background

        if (state == GameState::PLAYING) {
            window.setView(camera);
            
            // Draw game world
            for (auto& lane : lanes) lane.draw(window);
            for (auto* obs : obstacles) obs->draw(window);
            window.draw(player);

            // Switch to default view for static UI overlay
            window.setView(window.getDefaultView());
            window.draw(scoreText);
            window.draw(highScoreText);
        }
        else {
            // Draw Menu/Death Background
            window.setView(window.getDefaultView());
            window.draw(bgSprite);

            if (state == GameState::DEAD) {
                // Game Over Title
                Text deathText(font);
                deathText.setCharacterSize(70);
                deathText.setFillColor(Color::Red);
                deathText.setString("GAME OVER");
                FloatRect db = deathText.getGlobalBounds();
                deathText.setPosition(sf::Vector2f(
                    width / 2.f - db.size.x / 2.f,
                    height / 2.f - 80.f
                ));
                window.draw(deathText);

                // Final Score display
                Text finalScore(font);
                finalScore.setCharacterSize(60);
                finalScore.setFillColor(Color(21, 57, 108));
                finalScore.setString(to_string(score));
                FloatRect fb = finalScore.getGlobalBounds();
                finalScore.setPosition(sf::Vector2f(
                    width / 2.f - fb.size.x / 2.f,
                    height / 2.f - 20.f
                ));
                window.draw(finalScore);

                highScoreText.setString("Best: " + to_string(highScore));
                highScoreText.setPosition(sf::Vector2f(10.f, 60.f));
                window.draw(highScoreText);
            }

            window.draw(promptText);
        }

        window.display();
    }

    // Cleanup allocated memory
    for (auto* obs : obstacles)
        delete obs;

    return 0;
}
