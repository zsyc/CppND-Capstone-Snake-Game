/* asearch.cpp */
#include <algorithm>
#include <iostream>
#include "asearch.hpp"

using std::sort;

ASearch::ASearch(size_t height, size_t width){
    sMap.resize(height);
    for (int i=0; i<height; ++i)
        sMap[i].resize(width, State::kEmpty);
}

void ASearch::setObstacle(vector<SDL_Point> body){
    for (auto p:body)
        sMap[p.y][p.x] = State::kObstacle;
}
int ASearch::hValue(int x1, int y1, int x2, int y2){
    return abs(x1-x2) + abs(y1-y2);
}

void ASearch::expand(const vector<int> &current, int goal[2], vector<vector<int>> &openlist, vector<vector<State>> &grid){
    int x = current[0], y = current[1], g = current[2];
    auto checkEmpty = [](int x, int y, vector<vector<State>> &gd){return (0<=x && x<gd.size()) && (0<=y && y<gd[0].size()) && (gd[x][y]==State::kEmpty);};

    for (int i=0; i<4; ++i){
        int x2 = x+delta[i][0];
        int y2 = y+delta[i][1];
        if (checkEmpty(x2, y2, grid)){
            int g2 = g+1;
            int h2 = hValue(x2, y2, goal[0], goal[1]);
            openlist.push_back(vector<int>{x2, y2, g2, h2});
            grid[x2][y2] = State::kClosed;
        }
    }
}

vector<vector<State>> ASearch::Search(vector<vector<State>> grid, int init[2], int goal[2]){
    vector<vector<int>> open{};

    int x = init[0], y = init[1];
    int g = 0;
    int h = hValue(x, y, goal[0], goal[1]);

    // init, start point saved
    open.push_back(vector<int>{x, y, g, h});
    grid[x][y] = State::kClosed;

    auto comp = [](const vector<int>a, const vector<int> b){return a[2]+a[3]>b[2]+b[3];};

    while(open.size()>0){
        std::sort(open.begin(), open.end(), comp);
        auto current = open.back();
        open.pop_back();
        x = current[0];
        y = current[1];
        grid[x][y] = State::kPath;
        /* Solution is the cordinate from snake head to the food: for example: snakehead(16,16), food(17,18)
         * the solution can be {{16,16}, {17,16}, {17,17}, {17,18}} */
        solution.push_back(vector<int>{y, x});  
        
        if (x==goal[0] && y==goal[1]){
            grid[init[0]][init[1]] = State::kStart;
            grid[goal[0]][goal[1]] = State::kGoal;
            return grid;
        }
        expand(current, goal, open, grid);
    }
    // failed to find a path
    solution = {};
    return vector<vector<State>>{};
}

void ASearch::Draw(vector<vector<State>> grid){
    for (int i=0;i<grid.size();++i){
        for (int j=0;j<grid[0].size();++j)
            if (grid[i][j]==State::kPath) std::cout<<"P ";
            else if (grid[i][j]==State::kEmpty) std::cout<<"0 ";
            else if (grid[i][j]==State::kObstacle) std::cout<<"K ";
            else if (grid[i][j]==State::kStart) std::cout<<"S ";
            else if (grid[i][j]==State::kGoal) std::cout<<"G ";
            else std::cout<<"C "; 
            
        std::cout<<std::endl;
    }
}