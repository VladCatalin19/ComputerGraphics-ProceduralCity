#pragma once

#include <Component/SimpleScene.h>
#include <Component/Transform/Transform.h>
#include <Core/GPU/Mesh.h>
#include "MeshGenerator/MeshGenerator.h"
#include "Constants.h"
#include "ProceduralCityNamespace.h"
#include <iostream>
#include "Entities/Entity.h"
#include "DataStructures/QuadTree.h"

using namespace ProceduralCityObjects;

class ProceduralCity : public SimpleScene {
public:
    ProceduralCity();
    ~ProceduralCity();
    void Init() override;

private:
    //Light & material properties
    glm::vec3 lightPosition;
    float materialKs;
    float materialKd;
    unsigned int materialShininess;
    glm::vec3 lightColor;

    std::unordered_map<std::string, Texture2D*> textures;
    std::vector<Mesh*> statuesMeshes;
    std::vector<Mesh*> buildingMeshes;

    std::vector<std::vector<Mesh*>> meshMatrix;

    // Scene objects
    CityManhattan *cityManhattan;

    QuadTree<Mesh*> tree;

    void InitCamera();
    void GenerateMeshes();
    void GenerateShaders();
    void GenerateTextures();

    QuadTree<Mesh*>* GenerateInitialNode(Mesh* meshes[4]);
    QuadTree<Mesh*>* GenerateNode(QuadTree<Mesh*>* trees[4]);
    void GenerateQuadTree();

    void InitGlParameters();
    void FrameStart() override;
    void Update(float deltaTimeSeconds) override;
    void FrameEnd() override;

    void DrawObjects();
    void RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix) override;
    void RenderMeshTexture(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix,
                           Texture2D* texture, std::vector<ProceduralCityObjects::Entity::Light> spotLights
                                             = std::vector<ProceduralCityObjects::Entity::Light>());

    void OnInputUpdate(float deltaTime, int mods) override;
    void OnKeyPress(int key, int mods) override;
    void OnKeyRelease(int key, int mods) override;
    void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
    void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
    void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
    void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
};