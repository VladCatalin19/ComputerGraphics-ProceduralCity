#pragma once

#include <Core/Engine.h>
#include <Projects/ProceduralCity/Constants.h>
#include <Projects/ProceduralCity/ProceduralCityNamespace.h>
#include <vector>

#include <Projects/ProceduralCity/Entities/Entity.h>
#include <Projects/ProceduralCity/Entities/Building.h>
#include <Projects/ProceduralCity/Entities/Road.h>
#include <Projects/ProceduralCity/Entities/Plane.h>
#include <Projects/ProceduralCity/Entities/Intersection.h>

/**
 * This class implemets the entity of the scenes. An entity is any object from
 * the scenes, it provides a position and a structure containing information
 * sbout a light source.
 */
class ProceduralCityObjects::MeshGenerator {
public:
    MeshGenerator();
    ~MeshGenerator();

    std::vector<glm::vec3> CreatePrismVertices(glm::vec3 position,
                                               int fragments, float rotation,
                                               float radiusX, float radiusY,
                                               float startHeight, float endHeight,
                                               bool buildRoof);
    std::vector<glm::vec3> CreatePrismNormals(std::vector<glm::vec3> vertices,
                                              std::vector<unsigned short> indices);
    std::vector<glm::vec2> CreatePrismTextureCoords(int fragments, float startHeight,
                                                    float endHeight);
    std::vector<unsigned short> CreatePrismIndices(int fragmnents, int centerIndex,
                                                   int indexStartVal);

    std::vector<Entity::Light> MeshGenerator::CreateLightsBuilding(Building *building,
                                                                   Mesh* mesh);

    Mesh* CreateBuildingMesh(Building *building);

    Mesh* CreateRoadMesh(Road *road);

    Mesh* CreatePlaneMesh(Plane *plane);
    Mesh* CreateIntersectionMesh(Intersection *intersec);
};