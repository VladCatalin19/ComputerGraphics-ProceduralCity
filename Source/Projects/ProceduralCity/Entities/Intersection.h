#pragma once

#include "Entity.h"

/**
 * This class implemets the intersection drawn where two raods meet.
 * An intersection is a rectangular mesh with a width and a length.
 * The mesh can be rotated counter-clockwise in xOz plane.
 */
class ProceduralCityObjects::Intersection : public ProceduralCityObjects::Entity {
public:
    Intersection();
    Intersection(glm::vec3 position, float width, float length, float rotation);
    ~Intersection();

    float GetWidth();
    float GetLength();
    float GetRotation();

private:

    float width;
    float length;
    float rotation;
};