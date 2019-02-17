#include "Road.h"
#include <Core/Engine.h>

using namespace ProceduralCityObjects;
using namespace std;

Road::Road() {

}

Road::Road(glm::vec3 position, float width, float length, float rotation)
    : Entity(glm::vec3(position.x, ROAD_HEIGHT, position.z)) {

    this->width    = width;
    this->length   = length;
    this->rotation = rotation;
}

Road::~Road() {

}

float Road::GetWidth() {
    return width;
}

float Road::GetLength() {
    return length;
}

float Road::GetRotation() {
    return rotation;
}
