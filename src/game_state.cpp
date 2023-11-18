#include "game_state.h"
#include "colors.h"

GameState::GameState(int rows, int cols, int update_interval)
    : n_rows(rows), n_cols(cols), _update_interval(update_interval),
      _gen(std::random_device()())
{
    auto num_cells = this->n_rows * this->n_cols;
    this->_grid = new int[num_cells];
    memset(this->_grid, 0, num_cells * sizeof(int));

    Pos pos{(int)(rows / 2), (int)(cols / 2)};
    this->_snake.push_front(pos);

    this->newFood();

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
        return true;

    this->_interval_start = interval_end;

    if (!this->updateHead())
        return false;

    if (this->headPos() == this->_food_pos)
        this->newFood();
    else
        this->popTail();

    return true;
}

bool GameState::updateHead()
{
    auto pos = this->_snake.front();

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

    if (checkCollision(pos))
    {
        return false;
    }

    this->setCell(pos, GridCell::SNAKE);
    this->_snake.push_front(pos);

    return true;
}

bool GameState::checkCollision(const Pos &pos) const
{
    return this->getCell(pos) == GridCell::SNAKE ||
           pos.row == -1 || pos.row == this->n_rows ||
           pos.col == -1 || pos.col == this->n_cols;
}

void GameState::popTail()
{
    auto tail_pos = this->_snake.back();
    this->setCell(tail_pos, GridCell::BG);
    this->_snake.pop_back();
}

void GameState::newFood()
{
    auto num_cells = this->n_rows * this->n_cols;
    rand_int dist(0, num_cells - this->_snake.size() - 1);
    auto i = dist(this->_gen);
    int bg_count = 0;
    for (auto j = 0; j < num_cells; ++j)
    {
        if (this->_grid[j] != GridCell::BG)
            continue;

        if (bg_count == i)
        {
            this->_grid[j] = GridCell::FOOD;
            this->_food_pos = Pos{(int)(j / this->n_rows), j % this->n_cols};
        }

        ++bg_count;
    }
}