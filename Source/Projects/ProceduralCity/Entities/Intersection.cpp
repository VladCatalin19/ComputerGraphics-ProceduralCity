#include "Intersection.h"
#include <Core/Engine.h>

using namespace ProceduralCityObjects;
using namespace std;

Intersection::Intersection() {

}

Intersection::Intersection(glm::vec3 position, float width, float length,
                           float rotation)
    : Entity(glm::vec3(position.x, INTERSEC_HEIGHT, position.z)) {

    this->width    = width;
    this->length   = length;
    this->rotation = rotation;
}

Intersection::~Intersection() {

}

float Intersection::GetWidth() {
    return width;
}

float Intersection::GetLength() {
    return length;
}

float Intersection::GetRotation() {
    return rotation;
}
