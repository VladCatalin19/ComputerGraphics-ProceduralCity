#pragma once

#include "Entity.h"

/**
 * This class implemets the plane drawn at the very bottom of the scene.
 * A plane is a rectangular mesh with a width and a length.
 */
class ProceduralCityObjects::Plane : public ProceduralCityObjects::Entity {
public:
    Plane();
    Plane(glm::vec3 position, float width, float length);
    ~Plane();

    float GetWidth();
    float GetLength();

private:

    float width;
    float length;
};