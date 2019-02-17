#include <Core/Engine.h>

#include "ProceduralCity.h"
#include <Projects/ProceduralCity/Entities/Building.h>
#include <Projects/ProceduralCity/Entities/Road.h>
#include <Projects/ProceduralCity/Entities/Intersection.h>
#include <Projects/ProceduralCity/Entities/Plane.h>

#include <Projects/ProceduralCity/Entities/CityManhattan.h>

#include <iostream>
#include <limits>

using namespace ProceduralCityObjects;
using namespace std;

ProceduralCity::ProceduralCity() {
    cityManhattan = NULL;
}

ProceduralCity::~ProceduralCity() {
    delete cityManhattan;

    for (auto it = textures.begin(); it != textures.end(); ++it) {
        delete it->second;
    }
}

void ProceduralCity::Init() {
    cityManhattan = new CityManhattan(glm::vec3(0.0f, 0.0f, 0.0f), CITY_MAX_WID, CITY_MAX_HEI);
    
    InitCamera();
    GenerateMeshes();
    GenerateShaders();
    GenerateTextures();

    InitGlParameters();
}

void ProceduralCity::InitCamera() {
    GetSceneCamera()->SetPerspective(60, window->props.aspectRatio, 0.01f, 300);
    GetSceneCamera()->SetPositionAndRotation(CAMERA_INIT_POS, CAMERA_INIT_ROT);
}

void ProceduralCity::GenerateMeshes() {
    // Load models from files
    Mesh *mesh = new Mesh("skybox");
    mesh->LoadMesh(RESOURCE_PATH::MODELS + "Props", "skybox.obj");
    meshes[mesh->GetMeshID()] = mesh;

    mesh = new Mesh("spotlight");
    mesh->LoadMesh(RESOURCE_PATH::MODELS + "Props", "spotlight.obj");
    meshes[mesh->GetMeshID()] = mesh;

    Mesh *statue1 = new Mesh("statue1");
    statue1->LoadMesh(RESOURCE_PATH::MODELS + "Characters/Catgirl", "catgirl.obj");
    statue1->CreateAABB();
    statue1->boundingBox.scale(STATUE_SCALE);
    meshes[statue1->GetMeshID()] = statue1;
    statuesMeshes.push_back(statue1);

    Mesh *statue2 = new Mesh("statue2");
    statue2->LoadMesh(RESOURCE_PATH::MODELS + "Characters/ChibiMiku", "chibimiku.obj");
    statue2->CreateAABB();
    statue2->boundingBox.scale(STATUE_SCALE);
    meshes[statue2->GetMeshID()] = statue2;
    statuesMeshes.push_back(statue2);

    for (int i = 1; i <= CARS_MESHES; ++i) {
        string name = "car" + to_string(i);
        mesh = new Mesh(name);
        mesh->LoadMesh(RESOURCE_PATH::MODELS + "Props/Cars", name + ".obj");
        mesh->CreateAABB();
        meshes[mesh->GetMeshID()] = mesh;
    }

    mesh = new Mesh("ufo");
    mesh->LoadMesh(RESOURCE_PATH::MODELS + "Props", "ufo.obj");
    mesh->CreateAABB();
    meshes[mesh->GetMeshID()] = mesh;

    // Create city meshes
    MeshGenerator mg;
    mesh = mg.CreatePlaneMesh(cityManhattan->GetPlane());
    meshes[mesh->GetMeshID()] = mesh;

    mesh = mg.CreateRoadMesh(cityManhattan->GetHorizontalRoad());
    meshes[mesh->GetMeshID()] = mesh;

    mesh = mg.CreateRoadMesh(cityManhattan->GetVerticalRoad());
    meshes[mesh->GetMeshID()] = mesh;

    mesh = mg.CreateIntersectionMesh(cityManhattan->GetIntersection());
    meshes[mesh->GetMeshID()] = mesh;

    buildingMeshes.reserve(cityManhattan->GetBuildings().size());

    meshMatrix = vector<vector<Mesh*>>(CITY_MAX_HEI,
                                       vector<Mesh*>(CITY_MAX_WID,
                                                     nullptr));
    for (int i = 0; i < CITY_MAX_HEI; ++i) {
        for (int j = 0; j < CITY_MAX_WID; ++j) {
            CityManhattan::Cell c = cityManhattan->GetBuildsAndDecosMatrix()[i][j];
            if (c.building) {
                Mesh *m = mg.CreateBuildingMesh(c.building);
                buildingMeshes.push_back(m);
                c.building->SetLights(mg.CreateLightsBuilding(c.building, m));

                meshMatrix[i][j] = m;
            } 
            if (c.decoration) {
                meshMatrix[i][j] = meshes["statue" + to_string(c.decoration->GetMeshNumber())];
            }
        }
    }
    /*
    for (Building *b : cityManhattan->GetBuildings()) {
        Mesh *m = mg.CreateBuildingMesh(b);
        buildingMeshes.push_back(m);
        b->SetLights(mg.CreateLightsBuilding(b, m));
    }*/
}

