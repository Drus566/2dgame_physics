#include "CollisionDetection.h"

bool CollisionDetection::isColliding(Body* a, Body* b, Contact& contact) {
    bool aIsCircle = a->shape->GetType() == CIRCLE;
    bool bIsCircle = b->shape->GetType() == CIRCLE;

    if (aIsCircle && bIsCircle) {
        return isCollidingCircleCircle(a, b, contact);
    }
}

bool CollisionDetection::isCollidingCircleCircle(Body* a, Body* b, Contact& contact) {
    CircleShape* aCircleShape = (CircleShape*) a->shape;
    CircleShape* bCircleShape = (CircleShape*) b->shape;

    const Vec2 ab = b->position - a->position;
    const float radiusSum  = aCircleShape->radius + bCircleShape->radius;

    bool isColliding = ab.MagnitudeSquared() <= (radiusSum * radiusSum);

    if (!isColliding) {
        return false;
    }

    contact.a = a;
    contact.b = b;

    contact.normal = ab;
    contact.normal.Normalize();

    contact.start = b->position - contact.normal * bCircleShape->radius;
    contact.end = a->position + contact.normal * aCircleShape->radius;
    
    contact.depth = (contact.end - contact.start).Magnitude();

    return true;
}