#pragma once

#include "Entity.h"

/**
 * This class implemets the decorations used in the scenes.
 * Decoration are statues scattered around the city, cars
 * on roads or intersection or a ufo above the city.
 */
class ProceduralCityObjects::Decoration : public ProceduralCityObjects::Entity {
public:
    enum DecorationType { statue, car, ufo };

    Decoration();
    Decoration(glm::vec3 position, DecorationType type, float rotation);
    ~Decoration();

    DecorationType GetType();
    float GetRotation();
    glm::vec3 GetScale();

    int GetMeshNumber();

private:
    DecorationType type;
    float rotation;
    glm::vec3 scale;
    int meshNumber;
};