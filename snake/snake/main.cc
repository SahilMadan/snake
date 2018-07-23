#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <memory>
#include <vector>
#include "game.h"
#include "types.h"

constexpr int BLOCK_SIZE = 20;
constexpr int HEIGHT_NUM_BLOCKS = 25;
constexpr int WIDTH_NUM_BLOCKS = 25;

std::vector<sf::RectangleShape> GetSnakeBody(const snake::Game& game) {
  std::vector<sf::RectangleShape> snake_body;
  for (int i = 0; i < game.GetSnakeLength(); i++) {
    auto shape = sf::RectangleShape(sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE));
    auto position = game.GetSnakePosition(i);
    shape.setPosition(
        sf::Vector2f(position.x * BLOCK_SIZE, position.y * BLOCK_SIZE));
    shape.setFillColor(sf::Color(100, 250, 50));
    snake_body.push_back(shape);
  }
  return snake_body;
}

sf::RectangleShape GetFoodPosition(const snake::Game& game) {
  auto food = sf::RectangleShape(sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE));
  auto position = game.GetFoodPosition();
  food.setPosition(
      sf::Vector2f(position.x * BLOCK_SIZE, position.y * BLOCK_SIZE));
  food.setFillColor(sf::Color(250, 50, 100));
  return food;
}

void UpdateSnakeDirectionFromUserInput(snake::Game* game) {
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
    game->SetSnakeDirection(snake::Direction::LEFT);
  } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
    game->SetSnakeDirection(snake::Direction::RIGHT);
  } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
    game->SetSnakeDirection(snake::Direction::UP);
  } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
    game->SetSnakeDirection(snake::Direction::DOWN);
  }
}

int main() {
  sf::RenderWindow window(sf::VideoMode(BLOCK_SIZE * WIDTH_NUM_BLOCKS,
                                        BLOCK_SIZE * HEIGHT_NUM_BLOCKS),
                          "snake");
  sf::Clock clock;

  auto params = snake::GameParams(
      {{WIDTH_NUM_BLOCKS - 1, HEIGHT_NUM_BLOCKS - 1} /* map_bottom_right */,
       {10, 10} /* snake_start_position */,
       snake::Direction::RIGHT /* snake_start_direction */,
       2 /* snake_length */,
       {20, 10} /* initial_food_position */,
       10, /* initial_iteration_count */
       5,  /* update_iteration_count_food_amount */
       1,  /* iteration_count_decrement */
       1 /* min_iteration_count */});

  auto game = std::make_unique<snake::Game>(params);

  // Game loop.
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    // Restart game if requested.
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
      if (game->GetGameStatus() == snake::GameStatus::GAME_OVER_COLLISION ||
          game->GetGameStatus() ==
              snake::GameStatus::GAME_OVER_SNAKE_TOO_LONG) {
        game = std::make_unique<snake::Game>(params);
        clock.restart();
      }
    }

    // Read user input.
    UpdateSnakeDirectionFromUserInput(game.get());

    // Ensure game runs at 60fps.
    sf::Time elapsed = clock.getElapsedTime();
    if (elapsed < sf::milliseconds(17)) {
      continue;
    }

    clock.restart();
    game->RunSingleIteration();

    // Render game.
    window.clear(sf::Color::Black);
    auto snake_body = GetSnakeBody(*game);
    for (auto& shape : snake_body) {
      window.draw(shape);
    }
    window.draw(GetFoodPosition(*game));
    window.display();
  }

  return 0;
}
