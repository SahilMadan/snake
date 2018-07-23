#ifndef SNAKE_TYPES_H_
#define SNAKE_TYPES_H_

namespace snake {

enum class Direction { NONE, UP, DOWN, LEFT, RIGHT };

struct Coordinate {
  int x;
  int y;
};

struct GameParams {
  // Sets the bottom-right corner of the map (i.e. sets the map width and
  // height).
  Coordinate map_bottom_right;

  // The starting position for the snake.
  Coordinate snake_start_position;

  // The starting direction for the snake.
  Direction snake_start_direction;

  // The starting length of the snake.
  int snake_length;

  // Sets the initial food position for the snake.
  Coordinate initial_food_position;

  // Initial number of iterations before updating the snake position.
  int initial_iteration_count;

  // Number of foods that must be eaten to change update iterations.
  int update_iteration_count_food_amout;

  // The value to decrement from iteration count when updating it.
  int iteration_count_decrement;

  // The minimum value for number of iterations before updating the snake
  // position. Cannot be less than 1.
  int min_iteration_count;
};

enum class GameStatus {
  RUNNING,
  GAME_OVER_COLLISION,
  GAME_OVER_SNAKE_TOO_LONG
};

inline bool operator==(const Coordinate& lhs, const Coordinate& rhs) {
  return lhs.x == rhs.x && lhs.y == rhs.y;
}

inline bool operator!=(const Coordinate& lhs, const Coordinate& rhs) {
  return !(lhs == rhs);
}

}  // namespace snake

#endif
