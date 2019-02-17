#include "MeshGenerator.h"

using namespace ProceduralCityObjects;
using namespace std;

MeshGenerator::MeshGenerator() {}
MeshGenerator::~MeshGenerator() {}


vector<glm::vec3> MeshGenerator::CreatePrismVertices(glm::vec3 position,
    int fragments, float rotation, float radiusX, float radiusY,
    float startHeight, float endHeight, bool buildRoof) {

    glm::vec3 pos = position;
    float increment = 2.0f * (float)(M_PI) / fragments;

    vector<glm::vec3> vertices;
    vertices.reserve(2 * fragments + 2);

    // Divide base circle in fragments number of fragments
    for (int i = 0; i < fragments; ++i) {
        float currAngle = i * increment;
        glm::vec3 point = glm::vec3(radiusX * cos(currAngle),
            startHeight,
            radiusY * sin(currAngle));

        glm::vec3 rot = glm::vec3(0);

        // Rotate and translate point
        rot.x = cos(rotation) * point.x - sin(rotation) * point.z + pos.x;
        rot.y = point.y;
        rot.z = sin(rotation) * point.x + cos(rotation) * point.z + pos.z;

        vertices.push_back(rot);
    }
    // Add the translated center of the base circle
    vertices.push_back(glm::vec3(pos.x, startHeight, pos.z));

    // Divide top circle in fragments number of fragments
    for (int i = 0; i < fragments; ++i) {
        glm::vec3 point = vertices[i];
        vertices.push_back(glm::vec3(point.x, endHeight, point.z));
    }

    //  Add the translated center of the top circle
    glm::vec3 aboveCenter = vertices[fragments];
    if (buildRoof) {
        // Move the center upwards if the building should have a roof
        float prec = (rand() % 25 + 25) / 100.0f;
        float aboveCenterHeight = (endHeight - startHeight) * prec + endHeight;
        vertices.push_back(glm::vec3(aboveCenter.x, aboveCenterHeight, aboveCenter.z));
    }
    else {
        vertices.push_back(glm::vec3(aboveCenter.x, endHeight, aboveCenter.z));
    }

    return vertices;
}

// Algorithm taken from https://www.iquilezles.org/www/articles/normals/normals.htm
vector<glm::vec3> MeshGenerator::CreatePrismNormals(vector<glm::vec3> vertices,
    vector<unsigned short> indices) {

    vector<glm::vec3> normals = vector<glm::vec3>(vertices.size(),
        glm::vec3(0.0f, 0.0f, 0.0f));

    for (int i = 0; i < indices.size(); i += 3) {
        int ia = indices[i];
        int ib = indices[i + 1];
        int ic = indices[i + 2];

        glm::vec3 e1 = vertices[ib] - vertices[ia];
        glm::vec3 e2 = vertices[ic] - vertices[ib];
        glm::vec3 no = glm::cross(e1, e2);

        normals[ia] += no;
        normals[ib] += no;
        normals[ic] += no;
    }

    for (int i = 0; i < normals.size(); ++i) {
        if (normals[i] != glm::vec3(0.0f, 0.0f, 0.0f)) {
            normals[i] = glm::normalize(normals[i]);
        }
    }

    return normals;
}

vector<glm::vec2> MeshGenerator::CreatePrismTextureCoords(int fragments, 
    float startHeight, float endHeight) {

    vector<glm::vec2> textureCoords;
    textureCoords.reserve(2 * fragments + 2);

    glm::vec2 tex;
    for (int j = 1; j >= 0; --j) {

        if (fragments % 2 == 1) {
            // First half + 1 of the texture coords will have the u coordinate
            // from 0 to 1 and the v coordinate 1 for the base polygon and
            // 0 for the top polygon
            tex.x = 0.0f;
            tex.y = (float)(j);
            textureCoords.push_back(tex);

            tex.x = 1.0f / (float)(fragments);
            tex.y = (float)(j);
            textureCoords.push_back(tex);

            for (int i = 2; i < fragments / 2 + 1; ++i) {
                tex.x = 2.0f * (float)(i - 1) / (float)(fragments);
                tex.y = (float)(j);
                textureCoords.push_back(tex);
            }
            // Second half of the texture coords will have u coordinate from
            // 1 to 0 and the v coordinate 1 for the base polygon and 0 for
            // the top polygon
            for (int i = fragments / 2 + 1; i < fragments; ++i) {
                tex.x = 2.0f * (1.0f - (float)(i - 1) / (float)(fragments));
                tex.y = (float)(j);
                textureCoords.push_back(tex);
            }
        }
        else {
            // First half of the texture coords will have the u coordinate
            // from 0 to 1 and the v coordinate 1 for the base polygon and
            // 0 for the top polygon
            for (int i = 0; i < fragments / 2 + 1; ++i) {
                tex.x = 2.0f * (float)(i) / (float)(fragments);
                tex.y = (float)(j);
                textureCoords.push_back(tex);
            }
            // Second half of the texture coords will have u coordinate from
            // 1 to 0 and the v coordinate 1 for the base polygon and 0 for
            // the top polygon
            for (int i = fragments / 2 + 1; i < fragments; ++i) {
                tex.x = 2.0f * (1.0f - (float)(i) / (float)(fragments));
                tex.y = (float)(j);
                textureCoords.push_back(tex);
            }
        }
        textureCoords.push_back(glm::vec2(0.0f, 0.0f));
    }

    return textureCoords;
}

