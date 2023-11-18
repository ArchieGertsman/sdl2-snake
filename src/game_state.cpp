#include "game_state.h"
#include "colors.h"

// #include <iostream>

GameState::GameState(int rows, int cols)
    : _rows(rows), _cols(cols)
{
}

GameState::~GameState()
{
    if (this->_grid)
    {
        delete this->_grid;
        this->_grid = nullptr;
    }
}

void GameState::reset()
{
    auto num_cells = this->_rows * this->_cols;
    this->_grid = new int[num_cells];
    memset(this->_grid, 0, num_cells * sizeof(int));
}

int *GameState::getCell(int row, int col)
{
    return this->_grid + (row * this->_cols + col);
}