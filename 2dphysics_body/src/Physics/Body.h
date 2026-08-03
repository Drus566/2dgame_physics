#ifndef BODY_H
#define BODY_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Vec2.h"
#include "Shape.h"

#include <vector>

struct Body {
    bool isColliding = false;

    // Linear motion
    Vec2 position;
    Vec2 velocity;
    Vec2 acceleration;

    // Angular motion
    float rotation;
    float angularVelocity;
    float angularAcceleration;

    // Forces and torque
    Vec2 sumForces;
    float sumTorque;

    // Mass and Moment of Inertia
    float mass;
    float invMass;
    float I;
    float invI;

    // Coefficient of restitution (elasticity)/возвращение
    float restitution;

    // Coefficient of friction
    float friction;

    // Pointer to the shape/geometry of this rigid body
    Shape* shape = nullptr;

    // SDL texture object
    SDL_Texture* texture = nullptr;

    Body(const Shape& shape, float x, float y, float mass);
    ~Body();

    bool IsStatic() const;

    void AddForce(const Vec2& force);
    void AddTorque(float torque);
    void ClearForces();
    void ClearTorque();

    void SetTexture(const char* textureFileName);

    void ApplyImpulse(const Vec2& j);
    void ApplyImpulse(const Vec2& j, const Vec2& r);

    void IntegrateLinear(float dt);
    void IntegrateAngular(float dt);

    void Update(float dt);
};

// struct Body {
//     int radius; 
    
//     Vec2 position;
//     Vec2 velocity;
//     Vec2 acceleration;

//     Vec2 sumForces;

//     float mass;
//     float invMass;

//     Body(float x, float y, float mass);
//     ~Body();

//     void AddForce(const Vec2& force);
//     void ClearForces();

//     void Integrate(float dt);
// };

#endif