void ProceduralCity::GenerateShaders() {
    // Create a shader program for drawing face polygon with the color
    // and lighting of the object
    Shader *shader = new Shader("Texture");
    shader->AddShader(RESOURCE_PATH::SHADERS + "City/VertexShader.glsl",
                      GL_VERTEX_SHADER);
    shader->AddShader(RESOURCE_PATH::SHADERS + "City/FragmentShader.glsl",
                      GL_FRAGMENT_SHADER);
    shader->CreateAndLink();
    shaders[shader->GetName()] = shader;

    lightPosition = LIGHT_POSITION;
    materialShininess = 10;
    materialKd = 0.5f;
    materialKs = 0.5f;
    lightColor = LIGHT_COLOR;
}

void ProceduralCity::GenerateTextures() {
    Texture2D* texture;
    for (int i = 1; i <= WINDOWS_TEXTURES; ++i) {
        texture = new Texture2D();
        string textureName = "windows" + to_string(i);
        texture->Load2D((RESOURCE_PATH::TEXTURES + textureName + ".jpg").c_str(), GL_REPEAT);
        textures[textureName] = texture;
    }

    texture = new Texture2D();
    texture->Load2D((RESOURCE_PATH::TEXTURES + "road.jpg").c_str(), GL_REPEAT);
    textures["road"] = texture;

    texture = new Texture2D();
    texture->Load2D((RESOURCE_PATH::TEXTURES + "intersection.jpg").c_str(), GL_REPEAT);
    textures["intersection"] = texture;

    texture = new Texture2D();
    texture->Load2D((RESOURCE_PATH::TEXTURES + "plane.jpg").c_str(), GL_REPEAT);
    textures["plane"] = texture;
}

void ProceduralCity::InitGlParameters() {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}

void ProceduralCity::FrameStart() {
    glClearColor(0.165f, 0.200f, 0.541f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}

void ProceduralCity::Update(float deltaTimeSeconds) {
    DrawObjects();
}

void ProceduralCity::DrawObjects() {
    glm::mat4 modelMatrix = glm::mat4(1);

    // Draw horizontal roads
    for (glm::vec3 pos : cityManhattan->GetHorizontalRoadsPositions()) {
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, pos);
        RenderMeshTexture(meshes["road_horizontal"], shaders["Texture"],
                          modelMatrix, textures["road"]);
    }

    // Draw vertical roads
    for (glm::vec3 pos : cityManhattan->GetVerticalRoadsPositions()) {
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, pos);
        RenderMeshTexture(meshes["road_vertical"], shaders["Texture"],
                          modelMatrix, textures["road"]);
    }

    // Draw intersections
    for (glm::vec3 pos : cityManhattan->GetIntersectionsPositions()) {
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, pos);
        RenderMeshTexture(meshes["intersection"], shaders["Texture"],
                          modelMatrix, textures["intersection"]);
    }

    // Draw buildings and their spotlights
    modelMatrix = glm::mat4(1);
    int i = 0;
    for (Building *b : cityManhattan->GetBuildings()) {
        string textureName = "windows" +  to_string(b->GetTextureNumber());
        RenderMeshTexture(buildingMeshes[i++], shaders["Texture"], modelMatrix,
                          textures[textureName], b->GetLights());

        for (Entity::Light l : b->GetLights()) {
            glm::mat4 modelMatrix;
            modelMatrix = glm::translate(glm::mat4(1), l.meshPosition);
            modelMatrix = glm::scale(modelMatrix, SPOTLIGHT_SCALE);
            modelMatrix = glm::rotate(modelMatrix, l.meshRotation.x,
                                      glm::vec3(1.0f, 0.0f, 0.0f));

            RenderMesh(meshes["spotlight"], shaders["Simple"], modelMatrix);
        }
    }


    // Draw statues, cars and the ufo above the city
    for (Decoration *d : cityManhattan->GetDecorations()) {
        modelMatrix = glm::translate(glm::mat4(1), d->GetPosition());
        modelMatrix = glm::scale(modelMatrix, d->GetScale());
        modelMatrix = glm::rotate(modelMatrix, d->GetRotation(),
                                  glm::vec3(0.0f, 1.0f, 0.0f));

        string meshName = "";
        Decoration::DecorationType type = d->GetType();
        switch (type) {
        case Decoration::DecorationType::statue:
            meshName = "statue" + to_string(d->GetMeshNumber());
            break;

        case Decoration::DecorationType::car:
            meshName = "car" + to_string(d->GetMeshNumber());
            break;

        case Decoration::DecorationType::ufo:
            meshName = "ufo";
            break;
        }

        RenderMesh(meshes[meshName], shaders["Texture"], modelMatrix);
    }

    modelMatrix = glm::mat4(1);
    RenderMeshTexture(meshes["plane"], shaders["Texture"], modelMatrix, textures["plane"]);

    modelMatrix = glm::scale(glm::mat4(1), SKYBOX_SCALE);
    RenderMesh(meshes["skybox"], shaders["Simple"], modelMatrix);
}

