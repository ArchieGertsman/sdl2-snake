#pragma once

#include <SDL2/SDL.h>
#include <deque>
#include <random>

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

    bool operator==(const Pos &other)
    {
        return this->row == other.row && this->col == other.col;
    }
};

typedef std::uniform_int_distribution<std::mt19937::result_type> rand_int;

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

    Pos &headPos()
    {
        return this->_snake.front();
    }

    bool updateHead();

    void popTail();

    bool checkCollision(const Pos &pos) const;

    void newFood();

public:
    const int n_rows;
    const int n_cols;

private:
    int *_grid = nullptr;
    int _head_dir = Dir::NONE;
    std::deque<Pos> _snake;
    Pos _food_pos;
    int _interval_start;
    const int _update_interval;
    std::mt19937 _gen;
};