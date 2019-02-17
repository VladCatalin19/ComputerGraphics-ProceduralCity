#pragma once

#include "Entity.h"

/**
 * This class implemets the road drawn in the scene.
 * A road is a rectangular mesh with a width and a length.
 * The mesh can be rotated counter-clockwise in xOz plane.
 */
class ProceduralCityObjects::Road : public ProceduralCityObjects::Entity {
public:
    Road();
    Road(glm::vec3 position, float width, float length, float rotation);
    ~Road();

    float GetWidth();
    float GetLength();
    float GetRotation();
private:

    float width;
    float length;
    float rotation;
};