#ifndef SNAKE_GAME_H_
#define SNAKE_GAME_H_

#include <utility>
#include <vector>
#include "types.h"

namespace snake {

// Simulates snake game.
class Game {
 public:
  Game(GameParams params);

  // Runs a single iteration of the game (i.e. moves the snake, eats food,
  // checks for game over, etc.).
  GameStatus RunSingleIteration();

  GameStatus GetGameStatus() const;

  // Returns the length of the snake (i.e. the number of segments/blocks).
  int GetSnakeLength() const;

  // Returns the maximum possible length for the snake.
  int GetMaxSnakeLength() const;

  // Get the number of food eaten.
  int GetFoodEaten() const;

  // Gets the direction that the snake is heading towards.
  Direction GetSnakeDirection() const;

  // Returns the coordinate of a segment/block of the snake, specified as an
  // index in the range [0, getSnakeLength()).
  Coordinate GetSnakePosition(int index) const;

  // Get the position of the food.
  Coordinate GetFoodPosition() const;

  // Gets the coordinate of max x/y position.
  Coordinate GetMaxMapCoordinate() const;

  // Sets the new snake direction if valid.
  void SetSnakeDirection(Direction direction);

 private:
  std::vector<Coordinate> snake_;
  std::vector<Coordinate> valid_food_positions_buffer_;
  int snake_length_;
  int snake_head_index_;
  Direction snake_direction_;
  Direction new_snake_direction_;
  Coordinate food_position_;
  Coordinate map_max_coordinate_;
  GameStatus game_status_;

  int food_eaten_;

  int num_iterations_before_updating_;
  int num_food_before_updating_iteration_count_;
  int iteration_counter_;
  const int iteration_decrement_;
  const int iteration_min_;

  Coordinate GetNewSnakeHead() const;
  GameStatus ExtendSnake(const Coordinate& new_head);
  GameStatus MoveSnake(const Coordinate& new_head);
  void InitSnake(GameParams params);
  void UpdateIterationCheck();
  bool HasCollidedWithSnake() const;
  bool HasCollidedWithWall() const;
  void CreateNewFood();
};

}  // namespace snake

#endif
