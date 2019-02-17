#include "Building.h"
#include <Core/Engine.h>
#include <iostream>

using namespace ProceduralCityObjects;
using namespace std;

Building::Building() {}

Building::Building(glm::vec3 position, float radiusX, float radiusY, float height,
                   float rotation, int levels, int fragments, bool hasRoof, int id)
    : Entity(position) {
    
    this->radiusX   = radiusX;
    this->radiusY   = radiusY;
    this->height    = height;
    this->rotation  = rotation;
    this->levels    = levels;
    this->fragments = fragments;
    this->hasRoof   = hasRoof;
    this->id        = id;

    textureNumber = rand() % WINDOWS_TEXTURES + 1;
}

Building::~Building() {}

int Building::GetTextureNumber() {
    return textureNumber;
}

float Building::GetRadiusX() {
    return radiusX;
}

float Building::GetRadiusY() {
    return radiusY;
}

float Building::GetHeight() {
    return height;
}

float Building::GetRotation() {
    return rotation;
}

int Building::GetLevels() {
    return levels;
}

int Building::GetFragments() {
    return fragments;
}

bool Building::HasRoof() {
    return hasRoof;
}

int Building::GetId() {
    return id;
}

std::vector<Entity::Light> Building::GetLights() {
    return lights;
}

void Building::SetLights(std::vector<Entity::Light> lights) {
    this->lights = lights;
}
