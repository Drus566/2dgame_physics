#ifndef COLLISIONDETECTION_H
#define COLLISIONDETECTION_H

#include "Body.h"
#include "Contact.h"

struct CollisionDetection {
    static bool isColliding(Body* a, Body* b, Contact& contact);
    static bool isCollidingCircleCircle(Body* a, Body* b, Contact& contact);
    static bool isCollidingPolygonPolygon(Body* a, Body* b, Contact& contact);
    // TODO static bool isCollidingPolygonCircle(Body* a, Body* b);

};

#endif