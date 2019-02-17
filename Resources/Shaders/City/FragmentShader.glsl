#version 330

// Input from vertex shader
in vec3 world_position;
in vec3 world_normal;
in vec2 texture_coords;

uniform vec3 light_position;
uniform vec3 eye_position;

// Struct containing information about spotligths illuminating the fragment
#define MAX_LIGHTS 10
uniform int num_lights;
uniform struct SpotLight {
   vec3 position;
   float cone_angle;
   vec3 cone_direction;
} spot_lights[MAX_LIGHTS];

// Light and material properties
uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;
uniform vec3 light_color;

uniform sampler2D texture_1;

layout(location = 0) out vec4 out_color;

// Calculate light from a spotlight
float calculate_light(SpotLight light, vec3 N, vec3 L, vec3 V, vec3 H) {
    float ambient_light = 0.0f;
    float diffuse_light;
    float specular_light;
    float result_light;
    
    float cut_off = light.cone_angle;
    float spot_light = dot(- L, light.cone_direction);
    float spot_light_limit = cos(cut_off);

    if (spot_light > spot_light_limit) {
        // Fragment is illuminated by the spotlight
        diffuse_light  = material_kd * max(dot(L, N), 0) * 8;
        specular_light = 0.0f;
        float attenuation = pow((spot_light - spot_light_limit) / (1.0f - spot_light_limit), 2);
        result_light = ambient_light + attenuation * (diffuse_light + specular_light);
    } else {
        // Fragment is not illuminated by the spotlight
        result_light = ambient_light;
    }

    return result_light;
}

void main() {
	vec3 N = normalize(world_normal);
	vec3 L = normalize(light_position - world_position);
	vec3 V = normalize(eye_position - world_position);
	vec3 H = normalize(L + V);

	float ambient_light = 0.5f;
	float diffuse_light;
	float specular_light;
	float light;

    // Calculate directional light
	diffuse_light  = material_kd * max(dot(L, N), 0);
    specular_light = 0.0f;

	light = ambient_light + diffuse_light + specular_light;

    // Add spotlights light
    for (int i = 0; i < num_lights; ++i) {
        L = normalize(spot_lights[i].position - world_position);
        H = normalize(L + V);
        light += calculate_light(spot_lights[i], N, L, V, H);
    }

    // Apply light to textured fragment
    vec3 color = texture2D(texture_1, texture_coords).xyz * light * light_color;
	out_color = vec4(color, 1.0f);
}