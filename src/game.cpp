#include "game.h"
#include <iostream>
#include "SDL.h"
#include "button.hpp"
#include "asearch.hpp"

//temp
#include <chrono>
#include <thread>
using namespace std::this_thread;
using namespace std::chrono_literals;


int restart = 1;

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)) {
  PlaceFood();
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake);
    Update();
    renderer.Render(snake, food);

    if (!snake.alive){
      // pop up a button asking: retry or quit?
      Button bt(renderer.getWindow());
      if (bt.getButtonId()==0)
        return;
      else if (bt.getButtonId()==1){
        running = false;
        restart = -1;
        return;
      }
      else{
        restart = 0;
        return;
      }
    }

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake.SnakeCell(x, y)) {
      food.x = x;
      food.y = y;
      return;
    }
  }
}

void Game::Update() {
  if (!snake.alive) return;

  snake.Update();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  // Check if there's food over here
  if (food.x == new_x && food.y == new_y) {
    score++;
    PlaceFood();
    // Grow snake and increase speed.
    snake.GrowBody();
    snake.speed += 0.02;
  }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }

void Game::RunAI(Controller const &controller, Renderer &renderer, std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;

  bool running = true;
  while (running) {
    frame_start = SDL_GetTicks();

    int start[2] = {static_cast<int>(snake.head_x), static_cast<int>(snake.head_y)};  
    int goal[2] = {GetFood().x, GetFood().y};

    //coordinte is not the same with Asearch-Array
    int startR[2] = {static_cast<int>(snake.head_y), static_cast<int>(snake.head_x)};  
    int goalR[2] = {GetFood().y, GetFood().x};
    vector<vector<State>> path{};
    vector<Snake::Direction> step;

    ASearch route(renderer.getGridHeight(), renderer.getGridWidth());
    route.setObstacle(snake.body);
    path = route.Search(route.sMap, startR, goalR);
    route.Draw(path);

    if (path.size()!=0){      
      step = controller.AutoMov(route.solution, snake);
      for (int i=0; i<step.size(); ++i){
        snake.direction = step[i];
        while(static_cast<int>(snake.head_x)!= route.solution[i+1][0] || static_cast<int>(snake.head_y)!= route.solution[i+1][1]){
          Update();
          renderer.Render(snake, food);
          frame_end = SDL_GetTicks();
          frame_count++;
          frame_duration = frame_end - frame_start;
          if (frame_end - title_timestamp >= 1000) {
            renderer.UpdateWindowTitle(score, frame_count);
            frame_count = 0;
            title_timestamp = frame_end;
          }
          if (frame_duration < target_frame_duration) 
            SDL_Delay(target_frame_duration - frame_duration);
        }
      }
      Update();
      renderer.Render(snake, food);
      //running = false;
    }
    else
      running = false;
  }
}

