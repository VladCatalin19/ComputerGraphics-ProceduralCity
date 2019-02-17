#pragma once

#include <Core/World.h>
#include <Projects/ProceduralCity/Constants.h>
#include <Projects/ProceduralCity/ProceduralCityNamespace.h>

/**
 * This class implemets the entity of the scenes. An entity is any object from
 * the scenes, it provides a position and a structure containing information
 * sbout a light source.
 */
class ProceduralCityObjects::Entity {
public:
    Entity();
    Entity(glm::vec3 position);
    ~Entity();

    struct Light {
        glm::vec3 position;
        glm::vec3 meshPosition;
        glm::vec3 meshRotation;
        float cone_angle;
        glm::vec3 cone_direction;
    };

    glm::vec3 GetPosition();
    void SetPosition(glm::vec3 position);

private:
    glm::vec3 position;
};