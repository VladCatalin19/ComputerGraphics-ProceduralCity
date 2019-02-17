#pragma once

/**
 * This library contains the constants used in the game.
 */
// City constants
// Number of objects per dimension
#define CITY_MAX_HEI        35
#define CITY_MAX_WID        30

// Plane dimensions
#define PLANE_LENGTH        CITY_MAX_HEI + 2
#define PLANE_WIDTH         CITY_MAX_WID + 2

// Road and intersection dimensions
#define ROAD_WID            1.0f
#define INTERSEC_LEN        1.0f
#define INTERSEC_WID        1.0f

// Statue
#define STATUES_NUMBER      4

// Building
#define MAX_LEVELS          5
#define BUILDING_LIGHTS     2
#define DELTA_RADIUS        0.1f

// Probabilities
#define ROOF_PROB           25
#define DECORATION_PROB     5

// Scene constants
// Camera
#define CAMERA_DEPTH        300
#define CAMERA_INIT_POS     glm::vec3(-5.0f, 9.0f, -7.0f)
#define CAMERA_INIT_ROT     glm::vec3(0.0f, RADIANS(195), 0.0f)

// Light
#define LIGHT_POSITION      glm::vec3(-4.50f, 67.00f, 1500.0f)
#define LIGHT_COLOR         glm::vec3(0.75f, 0.77f, 1.0f)

// Meshes heights
#define PLANE_HEIGHT       -0.01f
#define ROAD_HEIGHT         0.00f
#define INTERSEC_HEIGHT     0.001f
#define STATUE_HEIGHT       PLANE_HEIGHT
#define CAR_HEIGHT          0.075f
#define UFO_HEIGHT          25.0f

// Decoration meshes scales
#define STATUE_SCALE        glm::vec3(0.005f,  0.005f,  0.005f)
#define CAR_SCALE           glm::vec3(0.007f,  0.007f,  0.007f)
#define UFO_SCALE           glm::vec3(1.0f,    1.0f,    1.0f)
#define SKYBOX_SCALE        glm::vec3(150.0f,  150.0f,  150.0f)
#define SPOTLIGHT_SCALE     glm::vec3(0.0002f, 0.0002f, 0.0002f)

// Meshes and textures numbers
#define WINDOWS_TEXTURES    4
#define CARS_MESHES         4
#define STATUES_MESHES      2