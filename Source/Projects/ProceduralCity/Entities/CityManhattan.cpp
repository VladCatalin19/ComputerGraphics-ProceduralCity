#include "CityManhattan.h"
#include <include/math.h>
#include <algorithm>
#include <iostream>

using namespace ProceduralCityObjects;
using namespace std;

CityManhattan::CityManhattan() {

}

CityManhattan::CityManhattan(glm::vec3 position, int width, int height) 
    : Entity(position) {

    this->width  = width;
    this->height = height;

    buildingId  = 0;
    buildings   = vector<Building*>();
    decorations = vector<Decoration*>();

    cells = vector<vector<CellObject>>(height,
                                       vector<CellObject>(width,
                                                          CellObject::nothing));

    buildsAndDecosMatrix = vector<vector<Cell>>(height,
                                                vector<Cell>(width, Cell()));

    horizontalRoadsIndices = vector<int>();
    verticalRoadsIndices   = vector<int>();

    horizontalRoadsPositions = vector<glm::vec3>();
    verticalRoadsPositions   = vector<glm::vec3>();
    intersectionsPositions   = vector<glm::vec3>();

    CreateRoadsAndIntersections();
    CreateBuildings();
    CreateDecorations();

    horizontalRoad = new Road(glm::vec3(0.0f, 0.0f, 0.0f), ROAD_WID,
                              (float)(width),  RADIANS(90));
    verticalRoad   = new Road(glm::vec3(0.0f, 0.0f, 0.0f), ROAD_WID,
                              (float)(height), RADIANS(0));

    intersection   = new Intersection(glm::vec3(0.0f, 0.0f, 0.0f),
                                      INTERSEC_WID, INTERSEC_LEN, RADIANS(0));
    
    plane          = new Plane(position, PLANE_WIDTH, PLANE_LENGTH);
}

CityManhattan::~CityManhattan() {
    delete horizontalRoad;
    delete verticalRoad;
    delete intersection;
    delete plane;

    for (int i = 0; i < buildings.size(); ++i) {
        delete buildings[i];
    }

    for (int i = 0; i < decorations.size(); ++i) {
        delete decorations[i];
    }

    for (int i = 0; i < height; ++i) {
        cells[i].clear();
        buildsAndDecosMatrix.clear();
    }

    cells.clear();
    buildsAndDecosMatrix.clear();
    buildings.clear();
    decorations.clear();

    horizontalRoadsIndices.clear();
    verticalRoadsIndices.clear();
    horizontalRoadsPositions.clear();
    verticalRoadsPositions.clear();
    intersectionsPositions.clear();
}

void CityManhattan::CreateRoadsAndIntersections() {
    // Create a random number of horizontal roads
    int randRoads = MAX(height / 10, 1);
    int partsHeight = rand() % randRoads + randRoads;
    horizontalRoadsPositions.reserve(partsHeight);
    horizontalRoadsIndices.reserve(partsHeight);

    for (int i = 0; i < partsHeight; ++i) {
        int roadHeight = rand() % (height / partsHeight - 1) + i * height / partsHeight + 1;

        horizontalRoadsPositions.push_back(CalculatePositionFromMatrix(roadHeight, 0));
        horizontalRoadsIndices.push_back(roadHeight);

        // Mark roads in matrix
        for (int j = 0; j < width; ++j) {
            cells[roadHeight][j] = CellObject::road;
        }
    }

    // Create a random number of vertical roads
    randRoads = MAX(width / 10, 1);
    int partsWidth = rand() % randRoads + randRoads;
    verticalRoadsPositions.reserve(partsWidth);
    verticalRoadsIndices.reserve(partsWidth);

    for (int i = 0; i < partsWidth; ++i) {
        int roadWidth = rand() % (width / partsWidth - 1) + i * width / partsWidth + 1;

        verticalRoadsPositions.push_back(CalculatePositionFromMatrix(0, roadWidth));
        verticalRoadsIndices.push_back(roadWidth);

        // Mark roads in matrix
        for (int j = 0; j < height; ++j) {
            cells[j][roadWidth] = CellObject::road;
        }

        // Create intersections and mark them in matrix
        for (int j = 0; j < horizontalRoadsPositions.size(); ++j) {
            intersectionsPositions.push_back(
                CalculatePositionFromMatrix(horizontalRoadsIndices[j], roadWidth)
            );

            cells[horizontalRoadsIndices[j]][roadWidth] = CellObject::intersec;
        }
    }
}

void CityManhattan::CreateStatue(int x, int y) {
    glm::vec3 position = CalculatePositionFromMatrix(x, y);

    // Neighbour points in cells matrix
    vector<pair<int, int>> points = {
        pair<int, int>(MAX(x - 1, 0),          y),
        pair<int, int>(x,                      MIN(y + 1, width - 1)),
        pair<int, int>(x,                      MAX(y - 1, 0)),
        pair<int, int>(MIN(x + 1, height - 1), y)
    };

    pair<float, float> center = pair<float, float>(0.0f, 0.0f);
    int roadsNumber = 0;

    // Calculate center of weight of road neighbours
    for (int i = 0; i < points.size(); ++i) {
        if (cells[points[i].first][points[i].second] == CellObject::road) {
            center.first += points[i].first;
            center.second += points[i].second;
            ++roadsNumber;
        }
    }

    // Rotate statue so it always faces towards neighbour roads
    float rotation;
    if (roadsNumber == 0) {
        rotation = 0.0f;
    } else {
        center.first  /= (float)(roadsNumber);
        center.second /= (float)(roadsNumber);
        rotation = atan2(center.first - x, center.second - y);
    
    }

    Decoration *d = new Decoration(position, Decoration::statue, rotation);
    decorations.push_back(d);
    buildsAndDecosMatrix[x][y] = d;
}

