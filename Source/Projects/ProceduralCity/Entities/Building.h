#pragma once

#include "Entity.h"
#include <vector>

/**
 * This class implements the buildings used in the scene. A building is
 * a 3D object. It is made by stacking prisms of different heights and radi.
 * The base prism always has the longest radi and height. All prisms have the
 * same base and top geometry. The building can also have a cone rooftop.
 * The mesh will be built in world space with desired dimensions and rotation.
 */
class ProceduralCityObjects::Building : public ProceduralCityObjects::Entity {
public:
    Building();
    Building(glm::vec3 position, float radiusX, float radiusY, float height,
             float rotation, int levels, int fragments, bool hasRoof, int id);
    ~Building();

    int GetTextureNumber();

    float GetRadiusX();
    float GetRadiusY();
    float GetHeight();
    float GetRotation();
    int GetLevels();
    int GetFragments();
    bool HasRoof();
    int GetId();

    std::vector<Entity::Light> GetLights();
    void SetLights(std::vector<Entity::Light> lights);

private:
    float radiusX;
    float radiusY;
    float height;
    float rotation;
    int levels;
    int fragments;
    bool hasRoof;
    int id;

    int textureNumber;
    std::vector<Entity::Light> lights;
};