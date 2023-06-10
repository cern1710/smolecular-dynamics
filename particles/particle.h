#include <glm/glm.hpp>

const int WINDOW_WIDTH = 1080;
const int WINDOW_HEIGHT = 720;
const double GRAVITY = 0.01;

struct Particle {
    glm::vec2 position;
    glm::vec2 velocity;
    int radius = 3;

    Particle(double x, double y, double vx, double vy) : position(x, y), velocity(vx, vy) {}

    void drawCircle(SDL_Renderer* renderer, int x_centre, int y_centre, int radius) {
        for (int w = 0; w < radius * 2; w++)
            for (int h = 0; h < radius * 2; h++) {
                int dx = radius - w;  // horizontal offset
                int dy = radius - h;  // vertical offset

                if ((dx*dx + dy*dy) <= (radius * radius))
                    SDL_RenderDrawPoint(renderer, x_centre + dx, y_centre + dy);
            }
    }
    
    void update() {
        position += velocity;
        velocity.y += GRAVITY;

        if (position.y > WINDOW_HEIGHT - radius) { // consider radius for bouncing back
            position.y = WINDOW_HEIGHT - radius;
            velocity.y *= -0.5;
        }
    }

    void render(SDL_Renderer* renderer) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        drawCircle(renderer, position.x, position.y, radius);
    }
};