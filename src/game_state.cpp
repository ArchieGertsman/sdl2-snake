#include "game_state.h"
#include "colors.h"

// #include <iostream>

GameState::GameState(int rows, int cols, int update_interval)
    : n_rows(rows), n_cols(cols), _update_interval(update_interval)
{
    auto num_cells = this->n_rows * this->n_cols;
    this->_grid = new int[num_cells];
    memset(this->_grid, 0, num_cells * sizeof(int));

    Pos pos_init{(int)(rows / 2), (int)(cols / 2)};
    this->_snake.push_back(pos_init);

    this->_interval_start = SDL_GetTicks();
}

GameState::~GameState()
{
    delete this->_grid;
}

void GameState::handleKeyDown(int code)
{
    switch (code)
    {
    case SDL_SCANCODE_RIGHT:
    case SDL_SCANCODE_LEFT:
    case SDL_SCANCODE_DOWN:
    case SDL_SCANCODE_UP:
    {
        int dir = code - SDL_SCANCODE_RIGHT;
        this->_head_dir = dir;
        break;
    }
    default:
        break;
    }
}

bool GameState::update()
{
    auto interval_end = SDL_GetTicks();
    if (interval_end - this->_interval_start < this->_update_interval)
    {
        return true;
    }

    this->_interval_start = interval_end;

    auto prev_head_pos = this->_snake[0];

    if (!this->updateHead())
    {
        return false;
    }

    this->updateBody(prev_head_pos);

    return true;
}

bool GameState::updateHead()
{
    auto &pos = this->_snake[0];

    this->setCell(pos, GridCell::BG);

    switch (this->_head_dir)
    {
    case Dir::RIGHT:
        pos.col += 1;
        break;
    case Dir::LEFT:
        pos.col -= 1;
        break;
    case Dir::DOWN:
        pos.row += 1;
        break;
    case Dir::UP:
        pos.row -= 1;
        break;
    default:
        break;
    }

    if (this->checkCollision(pos))
    {
        return false;
    }

    this->setCell(pos, GridCell::SNAKE);

    return true;
}

void GameState::updateBody(const Pos &prev_head_pos)
{
    Pos prev = prev_head_pos;
    Pos tmp;
    for (auto i = 1; i < this->_snake.size(); ++i)
    {
        auto pos = this->_snake[i];
        this->setCell(pos, GridCell::BG);

        tmp = this->_snake[i];
        this->_snake[i] = prev;
        prev = tmp;

        pos = this->_snake[i];
        this->setCell(pos, GridCell::SNAKE);
    }
}

bool GameState::checkCollision(const Pos &pos) const
{
    return this->getCell(pos) == GridCell::SNAKE ||
           pos.row == -1 || pos.row == this->n_rows ||
           pos.col == -1 || pos.col == this->n_cols;
}