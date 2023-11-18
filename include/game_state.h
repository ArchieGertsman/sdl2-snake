#pragma once

#include <SDL2/SDL.h>

enum GridCell
{
    BG,
    FOOD,
    SNAKE
};

class GameState
{
public:
    GameState(int rows, int cols);

    ~GameState();

    void reset();

    int *getCell(int row, int col);

public:
    int _rows;
    int _cols;
    int *_grid = nullptr;
};