vector<unsigned short> MeshGenerator::CreatePrismIndices(int fragments, int centerIndex, int indexStartVal) {
    vector<unsigned short> indices;
    indices.reserve(6 * fragments + 6);

    // Facades indices
    for (unsigned short i = indexStartVal; i < fragments - 1 + indexStartVal; ++i) {
        indices.push_back(i + 2 + fragments);
        indices.push_back(i + 1);
        indices.push_back(i);

        indices.push_back(i);
        indices.push_back(i + 1 + fragments);
        indices.push_back(i + 2 + fragments);
    }

    // Last facade indices
    indices.push_back(fragments + 1 + indexStartVal);
    indices.push_back(indexStartVal);
    indices.push_back(fragments - 1 + indexStartVal);

    indices.push_back(fragments - 1 + indexStartVal);
    indices.push_back(2 * fragments + indexStartVal);
    indices.push_back(fragments + 1 + indexStartVal);

    // Roof indices
    for (int i = fragments + 1 + indexStartVal; i < 2 * fragments + indexStartVal; ++i) {
        indices.push_back(i);
        indices.push_back(centerIndex);
        indices.push_back(i + 1);
    }

    indices.push_back(2 * fragments + indexStartVal);
    indices.push_back(centerIndex);
    indices.push_back(fragments + 1 + indexStartVal);

    return indices;
}

Mesh* MeshGenerator::CreateBuildingMesh(Building *building) {
    glm::vec3 pos = building->GetPosition();

    vector<glm::vec3> vertices;
    vector<glm::vec3> normals;
    vector<glm::vec2> textureCoords;
    vector<unsigned short> indices;

    float minH = 0.0f;
    float maxH = building->GetHeight();
    float prevH = building->GetHeight();
    float currRadiusX = building->GetRadiusX();
    float currRadiusY = building->GetRadiusY();

    // Create levels prisms and stack them on top of each other.
    for (int i = 0; i < building->GetLevels(); ++i) {
        int indexStartPoint = (int)(vertices.size());

        vector<glm::vec3> verticesAux = CreatePrismVertices(building->GetPosition(),
            building->GetFragments(), building->GetRotation(), currRadiusX, currRadiusY,
            minH, maxH,
            building->HasRoof() && i == building->GetLevels() - 1);
        vertices.reserve(vertices.size() + verticesAux.size());
        vertices.insert(vertices.end(), verticesAux.begin(), verticesAux.end());

        vector<glm::vec2> textureCoordsAux = CreatePrismTextureCoords(building->GetFragments(), minH, maxH);
        textureCoords.reserve(textureCoords.size() + textureCoordsAux.size());
        textureCoords.insert(textureCoords.end(), textureCoordsAux.begin(),
            textureCoordsAux.end());

        vector<unsigned short> indicesAux = CreatePrismIndices(building->GetFragments(),
            (int)(vertices.size()) - 1, indexStartPoint);
        indices.reserve(indices.size() + indicesAux.size());
        indices.insert(indices.end(), indicesAux.begin(), indicesAux.end());

        // Decrease prism radi
        float perc = (rand() % 11 + 5) / 100.0f;
        currRadiusX -= perc * currRadiusX;
        currRadiusY -= perc * currRadiusY;

        // Decrease prism height
        minH = maxH;
        perc = (rand() % 11 + 65) / 100.0f;
        maxH = minH + perc * prevH;
        prevH = maxH - minH;
    }
    normals = CreatePrismNormals(vertices, indices);

    Mesh *mesh = new Mesh("building" + to_string(building->GetId()));
    mesh->InitFromData(vertices, normals, textureCoords, indices);
    mesh->CreateAABB();
    return mesh;
}

