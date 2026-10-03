#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

const sf::Keyboard::Key controls[4] = {
  sf::Keyboard::Key::A,   // Player1 UP
  sf::Keyboard::Key::Z,   // Player1 Down
  sf::Keyboard::Key::W,  // Player2 UP
  sf::Keyboard::Key::D // Player2 Down
};
sf::Vector2f ball_velocity;
bool is_player_serving = true;
const float initial_velocity_x = 100.f; //horizontal velocity
const float initial_velocity_y = 60.f; //vertical velocity
const float velocity_multiplier = 1.1f;

//Parameters
const sf::Vector2f paddleSize(25.f, 100.f);
const float ballRadius = 10.f;
const int gameWidth = 800;
const int gameHeight = 600;
const float paddleSpeed = 400.f;
const float paddleOffsetWall = 10.f;
const float time_step = 0.017f; //60 fps

//Objects of the game
sf::CircleShape ball;
sf::RectangleShape paddles[2];

void reset();
void load();


void init() {



  // Set size and origin of paddles
  for (sf::RectangleShape &p : paddles) {
    p.setSize(paddleSize);
    p.setOrigin(paddleSize / 2.f);
  }
  // Set size and origin of ball
  ball.setRadius(ballRadius);
 ball.setOrigin({ballRadius, ballRadius}); //Should be half the ball width and height
  paddles[0].setPosition({
    paddleOffsetWall,
    gameHeight / 2.f
});

paddles[1].setPosition({
    gameWidth - paddleOffsetWall,
    gameHeight / 2.f
});

ball.setPosition({
    gameWidth / 2.f,
    gameHeight / 2.f
});

}

float playerSpeed = 10.0f;
void update(float dt) {

    // Handle paddle movement
    float direction = 0.0f;

    if (sf::Keyboard::isKeyPressed(controls[0])) {
        direction--;
    }

    if (sf::Keyboard::isKeyPressed(controls[1])) {
        direction++;
    }

    paddles[0].move(
        sf::Vector2f(0.f, direction * paddleSpeed * dt)
    );


    // Player 2
    float direction2 = 0.0f;

    if (sf::Keyboard::isKeyPressed(controls[2])) {
        direction2--;
    }

    if (sf::Keyboard::isKeyPressed(controls[3])) {
        direction2++;
    }

    paddles[1].move(
        sf::Vector2f(0.f, direction2 * paddleSpeed * dt)
    );


    // Move ball
    ball.move(ball_velocity * dt);


    // Check ball collision
    const float bx = ball.getPosition().x;
    const float by = ball.getPosition().y;


    // Bottom wall
    if (by > gameHeight) {

        ball_velocity.x *= velocity_multiplier;
        ball_velocity.y *= -velocity_multiplier;

        ball.move(sf::Vector2f(0.f, -10.f));


    // Top wall
    } else if (by < 0) {

        ball_velocity.x *= velocity_multiplier;
        ball_velocity.y *= -velocity_multiplier;

        ball.move(sf::Vector2f(0.f, 10.f));


    // Right score wall
    } else if (bx > gameWidth) {

        reset();


    // Left score wall
    } else if (bx < 0) {

        reset();


    // Left paddle
    } else if (
        bx < paddleSize.x + paddleOffsetWall &&
        by > paddles[0].getPosition().y - (paddleSize.y * 0.5f) &&
        by < paddles[0].getPosition().y + (paddleSize.y * 0.5f)
    ) {

        ball_velocity.x *= -velocity_multiplier;
        ball.move(sf::Vector2f(10.f, 0.f));


    // Right paddle
    } else if (
        bx > gameWidth - paddleSize.x - paddleOffsetWall &&
        by > paddles[1].getPosition().y - (paddleSize.y * 0.5f) &&
        by < paddles[1].getPosition().y + (paddleSize.y * 0.5f)
    ) {

        ball_velocity.x *= -velocity_multiplier;
        ball.move(sf::Vector2f(-10.f, 0.f));
    }
}

void reset() {
  //reset ball position
  ball.setPosition({gameWidth / 2.f, gameHeight / 2.f});
  //reset paddle position
  paddles[0].setPosition({paddleOffsetWall, gameHeight / 2.f});
  paddles[1].setPosition({gameWidth - paddleOffsetWall, gameHeight / 2.f});
  //reset ball velocity
  is_player_serving = !is_player_serving;
  load();
}
void load() {
  //load resources if necessary.
  ball_velocity = { (is_player_serving ? initial_velocity_x : -initial_velocity_x), initial_velocity_y };
 


}


void render(sf::RenderWindow &window) {
  // Draw Everything
  window.draw(paddles[0]);
  window.draw(paddles[1]);
  window.draw(ball);
}


void clean(){
  //free up the memory if necessary.
}

int main () {


  //create the window
  sf::RenderWindow window(sf::VideoMode({gameWidth, gameHeight}), "PONG");
  window.setVerticalSyncEnabled(false);
  //initialise and load

  init();
  load();
  

  while(window.isOpen()){


    // Process events
    while (const std::optional event = window.pollEvent())
    {
      // Close window: exit
      if (event->is<sf::Event::Closed>())
        window.close();
    }

    // Clear screen
    window.clear();
    update(time_step);
    render(window);
    //wait for the time_step to finish before displaying the next frame.
    sf::sleep(sf::seconds(time_step));
    //Wait for Vsync
    window.display();
  }
  //Unload and shutdown
  clean();
}