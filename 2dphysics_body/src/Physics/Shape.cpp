#include "Shape.h"

#include <iostream>
#include <limits>

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

Vec2 PolygonShape::EdgeAt(int index) const {
    // Find the edge at a certain vertex index (ищет край, если индекс 0, то край от точки 0 до 1)
    int currVertex = index;
    int nextVertex = (index + 1) % worldVertices.size();
    return worldVertices[nextVertex] - worldVertices[currVertex];
}

float PolygonShape::FindMinSeparation(const PolygonShape* other, Vec2& axis, Vec2& point) const {
    float separation =  std::numeric_limits<float>::lowest();
    // Loop all the vertices of "a"
    for (int i = 0; i < this->worldVertices.size(); i++) {
        Vec2 va = this->worldVertices[i];
        Vec2 normal = this->EdgeAt(i).Normal();

        float minSep = std::numeric_limits<float>::max(); 
        Vec2 minVertex;
        // Loop all the vertices of "b"
        for (int j = 0; j < other->worldVertices.size(); j++) {
            Vec2 vb = other->worldVertices[j];
            float proj = (vb - va).Dot(normal);
            if (proj < minSep) {
                minSep = proj;
                minVertex = vb;
            }
            minSep = std::min(minSep, (vb - va).Dot(normal));
        }
        if (minSep > separation) {
            separation = minSep;
            axis = this->EdgeAt(i);
            point = minVertex;
        }
        // separation = std::max(separation, minSep);
    }

    // Return the best separation out of all the axis
    return separation;
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