vector<Entity::Light> MeshGenerator::CreateLightsBuilding(Building *building, Mesh* mesh) {
    vector<Entity::Light> lights = vector<Entity::Light>(BUILDING_LIGHTS);

    // Choose a random level to place the light and get its height
    int level = rand() % (building->GetLevels() + 1);
    float lightsHeight = mesh->positions[(building->GetFragments() + 1) * level].y;

    glm::vec3 pos = building->GetPosition();
    float increment = 2.0f * (float)(M_PI) / MAX(BUILDING_LIGHTS, 1);

    // Place lights on circle's circumference making sure the distance between
    // two consecutive ones is the same
    for (int i = 0; i < BUILDING_LIGHTS; ++i) {
        float currAngle = i * increment;
        glm::vec3 point = glm::vec3((building->GetRadiusX() + DELTA_RADIUS) * cos(currAngle),
            lightsHeight,
            (building->GetRadiusY() + DELTA_RADIUS) * sin(currAngle));
        glm::vec3 rot = glm::vec3(0);

        // Rotate and translate light's position
        rot.x = cos(building->GetRotation()) * point.x - sin(building->GetRotation()) * point.z + pos.x;
        rot.y = point.y;
        rot.z = sin(building->GetRotation()) * point.x + cos(building->GetRotation()) * point.z + pos.z;

        lights[i].position = rot;

        // Calculate light's mesh position by getting the closest vertex to the
        // building and adding an offset
        int index = i * (building->GetFragments() / MAX(BUILDING_LIGHTS, 1)) + (building->GetFragments() + 1) * level;
        glm::vec3 vertexPos = mesh->positions[index];
        glm::vec3 v = vertexPos - pos;
        v.y = 0.0f;
        float len = glm::length(v);
        v = glm::normalize(v);

        lights[i].meshPosition = vertexPos + v * len * 0.1f;
        lights[i].meshPosition.y += 0.01f;

        int coneAngleDegr = rand() % 21 + 45;
        lights[i].cone_angle = RADIANS(coneAngleDegr);
    }
    ///*
    for (int i = 0; i < BUILDING_LIGHTS; ++i) {
        glm::vec3 dir = pos - lights[i].position;

        if (level == 0) {
            // If light is on the ground, make sure direction is upwards
            float nextLevelHeight = mesh->positions[(building->GetFragments() + 1)].y;
            dir.y = nextLevelHeight / 2.0f;
        }
        else if (level == building->GetLevels()) {
            // If light is on last level, make sure direction is downwards
            float prevLevelHeight = mesh->positions[(building->GetFragments() + 1) * (level - 1)].y;
            dir.y = -(prevLevelHeight + lightsHeight) / 2.0f;
        }
        else {
            // If light is neither on the last level nor on the ground, direction
            // will be randomly chosen to be upwards or downwards
            float prevLevelHeight = mesh->positions[(building->GetFragments() + 1) * (level - 1)].y;
            float nextLevelHeight = mesh->positions[(building->GetFragments() + 1) * (level + 1)].y;
            bool isUp = rand() & 1;

            if (isUp) {
                dir.y = (nextLevelHeight + lightsHeight) / 2.0f;
            }
            else {
                dir.y = -(prevLevelHeight + lightsHeight) / 2.0f;
            }
        }
        lights[i].cone_direction = glm::normalize(dir);

        lights[i].meshRotation.y = atan2(lights[i].cone_direction.x, lights[i].cone_direction.z);

        if (lights[i].cone_direction.y < 0) {
            lights[i].meshRotation.x = RADIANS(0);
        }
        else {
            lights[i].meshRotation.x = RADIANS(180);
        }
    }

    return lights;
}