void ProceduralCity::RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix) {
    if (!mesh || !shader || !shader->program) return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE,
                       glm::value_ptr(GetSceneCamera()->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE,
                       glm::value_ptr(GetSceneCamera()->GetProjectionMatrix()));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE,
                       glm::value_ptr(modelMatrix));
    mesh->Render();
}

void ProceduralCity::RenderMeshTexture(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix,
                              Texture2D* texture, vector<Entity::Light> spotLights) {

    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Vertex shader uniforms
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    GLint loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    GLint loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Fragment shader uniforms
    GLint light_position = glGetUniformLocation(shader->program, "light_position");
    glUniform3f(light_position, lightPosition.x, lightPosition.y, lightPosition.z);

    glm::vec3 cameraPoaition = GetSceneCamera()->transform->GetWorldPosition();
    GLint camera_position = glGetUniformLocation(shader->program, "eye_position");
    glUniform3f(camera_position, cameraPoaition.x, cameraPoaition.y, cameraPoaition.z);

    GLint building_lights = glGetUniformLocation(shader->program, "num_lights");
    glUniform1i(building_lights, (int)spotLights.size());

    // Bind spot lights
    for (int i = 0; i < spotLights.size(); ++i) {
        string elem_name = "spot_lights[" + to_string(i) + "]";
        string position_name    = elem_name + ".position";
        string color_name       = elem_name + ".color";
        string angle_name       = elem_name + ".cone_angle";
        string direction_name   = elem_name + ".cone_direction";

        GLint position = glGetUniformLocation(shader->program, position_name.c_str());
        glUniform3f(position, spotLights[i].position.x, spotLights[i].position.y,
                    spotLights[i].position.z);

        GLint cone_angle = glGetUniformLocation(shader->program, angle_name.c_str());
        glUniform1f(cone_angle, spotLights[i].cone_angle);

        GLint cone_direction = glGetUniformLocation(shader->program, direction_name.c_str());
        glUniform3f(cone_direction, spotLights[i].cone_direction.x, spotLights[i].cone_direction.y,
                    spotLights[i].cone_direction.z);
    }

    GLint material_kd = glGetUniformLocation(shader->program, "material_kd");
    glUniform1f(material_kd, materialKd);

    GLint material_ks = glGetUniformLocation(shader->program, "material_ks");
    glUniform1f(material_ks, materialKs);

    GLint material_shininess = glGetUniformLocation(shader->program, "material_shininess");
    glUniform1i(material_shininess, materialShininess);

    GLint light_color = glGetUniformLocation(shader->program, "light_color");
    glUniform3f(light_color, lightColor.r, lightColor.g, lightColor.b);


    if (texture) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
        GLint location = glGetUniformLocation(shader->program, "texture_1");
        glUniform1i(location, 0);
    }

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()),
                   GL_UNSIGNED_SHORT, 0);
}

void ProceduralCity::FrameEnd() {
    
}


void ProceduralCity::OnInputUpdate(float deltaTime, int mods) {

}


void ProceduralCity::OnKeyPress(int key, int mods) {

}

void ProceduralCity::OnKeyRelease(int key, int mods) {

}

void ProceduralCity::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {

}

void ProceduralCity::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {

}

void ProceduralCity::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {

}

void ProceduralCity::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {

}