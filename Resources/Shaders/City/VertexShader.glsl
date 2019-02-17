#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coords;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Output values to fragment shader 
out vec3 world_position;
out vec3 world_normal;
out vec2 texture_coords;

void main() {
    mat3 model_m3  = mat3(Model);
    world_position = model_m3 * v_position;
	world_normal   = model_m3 * v_normal;

    texture_coords = v_texture_coords;

	gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
