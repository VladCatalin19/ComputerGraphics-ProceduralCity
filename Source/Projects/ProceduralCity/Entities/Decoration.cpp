#include "Decoration.h"

using namespace ProceduralCityObjects;
using namespace std;

Decoration::Decoration() {

}

Decoration::Decoration(glm::vec3 position, DecorationType type, float rotation) {
    this->type     = type;
    this->rotation = rotation;

    glm::vec3 pos = position;

    switch (type) {
    case statue:
        pos.y = STATUE_HEIGHT;
        scale = STATUE_SCALE;
        meshNumber = rand() % STATUES_MESHES + 1;
        break;

    case car:
        pos.y = CAR_HEIGHT;
        scale = CAR_SCALE;
        meshNumber = rand() % CARS_MESHES + 1;
        break;

    case ufo:
        pos.y = UFO_HEIGHT;
        scale = UFO_SCALE;
        meshNumber = 1;
        break;
    }

    SetPosition(pos);
}

Decoration::~Decoration() {

}

Decoration::DecorationType Decoration::GetType() {
    return type;
}

float Decoration::GetRotation() {
    return rotation;
}

glm::vec3 Decoration::GetScale() {
    return scale;
}

int Decoration::GetMeshNumber() {
    return meshNumber;
}