#include <SDL2/SDL.h>

const int WINDOW_WIDTH = 1080;
const int WINDOW_HEIGHT = 720;
const double GRAVITY = 0.01;

struct Particle {
    double x, y;
    double vx, vy;
    int radius = 3;

    Particle(double x, double y, double vx, double vy) : x(x), y(y), vx(vx), vy(vy) {}

    void drawCircle(SDL_Renderer* renderer, int x_centre, int y_centre, int radius) {
        for (int w = 0; w < radius * 2; w++)
            for (int h = 0; h < radius * 2; h++) {
                int dx = radius - w, dy = radius - h;   // horizontal and vertical offset

                if ((dx*dx + dy*dy) <= (radius * radius))
                    SDL_RenderDrawPoint(renderer, x_centre + dx, y_centre + dy);
            }
    }
    
    void update() {
        x += vx;
        y += vy;
        vy += GRAVITY; // gravity effect

        if (y > WINDOW_HEIGHT - radius) { // consider radius for bouncing back
            y = WINDOW_HEIGHT - radius;
            vy *= -0.5;
        }
    }

    void render(SDL_Renderer* renderer) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        drawCircle(renderer, x, y, radius);
    }
};