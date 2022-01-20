#include "controller.h"
#include <iostream>
#include "SDL.h"
#include "snake.h"

void Controller::ChangeDirection(Snake &snake, Snake::Direction input,
                                 Snake::Direction opposite) const {
  if (snake.direction != opposite || snake.size == 1) snake.direction = input;
  return;
}

void Controller::HandleInput(bool &running, Snake &snake) const {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      running = false;
    } else if (e.type == SDL_KEYDOWN) {
      switch (e.key.keysym.sym) {
        case SDLK_UP:
          ChangeDirection(snake, Snake::Direction::kUp,
                          Snake::Direction::kDown);
          break;

        case SDLK_DOWN:
          ChangeDirection(snake, Snake::Direction::kDown,
                          Snake::Direction::kUp);
          break;

        case SDLK_LEFT:
          ChangeDirection(snake, Snake::Direction::kLeft,
                          Snake::Direction::kRight);
          break;

        case SDLK_RIGHT:
          ChangeDirection(snake, Snake::Direction::kRight,
                          Snake::Direction::kLeft);
          break;
      }
    }
  }
}

vector<Snake::Direction> Controller::AutoMov(const vector<vector<int>> &solution, Snake &snake) const {
  vector<Snake::Direction> step;

  auto dir = [](vector<int>a, vector<int>b){
    if (a[0]!=b[0])
      return b[0]>a[0] ? Snake::Direction::kRight : Snake::Direction::kLeft;
    else
      return b[1]>a[1] ? Snake::Direction::kDown : Snake::Direction::kUp;
  };

  for (int i=1; i<solution.size();++i){
    step.push_back(dir(solution[i-1], solution[i]));
  }

  // This part is only for test
  for (auto i:step){
    switch (i)
    {
      case Snake::Direction::kUp:
        std::cout<<"Up ";
        break;

      case Snake::Direction::kDown:
        std::cout<<"Down ";
        break;

      case Snake::Direction::kLeft:
        std::cout<<"Left ";
        break;

      case Snake::Direction::kRight:
        std::cout<<"Right ";
        break;    
      
      default:
        break;
    }
  }
    return step;
}