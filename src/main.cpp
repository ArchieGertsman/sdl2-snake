#include <iostream>
#include <SDL2/SDL.h>

#include "app.h"

int main()
{
    App app("Snake", 512, 512, 16, 16);

    if (!app.init())
    {
        return EXIT_FAILURE;
    }

    app.run();

    app.shutdown();

    return EXIT_SUCCESS;
}