#include <iostream>
#include <SDL2/SDL.h>
#include <string>

#include "app.h"

int main(int argc, char *argv[])
{
    int window_height = 512;
    int window_width = 512;
    int grid_rows = 16;
    int grid_cols = 16;
    int update_interval = 100;

    if (argc >= 3)
    {
        window_height = std::stoi(argv[1]);
        window_width = std::stoi(argv[2]);
    }

    if (argc >= 5)
    {
        grid_rows = std::stoi(argv[3]);
        grid_cols = std::stoi(argv[4]);
    }

    if (argc == 6)
    {
        update_interval = std::stoi(argv[5]);
    }

    App app("Snake", window_width, window_height, grid_rows, grid_cols,
            update_interval);

    if (!app.init())
        return EXIT_FAILURE;

    app.run();

    app.shutdown();

    return EXIT_SUCCESS;
}