void CityManhattan::CreateBuilding(int x, int y) {
    glm::vec3 position = CalculatePositionFromMatrix(x, y);

    // Create pseudo-random parameters of building
    float radiusX = 0.35f + (rand() % 20 - 10) / 100.0f;
    float radiusY = 0.35f + (rand() % 20 - 10) / 100.0f;
    float height = (rand() % 20 + 10) / 10.0f;
    int rnd = rand() % 180;
    float rotation = RADIANS(rnd);
    int levels = rand() % MAX_LEVELS + 1;
    int fragments = rand() % 15 + 4;
    if (fragments >= 12) {
        fragments = 16;
    }
    bool hasRoof = (rand() % 100) < ROOF_PROB;

    Building *b = new Building(position, radiusX, radiusY, height, rotation,
                               levels, fragments, hasRoof, buildingId++);
    buildings.push_back(b);
    buildsAndDecosMatrix[x][y] = b;
}

void CityManhattan::CreateBuildings() {
    // For each unoccupied cell choose whether to build a building or a statue
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (cells[i][j] == CellObject::nothing) {
                int prob = rand() % 100;
                if (prob < DECORATION_PROB) {
                    CreateStatue(i, j);
                }
                else {
                    CreateBuilding(i, j);
                }
            }
        }
    }
}

bool CityManhattan::CarsNearby(int x, int y) {
    vector<pair<int,int>> points = {
        pair<int, int>(MAX(x - 1, 0),          y),
        pair<int, int>(x,                      MIN(y + 1, width - 1)),
        pair<int, int>(x,                      y),
        pair<int, int>(x,                      MAX(y - 1, 0)),
        pair<int, int>(MIN(x + 1, height - 1), y)
    };

    for (int i = 0; i < points.size(); ++i) {
        if (cells[points[i].first][points[i].second] == car) {
            return true;
        }
    }
    return false;
}

void CityManhattan::CreateDecorations() {
    // Create random number of cars
    int carsNumber = rand() % 25 + 25;
    decorations.reserve(decorations.size() + carsNumber);

    // Place half of the cars on the horizontal roads
    for (int i = 0; i < carsNumber / 2; ++i) {
        int h = 0;
        int w = 0;
        int rnd;
        glm::vec3 position;
        float rotation;

        // Generate random coordinates until there are no cars in the
        // neighbour cells
        do {
            h = horizontalRoadsIndices[rand() % horizontalRoadsIndices.size()];
            w = rand() % this->width;
        } while (CarsNearby(h, w));

        cells[h][w] = CellObject::car;

        position = CalculatePositionFromMatrix(h, w);
        rnd = (rand() % 2) * 180 + 90;
        // Rotate car so it is facing either positive or negative z
        rotation = RADIANS(rnd);

        decorations.push_back(new Decoration(position, Decoration::car, rotation));
    }

    // Place the other half of the cars on the vertical roads
    for (int i = carsNumber / 2 + 1; i < carsNumber; ++i) {
        int h = 0;
        int w = 0;
        int rnd;
        glm::vec3 position;
        float rotation;

        // Generate random coordinates until there are no cars in the
        // neighbour cells
        do {
            h = rand() % this->height;
            w = verticalRoadsIndices[rand() % verticalRoadsIndices.size()];
        } while (CarsNearby(h, w));

        cells[h][w] = CellObject::car;

        position = CalculatePositionFromMatrix(h, w);
        rnd = (rand() % 2) * 180;
        // Rotate car so it is facing either positive or negative x
        rotation = RADIANS(rnd);

        decorations.push_back(new Decoration(position, Decoration::car, rotation));
    }

    decorations.push_back(new Decoration(glm::vec3(0.0f, UFO_HEIGHT, 0.0f), Decoration::ufo, RADIANS(0)));
}

glm::vec3 CityManhattan::CalculatePositionFromMatrix(int height, int width) {
    glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
    pos.x = (float)(height - this->height / 2);
    pos.z = (float)(width  - this->width  / 2);

    return pos;
}

vector<Decoration*> CityManhattan::GetDecorations() {
    return decorations;
}

vector<Building*> CityManhattan::GetBuildings() {
    return buildings;
}

Road* CityManhattan::GetHorizontalRoad() {
    return horizontalRoad;
}

Road* CityManhattan::GetVerticalRoad() {
    return verticalRoad;
}

Intersection* CityManhattan::GetIntersection() {
    return intersection;
}

Plane* CityManhattan::GetPlane() {
    return plane;
}

vector<glm::vec3> CityManhattan::GetHorizontalRoadsPositions() {
    return horizontalRoadsPositions;
}

vector<glm::vec3> CityManhattan::GetVerticalRoadsPositions() {
    return verticalRoadsPositions;
}

vector<glm::vec3> CityManhattan::GetIntersectionsPositions() {
    return intersectionsPositions;
}

vector<vector<CityManhattan::Cell>> CityManhattan::GetBuildsAndDecosMatrix() {
    return buildsAndDecosMatrix;
}

vector<int> CityManhattan::GetHorizontalRoadsIndices() {
    return horizontalRoadsIndices;
}

vector<int> CityManhattan::GetVerticalRoadsIndices() {
    return verticalRoadsIndices;
}
