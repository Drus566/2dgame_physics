#include "World.h"
#include "Constans.h"
#include "CollisionDetection.h"
#include <iostream>

World::World(float gravity) {
    G = -gravity; // minus - Y coord 
    std::cout << "World constructor called" << std::endl;
}

World::~World() {
    for (auto body : bodies) {
        delete body;
    }
    std::cout << "World destructor called!" << std::endl;
}

void World::AddBody(Body* body) {
    bodies.push_back(body);
}

std::vector<Body*>& World::GetBodies() {
    return bodies;
}

void World::AddForce(const Vec2& force) {
    forces.push_back(force);
}

void World::Update(float dt) {
    for (auto body: bodies) {
        Vec2 weight = Vec2(0.0, body->mass * G * PIXELS_PER_METER);
        body->AddForce(weight);
    }
}