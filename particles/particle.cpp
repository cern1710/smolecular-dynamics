#include <iostream>
#include <vector>

#include <SDL2/SDL.h>
#include "quadtree.h"

const static float min = 1.0f;
const static float max = 3.0f;

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Simulation of Particles Falling", SDL_WINDOWPOS_UNDEFINED,
                                            SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    std::vector<Particle> particles;
    for(int i = 0; i < 4000; i++) {
        double x = (double)rand() / RAND_MAX * WINDOW_WIDTH;
        double y = (double)rand() / RAND_MAX * WINDOW_HEIGHT;
        double vx = (double)rand() / RAND_MAX * 2 - 1;
        double vy = (double)rand() / RAND_MAX * 2 - 1;
        float radius = (float)rand() / RAND_MAX * max + min;
        particles.push_back(Particle(x, y, vx, vy, radius));
    }

    bool running = true;
    SDL_Event event;
    while (running) {
        const Uint64 FRAME_TIME = SDL_GetPerformanceFrequency() / 60;  // 60 FPS
        Uint64 start = SDL_GetPerformanceCounter();
        while (SDL_PollEvent(&event))
            if (event.type == SDL_QUIT)
                running = false;

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        // Create a Quadtree that covers the entire window.
        Quadtree qt(glm::vec2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

        // Insert all particles into the Quadtree.
        for (auto& p : particles) {
            p.update();
            qt.insert(&p);
        }

        // Check for collisions using the Quadtree to only check nearby particles.
        for (auto& p : particles) {
            std::vector<Particle*> nearby;
            qt.queryRange(p.position, (max+1)*2, (max+1)*2, nearby);
            for (auto* other : nearby) {
                if (&p != other) {
                    p.collide(*other);
                }
            }
            p.render(renderer);
        }

        SDL_RenderPresent(renderer);
        Uint64 end = SDL_GetPerformanceCounter();
        Uint64 elapsed = end - start;

        if (elapsed < FRAME_TIME) {
            SDL_Delay((FRAME_TIME - elapsed) * 1000 / SDL_GetPerformanceFrequency());
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}