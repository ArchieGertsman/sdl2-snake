#include "app.h"
#include "colors.h"

#include <iostream>

App::App(const std::string &window_name, int window_height, int window_width,
         int grid_rows, int grid_cols, int update_interval)
    : _game_state(grid_rows, grid_cols, update_interval)
{
    this->_window.name = window_name;
    this->_window.height = window_height;
    this->_window.width = window_width;

    this->_cell_rect.w = this->_window.width / this->_game_state.n_cols;
    this->_cell_rect.h = this->_window.height / this->_game_state.n_rows;
}

bool App::init()
{
    if (SDL_Init(SDL_INIT_EVERYTHING))
    {
        std::cerr << "SDL_Init failed with error: ";
        std::cerr << SDL_GetError() << '\n';
        return false;
    }

    this->_window.handle = SDL_CreateWindow(this->_window.name.c_str(),
                                            SDL_WINDOWPOS_CENTERED,
                                            SDL_WINDOWPOS_CENTERED,
                                            this->_window.width,
                                            this->_window.height,
                                            SDL_WINDOW_OPENGL);

    if (!this->_window.handle)
    {
        std::cerr << "Window creation failed with error: ";
        std::cerr << SDL_GetError() << '\n';
        SDL_Quit();
        return false;
    }

    this->_renderer = SDL_CreateRenderer(this->_window.handle, -1,
                                         SDL_RENDERER_PRESENTVSYNC);

    return true;
}

void App::shutdown()
{
    if (this->_renderer)
    {
        SDL_DestroyRenderer(this->_renderer);
        this->_renderer = nullptr;
    }

    if (this->_window.handle)
    {
        SDL_DestroyWindow(this->_window.handle);
        this->_window.handle = nullptr;
    }

    SDL_Quit();
}

void App::run()
{
    while (true)
    {
        if (!this->pollEvents() || !this->_game_state.update())
        {
            return;
        }

        this->render();
    }
}

void App::render()
{
    for (auto row = 0; row < this->_game_state.n_rows; ++row)
    {
        for (auto col = 0; col < this->_game_state.n_cols; ++col)
        {
            this->renderCell(Pos{row, col});
        }
    }

    SDL_RenderPresent(this->_renderer);
}

void App::renderCell(const Pos &pos)
{
    auto cell = this->_game_state.getCell(pos);

    this->_cell_rect.x = pos.col * this->_cell_rect.w;
    this->_cell_rect.y = pos.row * this->_cell_rect.h;

    SDL_Color color;

    switch (cell)
    {
    case GridCell::FOOD:
        color = Colors::RED;
        break;
    case GridCell::SNAKE:
        color = Colors::BLUE;
        break;
    default:
        color = Colors::BLACK;
        break;
    }

    SDL_SetRenderDrawColor(this->_renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(this->_renderer, &this->_cell_rect);
}

bool App::pollEvents()
{
    if (!SDL_PollEvent(&this->_e))
    {
        return true;
    }

    switch (this->_e.type)
    {
    case SDL_KEYDOWN:
    {
        auto code = this->_e.key.keysym.scancode;
        if (code == SDL_SCANCODE_ESCAPE)
        {
            return false;
        }
        else
        {
            this->_game_state.handleKeyDown(code);
            return true;
        }
    }
    case SDL_QUIT:
        return false;
    default:
        return true;
    }
}
