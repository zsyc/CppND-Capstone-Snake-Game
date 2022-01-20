/* asearch.hpp */
#ifndef ASEARCH_HPP
#define ASEARCH_HPP

#include <vector>
#include "SDL.h"
using std::vector;

enum class State {kEmpty, kObstacle, kClosed, kPath, kStart, kGoal};

class ASearch{
public:
    ASearch(size_t, size_t);

    vector<vector<State>> sMap; 
    vector<vector<int>> solution;

    void setObstacle(vector<SDL_Point> body); // set the snake body as obstacle
    void Draw(vector<vector<State>> grid);

    vector<vector<State>> Search(vector<vector<State>> grid, int init[2], int goal[2]);


private:
    const int delta[4][2] = {{-1,0}, {0,-1}, {1,0}, {0,1}};

    int hValue(int x1, int y1, int x2, int y2);
    void expand(const vector<int> &current, int goal[2], vector<vector<int>> &openlist, vector<vector<State>> &grid);

};

#endif