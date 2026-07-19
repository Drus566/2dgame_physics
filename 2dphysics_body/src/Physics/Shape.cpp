#include "Shape.h"

#include <iostream>

CircleShape::CircleShape(float radius) {
    this->radius = radius;
    std::cout << "CircleShape constructor called!" << std::endl;
}

CircleShape::~CircleShape() {
    std::cout << "CircleShape destructor called!" << std::endl;
}

Shape* CircleShape::Clone() const {
    return new CircleShape(this->radius);
}

float CircleShape::GetMomentOfInertia() const {
    // 1/2 * r^2
    return 0.5 * (radius * radius);
}

ShapeType CircleShape::GetType() const {
    return CIRCLE;
};

PolygonShape::PolygonShape(const std::vector<Vec2> verticles) {
    // TODO
};

PolygonShape::~PolygonShape() {
    // TODO
};

float PolygonShape::GetMomentOfInertia() const {
    // TODO
    return 0.0;
}

ShapeType PolygonShape::GetType() const {
    return POLYGON;
};

Shape* PolygonShape::Clone() const {
    return new PolygonShape(localVertices);
}

void PolygonShape::UpdateVertices(float angle, const Vec2& position) {
    // Loop all the vertices, transforming from local to world space
    for (int i = 0; i < localVertices.size(); i++) {
        // first rotate, then we translate
        worldVertices[i] = localVertices[i].Rotate(angle);
        worldVertices[i] += position;
    }
}

BoxShape::BoxShape(float width, float height) {
    this->width = width;
    this->height = height;
    
    // load the verticles of the box polygon 
    localVertices.push_back(Vec2(-width / 2.0, -height / 2.0));
    localVertices.push_back(Vec2(+width / 2.0, -height / 2.0));
    localVertices.push_back(Vec2(+width / 2.0, +height / 2.0));
    localVertices.push_back(Vec2(-width / 2.0, +height / 2.0));

    worldVertices.push_back(Vec2(-width / 2.0, -height / 2.0));
    worldVertices.push_back(Vec2(+width / 2.0, -height / 2.0));
    worldVertices.push_back(Vec2(+width / 2.0, +height / 2.0));
    worldVertices.push_back(Vec2(-width / 2.0, +height / 2.0));
};

BoxShape::~BoxShape() {
    // TODO
};

float BoxShape::GetMomentOfInertia() const {
    // 1/12 * (w^2 + h^2);
    return (0.083333) * (width * width + height * height);
}

ShapeType BoxShape::GetType() const {
    return BOX;
}

Shape* BoxShape::Clone() const {
    return new BoxShape(width, height);
}