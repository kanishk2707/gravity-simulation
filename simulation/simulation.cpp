//simulation.cpp
//A simple gravity simulation of a solar system with a curvature mesh background.
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <vector>
#include <cmath>

struct Body {
    glm::vec2 pos;        // Position (x, y)
    glm::vec2 vel;        // Velocity
    float mass;
    float radius;
    float r, g, b;        // Color

    void draw() const {
        glColor3f(r, g, b);
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(pos.x, pos.y);
        for(int i = 0; i <= 30; i++) {
            float angle = i * 2 * M_PI / 30;
            glVertex2f(pos.x + cos(angle)*radius, pos.y + sin(angle)*radius);
        }
        glEnd();
    }
};

std::vector<Body> bodies = {
    {{0.0f, 0.0f}, {0.0f, 0.0f}, 50000.0f, 0.1f, 1.0f, 1.0f, 0.0f},   // Sun
    {{-0.6f, -0.4f}, {1.6f, 2.0f}, 10.0f, 0.06f, 0.2f, 0.5f, 1.0f},   // Earth
    {{0.65f, 0.25f}, {-1.7f, -1.3f}, 8.0f, 0.04f, 1.0f, 0.7f, 0.3f}   // Mars
};

const float G = 2.0f; // Gravitational constant (adjust for simulation scale)
float dt = 0.008f;    // Time step

void updatePhysics() {
    for (size_t i = 0; i < bodies.size(); ++i) {
        glm::vec2 force(0.0f);
        for (size_t j = 0; j < bodies.size(); ++j) {
            if (i == j) continue;
            glm::vec2 dir = bodies[j].pos - bodies[i].pos;
            float dist = glm::length(dir) + 1e-3f;
            dir /= dist;
            float f = G * bodies[i].mass * bodies[j].mass / (dist * dist);
            force += f * dir;
        }
        glm::vec2 acc = force / bodies[i].mass;
        bodies[i].vel += acc * dt;
    }
    for (auto& b : bodies) {
        b.pos += b.vel * dt;
    }
}

void drawCurvatureMesh() {
    glColor3f(1, 1, 1);
    for (float x = -1.0f; x <= 1.0f; x += 0.1f) {
        glBegin(GL_LINE_STRIP);
        for (float y = -1.0f; y <= 1.0f; y += 0.1f) {
            float offset = 0.0f;
            for (const auto& b : bodies) {
                float d = glm::length(glm::vec2(x, y) - b.pos);
                offset -= b.mass / (80.0f + 100*d); // Simulate "warping" mesh
            }
            glVertex3f(x, y, offset);
        }
        glEnd();
    }
    for (float y = -1.0f; y <= 1.0f; y += 0.1f) {
        glBegin(GL_LINE_STRIP);
        for (float x = -1.0f; x <= 1.0f; x += 0.1f) {
            float offset = 0.0f;
            for (const auto& b : bodies) {
                float d = glm::length(glm::vec2(x, y) - b.pos);
                offset -= b.mass / (80.0f + 100*d);
            }
            glVertex3f(x, y, offset);
        }
        glEnd();
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawCurvatureMesh();
    for (const auto& b : bodies)
        b.draw();
    glutSwapBuffers();
}

void idle() {
    updatePhysics();
    glutPostRedisplay();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Gravity Simulation - Solar System");
    glClearColor(0, 0, 0, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.0, 0.1, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0,0,2, 0,0,0, 0,1,0);

    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}
