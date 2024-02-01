#include <SFML/Graphics.hpp>
#include <ctime>
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Pong Game Window");

    // Ball
    sf::CircleShape ball(20);
    ball.setFillColor(sf::Color::White);
    ball.setPosition(375, 275); // Center of the window

    // Ball Speed Vector
    srand(static_cast<unsigned int>(time(NULL)));
    sf::Vector2f ballVelocity;
    float minHorizontalSpeed = 0.05f;

    ballVelocity.x = minHorizontalSpeed + static_cast<float>((rand() % 7 - 3) * 0.05);
    if (std::abs(ballVelocity.x) < 0.05f){
        ballVelocity.x = (ballVelocity.x > 0) ? 0.05f : -0.05f;
    }

    ballVelocity.y = 0.03f;

    // Paddle
    sf::RectangleShape paddle(sf::Vector2f(100, 20)); // Width: 100, Height: 20
    paddle.setFillColor(sf::Color::White);
    paddle.setPosition(350, 550); // Position towards the bottom of the window

    // Game Over
    sf::Font font;
    if (!font.loadFromFile("../assets/fonts/twenty_eight_days_later.ttf")) {
        // Handle error, e.g., print a message
        std::cerr << "Failed to load font from assets/fonts/YourFont.ttf\n";
    }

    sf::Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setString("GAME OVER");
    gameOverText.setCharacterSize(48);
    gameOverText.setFillColor(sf::Color::White);
    gameOverText.setStyle(sf::Text::Bold);
    sf::FloatRect textRect = gameOverText.getLocalBounds();
    gameOverText.setOrigin(textRect.left + textRect.width/2.0f,
                       textRect.top  + textRect.height/2.0f);
    gameOverText.setPosition(sf::Vector2f(window.getSize().x/2.0f, window.getSize().y/2.0f));

    bool isGameOver = false;

    //Score Counter
    int scoreCount = 0;

    sf::Text scoreCountText;
    scoreCountText.setFont(font);
    scoreCountText.setCharacterSize(48);
    scoreCountText.setFillColor(sf::Color::White);
    scoreCountText.setStyle(sf::Text::Bold);
    scoreCountText.setPosition(10,10);




    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }


        if (!isGameOver) {
            ball.move(ballVelocity.x, ballVelocity.y);

            // Collision with left or right wall
            if (ball.getPosition().x < 0 || ball.getPosition().x + ball.getRadius() * 2 > window.getSize().x) {
                ballVelocity.x = -ballVelocity.x; // Reverse x velocity
            }

            if (ball.getPosition().y < 0){
                ballVelocity.y = -ballVelocity.y;
                scoreCount++;
                scoreCountText.setString("Score " + std::to_string(scoreCount));

                float speedIncrease = 0.01f;

                ballVelocity.x += (ballVelocity.x > 0 ? speedIncrease : -speedIncrease);
                ballVelocity.y += (ballVelocity.y > 0 ? speedIncrease : -speedIncrease);
            }

            if (ball.getPosition().y + ball.getRadius() * 2 > window.getSize().y){
                isGameOver = true;
            }

            if (ball.getGlobalBounds().intersects(paddle.getGlobalBounds())){
                ballVelocity.y = -ballVelocity.y;
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
                paddle.move(-0.1,0);
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
                paddle.move(0.1,0);
            }

            if(paddle.getPosition().x < 0) {
                paddle.setPosition(0, paddle.getPosition().y);
            } else if (paddle.getPosition().x + paddle.getSize().x > window.getSize().x){
                paddle.setPosition(window.getSize().x - paddle.getSize().x, paddle.getPosition().y);
            }
        }

        window.clear(sf::Color::Blue);
        window.draw(ball);
        window.draw(paddle); // Draw the paddle
        window.draw(scoreCountText);

        if(isGameOver){
            window.draw(gameOverText);
            window.draw(scoreCountText);
        }

        window.display();
    }
}
