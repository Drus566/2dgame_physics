#ifndef CONTACT_H
#define CONTACT_H

#include "Vec2.h"
#include "Body.h"

struct Contact {
    Body* a;
    Body* b;

    Vec2 start;
    Vec2 end;

    Vec2 normal;
    float depth;

    Contact() = default;
    ~Contact() = default;

    void ResolvePenetration();
    void ResolveCollision();
    
    // void isOverlappingAABB() {
    //     bool isOverlappingX = aLeft <= bRight && bLeft <= aRight;
    //     bool isOverlappingY = aBottom <= bTop && bBottom <= aTop;
    //     return isOverlappingX && isOverlappingY;
    // }
};

#endif