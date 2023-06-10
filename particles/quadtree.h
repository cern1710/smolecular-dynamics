#include <vector>
#include <memory>
#include "particle.h"

const int CAPACITY = 4;

class Rectangle {
public:
    glm::vec2 center;
    float width;
    float height;

    Rectangle(glm::vec2 center, float width, float height)
        : center(center), width(width), height(height) {}

    bool contains(Particle* p) {
        return (p->position.x >= center.x - width &&
                p->position.x <  center.x + width &&
                p->position.y >= center.y - height &&
                p->position.y <  center.y + height);
    }

    bool intersects(Rectangle* range) {
        return !(range->center.x - range->width  > center.x + width ||
                 range->center.x + range->width  < center.x - width ||
                 range->center.y - range->height > center.y + height ||
                 range->center.y + range->height < center.y - height);
    }
};

class Quadtree {
public:
    glm::vec2 boundary;
    float width;
    float height;
    std::vector<Particle*> particles;
    std::unique_ptr<Quadtree> NW;
    std::unique_ptr<Quadtree> NE;
    std::unique_ptr<Quadtree> SW;
    std::unique_ptr<Quadtree> SE;

    Quadtree(glm::vec2 boundary, float width, float height)
        : boundary(boundary), width(width), height(height) {}

    bool insert(Particle* p) {
        if (!inBoundary(p)) return false;

        if (particles.size() < CAPACITY) {
            particles.push_back(p);
            return true;
        }
        if (NW == nullptr) subdivide();

        if (NW->insert(p)) return true;
        if (NE->insert(p)) return true;
        if (SW->insert(p)) return true;
        if (SE->insert(p)) return true;

        // if we can't insert the particle into any child, insert it into this node
        particles.push_back(p);
        return true;
    }

    void queryRange(glm::vec2 range, float rangeWidth, float rangeHeight, std::vector<Particle*>& out) {
        if (!intersects(range, rangeWidth, rangeHeight)) return;
        for (Particle* p : particles) {
            if (isInRange(p, range, rangeWidth, rangeHeight)) {
                out.push_back(p);
            }
        }
        if (NW == nullptr) return;

        NW->queryRange(range, rangeWidth, rangeHeight, out);
        NE->queryRange(range, rangeWidth, rangeHeight, out);
        SW->queryRange(range, rangeWidth, rangeHeight, out);
        SE->queryRange(range, rangeWidth, rangeHeight, out);
    }

private:
    bool inBoundary(Particle* p) {
        return (p->position.x >= boundary.x - width &&
                p->position.x <  boundary.x + width &&
                p->position.y >= boundary.y - height &&
                p->position.y <  boundary.y + height);
    }

    void subdivide() {
        glm::vec2 nwBound = glm::vec2(boundary.x - width / 2, boundary.y - height / 2);
        glm::vec2 neBound = glm::vec2(boundary.x + width / 2, boundary.y - height / 2);
        glm::vec2 swBound = glm::vec2(boundary.x - width / 2, boundary.y + height / 2);
        glm::vec2 seBound = glm::vec2(boundary.x + width / 2, boundary.y + height / 2);

        NW = std::make_unique<Quadtree>(nwBound, width / 2, height / 2);
        NE = std::make_unique<Quadtree>(neBound, width / 2, height / 2);
        SW = std::make_unique<Quadtree>(swBound, width / 2, height / 2);
        SE = std::make_unique<Quadtree>(seBound, width / 2, height / 2);
    }

    bool intersects(glm::vec2 range, float rangeWidth, float rangeHeight) {
        return !(range.x - rangeWidth  > boundary.x + width ||
                 range.x + rangeWidth  < boundary.x - width ||
                 range.y - rangeHeight > boundary.y + height ||
                 range.y + rangeHeight < boundary.y - height);
    }

    bool isInRange(Particle* p, glm::vec2 range, float rangeWidth, float rangeHeight) {
        return (p->position.x >= range.x - rangeWidth &&
                p->position.x <  range.x + rangeWidth &&
                p->position.y >= range.y - rangeHeight &&
                p->position.y <  range.y + rangeHeight);
    }
};