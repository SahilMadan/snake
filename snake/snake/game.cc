#include "game.h"

#include <exception>
#include <random>
#include <vector>
#include "types.h"

namespace snake {

Game::Game(GameParams params)
    : snake_length_(params.snake_length),
      snake_head_index_(0),
      snake_direction_(params.snake_start_direction),
      new_snake_direction_(Direction::NONE),
      food_position_(params.initial_food_position),
      map_max_coordinate_(params.map_bottom_right),
      game_status_(GameStatus::RUNNING),
      food_eaten_(0),
      num_iterations_before_updating_(params.initial_iteration_count),
      num_food_before_updating_iteration_count_(
          params.update_iteration_count_food_amout),
      iteration_counter_(0),
      iteration_decrement_(params.iteration_count_decrement),
      iteration_min_(params.min_iteration_count > 0 ? params.min_iteration_count
                                                    : 1) {
  snake_ = std::vector<Coordinate>(GetMaxSnakeLength(), {0, 0});

  InitSnake(params);

  valid_food_positions_buffer_ =
      std::vector<Coordinate>(GetMaxSnakeLength(), {0, 0});
}

snake::GameStatus Game::RunSingleIteration() {
  if (game_status_ != GameStatus::RUNNING) {
    return game_status_;
  }

  iteration_counter_++;
  if (iteration_counter_ <= num_iterations_before_updating_) {
    return game_status_;
  }
  iteration_counter_ = 0;

  if (new_snake_direction_ != Direction::NONE) {
    snake_direction_ = new_snake_direction_;
    new_snake_direction_ = Direction::NONE;
  }

  auto new_head = GetNewSnakeHead();

  if (new_head == food_position_) {
    food_eaten_++;
    UpdateIterationCheck();
    game_status_ = ExtendSnake(new_head);
    CreateNewFood();
  } else {
    game_status_ = MoveSnake(new_head);
  }

  return game_status_;
}

snake::GameStatus Game::GetGameStatus() const { return game_status_; }

int Game::GetSnakeLength() const { return snake_length_; }

int Game::GetFoodEaten() const { return food_eaten_; }

Direction Game::GetSnakeDirection() const { return snake_direction_; }

Coordinate Game::GetSnakePosition(int index) const {
  int circular_buffer_index = (snake_head_index_ + index) % snake_length_;
  return snake_[circular_buffer_index];
}

Coordinate Game::GetFoodPosition() const { return food_position_; }

Coordinate Game::GetMaxMapCoordinate() const { return map_max_coordinate_; }

void Game::SetSnakeDirection(Direction direction) {
  switch (direction) {
    case Direction::NONE:
      break;
    case Direction::LEFT:
      if (snake_direction_ != Direction::RIGHT) {
        new_snake_direction_ = direction;
      }
      break;
    case Direction::RIGHT:
      if (snake_direction_ != Direction::LEFT) {
        new_snake_direction_ = direction;
      }
      break;
    case Direction::UP:
      if (snake_direction_ != Direction::DOWN) {
        new_snake_direction_ = direction;
      }
      break;
    case Direction::DOWN:
      if (snake_direction_ != Direction::UP) {
        new_snake_direction_ = direction;
      }
    default:
      break;
  }
}

void Game::InitSnake(GameParams params) {
  for (int i = 0; i < params.snake_length; i++) {
    switch (params.snake_start_direction) {
      case Direction::UP:
        snake_[i] = {params.snake_start_position.x,
                     params.snake_start_position.y + i};
        break;
      case Direction::DOWN:
        snake_[i] = {params.snake_start_position.x,
                     params.snake_start_position.y - i};
        break;
      case Direction::LEFT:
        snake_[i] = {params.snake_start_position.x + i,
                     params.snake_start_position.y};
        break;
      case Direction::RIGHT:
        snake_[i] = {params.snake_start_position.x - i,
                     params.snake_start_position.y};
        break;
      default:
        // TODO(sahilmadan): Throw error.
        break;
    }
  }
}

Coordinate Game::GetNewSnakeHead() const {
  auto& previous_head = snake_[snake_head_index_];
  switch (snake_direction_) {
    case Direction::UP:
      return {previous_head.x, previous_head.y - 1};
    case Direction::DOWN:
      return {previous_head.x, previous_head.y + 1};
    case Direction::LEFT:
      return {previous_head.x - 1, previous_head.y};
    case Direction::RIGHT:
      return {previous_head.x + 1, previous_head.y};
    default:
      throw std::exception("Invalid snake direction.");
  }
}

snake::GameStatus Game::ExtendSnake(const Coordinate& new_head) {
  snake_length_++;
  if (snake_length_ >= GetMaxSnakeLength()) {
    return snake::GameStatus::GAME_OVER_SNAKE_TOO_LONG;
  }
  for (int i = snake_length_ - 1; i >= snake_head_index_; i--) {
    snake_[i + 1] = snake_[i];
  }
  snake_[snake_head_index_] = new_head;
  return snake::GameStatus::RUNNING;
}

snake::GameStatus Game::MoveSnake(const Coordinate& new_head) {
  snake_head_index_ =
      snake_head_index_ ? snake_head_index_ - 1 : snake_length_ - 1;
  snake_[snake_head_index_] = new_head;

  if (HasCollidedWithSnake() || HasCollidedWithWall()) {
    return GameStatus::GAME_OVER_COLLISION;
  }

  return GameStatus::RUNNING;
}

void Game::CreateNewFood() {
  int valid_food_count = 0;
  for (int x = 0; x <= map_max_coordinate_.x; x++) {
    for (int y = 0; y <= map_max_coordinate_.y; y++) {
      if (y == map_max_coordinate_.y) {
        bool valid = true;
      }
      bool valid = true;
      for (int i = 0; i < snake_length_; i++) {
        if (snake_[i].x == x && snake_[i].y == y) {
          valid = false;
          break;
        }
      }
      if (valid) {
        valid_food_positions_buffer_[valid_food_count].x = x;
        valid_food_positions_buffer_[valid_food_count].y = y;
        valid_food_count++;
      }
    }
  }

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, valid_food_count - 1);
  food_position_ = valid_food_positions_buffer_[dis(gen)];
}

bool Game::HasCollidedWithSnake() const {
  for (int i = 0; i < snake_length_; i++) {
    if (i == snake_head_index_) {
      continue;
    }

    if (snake_[i] == snake_[snake_head_index_]) {
      return true;
    }
  }
  return false;
}

bool Game::HasCollidedWithWall() const {
  return (snake_[snake_head_index_].x < 0 ||
          snake_[snake_head_index_].x > map_max_coordinate_.x ||
          snake_[snake_head_index_].y < 0 ||
          snake_[snake_head_index_].y > map_max_coordinate_.y);
}

int Game::GetMaxSnakeLength() const {
  return (map_max_coordinate_.x + 1) * (map_max_coordinate_.y + 1);
}

void Game::UpdateIterationCheck() {
  if ((food_eaten_ % num_food_before_updating_iteration_count_) == 0) {
    num_iterations_before_updating_ -= iteration_decrement_;
    if (num_iterations_before_updating_ < iteration_min_) {
      num_iterations_before_updating_ = iteration_min_;
    }
  }
}

}  // namespace snake
