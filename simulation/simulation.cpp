#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <deque>

struct Body {
    glm::vec2 pos;
    glm::vec2 vel;
    glm::vec2 acc;
    float mass;
    float radius;
    glm::vec3 color;
    std::deque<glm::vec2> trail;
    static const int MAX_TRAIL_LENGTH = 500;
};

class Simulation {
public:
    float G = 2.0f;
    float dt = 0.001f;

    Simulation();
    void updatePhysics();
    void addBody(const Body& body);
    void reset();
    const std::vector<Body>& getBodies() const;

private:
    std::vector<Body> bodies;
    void initial_setup();
};