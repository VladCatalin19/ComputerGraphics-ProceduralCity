#pragma once

#include "Entity.h"
#include "Building.h"
#include "Road.h"
#include "Intersection.h"
#include "Plane.h"
#include "Decoration.h"
#include <vector>

/**
 * This class implemets the city used in the scene. A city has vertical and
 * horizontal roads, intersections, buildings and decorations. The city is
 * generated procedurally with a random number of roads and intersections.
 * The buildings are also generated procedurally, each building having
 * different number of levels and facades. 
 */
class ProceduralCityObjects::CityManhattan : public ProceduralCityObjects::Entity{
public:
    CityManhattan();
    CityManhattan(glm::vec3 position, int width, int height);
    ~CityManhattan();

    std::vector<Decoration*> GetDecorations();
    std::vector<Building*> GetBuildings();

    Road* GetHorizontalRoad();
    Road* GetVerticalRoad();
    Intersection* GetIntersection();
    Plane* GetPlane();

    std::vector<int> GetHorizontalRoadsIndices();
    std::vector<glm::vec3> GetHorizontalRoadsPositions();
    std::vector<int> GetVerticalRoadsIndices();
    std::vector<glm::vec3> GetVerticalRoadsPositions();
    std::vector<glm::vec3> GetIntersectionsPositions();

    struct Cell {
        Building *building;
        Decoration *decoration;

        Cell() : building(nullptr), decoration(nullptr) {}
        Cell(Building *building) : building(building), decoration(nullptr) {}
        Cell(Decoration *decoration) : building(nullptr), decoration(decoration) {}
    };

    std::vector<std::vector<Cell>> GetBuildsAndDecosMatrix();

private:
    void CreateRoadsAndIntersections();
    void CreateStatue(int x, int y);
    void CreateBuilding(int x, int y);
    void CreateBuildings();
    void CreateDecorations();

    bool CarsNearby(int x, int y);

    glm::vec3 CalculatePositionFromMatrix(int height, int width);

    int width;
    int height;

    enum CellObject {nothing, road, intersec, building, statue, car};

    // Matrix with all elements in the city
    std::vector<std::vector<CellObject>> cells;

    int buildingId;             // keep track of last created building's id
    std::vector<Building*> buildings;
    std::vector<Decoration*> decorations;
    Road *horizontalRoad;
    Road *verticalRoad;
    Intersection *intersection;
    Plane *plane;

    // Position of each horizontal road in world space
    std::vector<glm::vec3> horizontalRoadsPositions;
    // Indices of each horizontal road in cells matrix
    std::vector<int>       horizontalRoadsIndices;

    // Position of each vertical road in world space
    std::vector<glm::vec3> verticalRoadsPositions;
    // Indices of each vertical road in cells matrix
    std::vector<int>       verticalRoadsIndices;

    std::vector<glm::vec3> intersectionsPositions;

    std::vector<std::vector<Cell>> buildsAndDecosMatrix;
};