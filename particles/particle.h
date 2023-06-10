#include <glm/glm.hpp>
#include <SDL2/SDL.h>

const int WINDOW_WIDTH = 1080;
const int WINDOW_HEIGHT = 720;
const double GRAVITY = 0.1;

struct Particle {
    glm::vec2 position;
    glm::vec2 velocity;
    float radius;
    const double friction = 0.01;
    const double collision_loss = 0.05;
    const double drag_coeff = 0.005;

    Particle(double x, double y, double vx, double vy, float r) : position(x, y), velocity(vx, vy), radius(r) {}

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
        velocity *= (1.0 - friction) * (1.0 - drag_coeff);
        if (position.y > WINDOW_HEIGHT - radius) { // consider radius for bouncing back
            position.y = WINDOW_HEIGHT - radius;
            velocity.y *= -(1.0 - collision_loss);
        }
    }

    // simplified collision; assumes two-particle
    void collide(Particle& other) {
        glm::vec2 diff = position - other.position;
        if (glm::length(diff) < radius + other.radius) {
            float dist = glm::length(diff);
            float mass = radius * radius;
            float other_mass = other.radius * other.radius;
            float total_mass = mass + other_mass;
            glm::vec2 normal = diff / dist; // normalised collision vector (direction of collision)
        
            // calculate momentum exchange along the collision normal
            // p = amount of velocity to exchange between 2 particles
            float p = 2.0f * (glm::dot(velocity, normal)-glm::dot(other.velocity, normal)) / total_mass;

            // update velocities based on the momentum change
            // distributes proportionally to the particle mass
            velocity       -= p * normal * other_mass;
            other.velocity += p * normal * mass;

            // inelastic collision by reducing speed
            velocity       *= 1.0 - collision_loss;
            other.velocity *= 1.0 - collision_loss;

            // resolve overlapping of particles along the collision normal
            // displacement is proportional to the particle mass
            float overlap   = 0.5f * (dist - (radius + other.radius));
            float buffer = 1.0f;
            overlap -= buffer;
            position       -= overlap * normal * (other_mass / total_mass);
            other.position += overlap * normal * (mass / total_mass);
        }
    }

    void render(SDL_Renderer* renderer) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        drawCircle(renderer, position.x, position.y, radius);
    }
};