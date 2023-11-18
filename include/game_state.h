#pragma once

#include <SDL2/SDL.h>
#include <vector>

enum GridCell
{
    BG,
    FOOD,
    SNAKE
};

enum Dir
{
    RIGHT,
    LEFT,
    DOWN,
    UP,
    NONE
};

struct Pos
{
    int row;
    int col;
};

class GameState
{
public:
    GameState(int rows, int cols, int update_interval);

    ~GameState();

    bool update();

    void handleKeyDown(int code);

    int getCell(const Pos &pos) const
    {
        return this->_grid[pos.row * this->n_cols + pos.col];
    }

private:
    void setCell(const Pos &pos, int val)
    {
        this->_grid[pos.row * this->n_cols + pos.col] = val;
    }

    bool updateHead();

    void updateBody(const Pos &prev_head_pos);

    bool checkCollision(const Pos &pos) const;

public:
    const int n_rows;
    const int n_cols;

private:
    int *_grid = nullptr;
    int _head_dir = Dir::NONE;
    std::vector<Pos> _snake;
    int _interval_start;
    const int _update_interval;
};