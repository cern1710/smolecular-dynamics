#include <glm/glm.hpp>

const int WINDOW_WIDTH = 1080;
const int WINDOW_HEIGHT = 720;
const double GRAVITY = 0.005;

struct Particle {
    glm::vec2 position;
    glm::vec2 velocity;
    float radius = 3.0f;

    Particle(double x, double y, double vx, double vy) : position(x, y), velocity(vx, vy) {}

    void drawCircle(SDL_Renderer* renderer, int x_centre, int y_centre, int radius) {
        for (int w = 0; w < radius * 2; w++)
            for (int h = 0; h < radius * 2; h++) {
                int dx = radius - w;
                int dy = radius - h;

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

    // simplified collision; assumes perfectly elastic and two-particle
    void collide(Particle& other) {
        glm::vec2 diff = position - other.position;
        if (glm::length(diff) < radius + other.radius) {
            float dist = glm::length(diff);
            float total_radius = radius + other.radius;
            glm::vec2 normal = diff / dist; // normalised collision vector (direction of collision)
        
            // Conservation of momentum (assumes collision occurs in one dimension) - mv
            // Conservation of kinetic energy - mv^2/2
            // p = amount of velocity to exchange between 2 particles
            float p = 2.0f * (glm::dot(velocity, normal)-glm::dot(other.velocity, normal)) / total_radius;

            // update velocities based on the momentum change
            // distributes proportionally to the radii of particles
            velocity = velocity - p * normal * other.radius;
            other.velocity = other.velocity + p * normal * radius;

            // resolve overlapping of particles
            // particles are moved along normal by half the overlap distance
            float overlap = 0.5f * (dist - total_radius);
            position -= overlap * normal;
            other.position += overlap * normal;
        }
    }

    void render(SDL_Renderer* renderer) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        drawCircle(renderer, position.x, position.y, radius);
    }
};