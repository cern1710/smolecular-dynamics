#include <iostream>
#include <vector>

#include <SDL2/SDL.h>
#include "particle.h"

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
        float radius = (float)rand() / RAND_MAX * 5.0f + 2.0f;
        particles.push_back(Particle(x, y, vx, vy, radius));
    }

    bool running = true;
    SDL_Event event;
    while (running) {
        Uint64 start = SDL_GetPerformanceCounter();
        while (SDL_PollEvent(&event))
            if (event.type == SDL_QUIT)
                running = false;

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        for (auto& p : particles) {
            p.update();
            p.render(renderer);
        }
        for (size_t i = 0; i < particles.size(); i++)
            for (size_t j = i + 1; j < particles.size(); j++)
                particles[i].collide(particles[j]);

        SDL_RenderPresent(renderer);
        Uint64 end = SDL_GetPerformanceCounter();
        float elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
        SDL_Delay(floor(16.666f - elapsedMS)); // cap to 60 fps
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}