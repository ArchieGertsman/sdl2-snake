#include "app.h"

#include <iostream>

App::App(const std::string &window_name, int window_height, int window_width,
         int grid_rows, int grid_cols)
    : _game_state(grid_rows, grid_cols)
{
    this->_window.name = window_name;
    this->_window.height = window_height;
    this->_window.width = window_width;
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
    this->_game_state.reset();

    // for (auto i = 0; i < 5; ++i)
    // {
    //     *this->_game_state.getCell(0, i) = GridCell::SNAKE;
    // }
    // *this->_game_state.getCell(2, 3) = GridCell::FOOD;

    bool running = true;
    while (running)
    {
        running = this->pollEvents();
        this->render();
    }
}

void App::render()
{
    SDL_Rect rect;
    rect.w = this->_window.width / this->_game_state._cols;
    rect.h = this->_window.height / this->_game_state._rows;

    for (auto row = 0; row < this->_game_state._rows; ++row)
    {
        for (auto col = 0; col < this->_game_state._cols; ++col)
        {
            auto cell = *this->_game_state.getCell(row, col);

            rect.x = col * rect.w;
            rect.y = row * rect.h;

            if (cell == GridCell::BG)
            {
                SDL_SetRenderDrawColor(this->_renderer, 0, 0, 0, 255);
            }
            else if (cell == GridCell::FOOD)
            {
                SDL_SetRenderDrawColor(this->_renderer, 255, 0, 0, 255);
            }
            else
            {
                SDL_SetRenderDrawColor(this->_renderer, 0, 0, 255, 255);
            }

            SDL_RenderFillRect(this->_renderer, &rect);
        }
    }

    SDL_RenderPresent(this->_renderer);
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
        return this->_e.key.keysym.scancode != SDL_SCANCODE_ESCAPE;
    }
    case SDL_QUIT:
    {
        return false;
    }
    default:
        break;
    }

    return true;
}