#include <iostream>
#include <vector>

#include "/opt/homebrew/include/SDL2/SDL.h"
#include "gravity.h"

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Simulation of Particles Falling", SDL_WINDOWPOS_UNDEFINED,
                                            SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    std::vector<Particle> particles;
    for(int i = 0; i < 1000; i++) {
        double x = (double)rand() / RAND_MAX * WINDOW_WIDTH;
        double y = (double)rand() / RAND_MAX * WINDOW_HEIGHT;
        double vx = (double)rand() / RAND_MAX * 2 - 1;
        double vy = (double)rand() / RAND_MAX * 2 - 1;
        particles.push_back(Particle(x, y, vx, vy));
    }

    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event))
            if (event.type == SDL_QUIT)
                running = false;

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        for (auto& p : particles) {
            p.update();
            p.render(renderer);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(1);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}