#include "Plane.h"
#include <Core/Engine.h>
#include <vector>

using namespace ProceduralCityObjects;
using namespace std;

Plane::Plane() {

}

Plane::Plane(glm::vec3 position, float width, float length)
    : Entity(glm::vec3(position.x, PLANE_HEIGHT, position.z)) {

    this->width  = width;
    this->length = length;
}

Plane::~Plane() {

}

float Plane::GetWidth() {
    return width;
}
float Plane::GetLength() {
    return length;
}
