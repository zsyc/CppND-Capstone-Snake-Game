#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <vector>
#include "snake.h"
using std::vector;

class Controller {
 public:
  void HandleInput(bool &running, Snake &snake) const;
  vector<Snake::Direction> AutoMov(const vector<vector<int>> &solution, Snake &snake) const;

 private:
  void ChangeDirection(Snake &snake, Snake::Direction input,
                       Snake::Direction opposite) const;
};

#endif