Mesh* MeshGenerator::CreateRoadMesh(Road *road) {
    glm::vec3 pos = road->GetPosition();

    glm::vec3 offset = glm::vec3();

    vector<glm::vec3> vertices = {
        glm::vec3(0.0f,   0.0f, 0.0f),
        glm::vec3(road->GetLength(), 0.0f, 0.0f),
        glm::vec3(road->GetLength(), 0.0f, road->GetWidth()),
        glm::vec3(0.0f,   0.0f, road->GetWidth())
    };

    // Rotate vertices and translate them to desired position
    for (int i = 0; i < vertices.size(); ++i) {
        glm::vec3 rot = vertices[i];
        rot.x = cos(road->GetRotation()) * vertices[i].x
            - sin(road->GetRotation()) * (vertices[i].z - road->GetWidth() / 2) + pos.x;
        rot.y = vertices[i].y;
        rot.z = sin(road->GetRotation()) * vertices[i].x
            + cos(road->GetRotation()) * (vertices[i].z - road->GetWidth() / 2) + pos.z;

        vertices[i] = rot;
    }

    vector<glm::vec3> normals = {
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    };
    vector<glm::vec2> textureCoords = {
        glm::vec2(0.0f,   0.0f),
        glm::vec2(road->GetLength(), 0.0f),
        glm::vec2(road->GetLength(), 1.0f),
        glm::vec2(0.0f,   1.0f)
    };
    vector<unsigned short> indices = {
        2, 1, 0,
        0, 3, 2
    };

    string str;
    if (road->GetRotation() == RADIANS(90)) {
        str = "_horizontal";
    }
    else {
        str = "_vertical";
    }

    Mesh *mesh = new Mesh("road" + str);
    mesh->InitFromData(vertices, normals, textureCoords, indices);
    return mesh;
}

Mesh* MeshGenerator::CreatePlaneMesh(Plane *plane) {
    glm::vec3 pos = plane->GetPosition();
    float widOffset = plane->GetWidth() / 2.0f;
    float lenOffset = plane->GetLength() / 2.0f;

    vector<glm::vec3> vertices = {
        glm::vec3(pos.x - lenOffset, pos.y, pos.z - widOffset),
        glm::vec3(pos.x + lenOffset, pos.y, pos.z - widOffset),
        glm::vec3(pos.x + lenOffset, pos.y, pos.z + widOffset),
        glm::vec3(pos.x - lenOffset, pos.y, pos.z + widOffset)
    };

    vector<glm::vec3> normals = {
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    };

    vector<glm::vec2> textureCoords = {
        glm::vec2(0.0f,   0.0f),
        glm::vec2(plane->GetLength(), 0.0f),
        glm::vec2(plane->GetLength(), plane->GetWidth()),
        glm::vec2(0.0f,   plane->GetWidth())
    };

    vector<unsigned short> indices = {
        2, 1, 0,
        0, 3, 2
    };

    Mesh *mesh = new Mesh("plane");
    mesh->InitFromData(vertices, normals, textureCoords, indices);
    return mesh;
}

Mesh* MeshGenerator::CreateIntersectionMesh(Intersection *intersec) {
    glm::vec3 pos = intersec->GetPosition();
    float widOffset = intersec->GetWidth() / 2.0f;
    float lenOffset = intersec->GetLength() / 2.0f;

    vector<glm::vec3> vertices = {
        glm::vec3(-widOffset, pos.y, -lenOffset),
        glm::vec3(+widOffset, pos.y, -lenOffset),
        glm::vec3(+widOffset, pos.y, +lenOffset),
        glm::vec3(-widOffset, pos.y, +lenOffset)
    };

    // Rotate vertices and translate them to desired position
    for (int i = 0; i < vertices.size(); ++i) {
        glm::vec3 rot = vertices[i];
        rot.x = cos(intersec->GetRotation()) * vertices[i].x - sin(intersec->GetRotation()) * vertices[i].z + pos.x;
        rot.y = vertices[i].y;
        rot.z = sin(intersec->GetRotation()) * vertices[i].x + cos(intersec->GetRotation()) * vertices[i].z + pos.z;

        vertices[i] = rot;
    }

    vector<glm::vec3> normals = {
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    };

    vector<glm::vec2> textureCoords = {
        glm::vec2(0.0f, 0.0f),
        glm::vec2(0.0f, 1.0f),
        glm::vec2(1.0f, 1.0f),
        glm::vec2(1.0f, 0.0f)
    };

    vector<unsigned short> indices = {
        2, 1, 0,
        0, 3, 2
    };

    Mesh *mesh = new Mesh("intersection");
    mesh->InitFromData(vertices, normals, textureCoords, indices);
    return mesh;
}