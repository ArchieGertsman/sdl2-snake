#pragma once

#include <SDL2/SDL.h>
#include <string>
#include "game_state.h"

struct Window
{
    std::string name;
    int height;
    int width;
    SDL_Window *handle = nullptr;
};

class App
{
public:
    App(const std::string &window_name, int window_height, int window_width,
        int grid_rows, int grid_cols);

    bool init();

    void shutdown();

    void run();

private:
    void render();

    bool pollEvents();

private:
    Window _window;
    SDL_Renderer *_renderer;
    GameState _game_state;
    SDL_Event _e;
};