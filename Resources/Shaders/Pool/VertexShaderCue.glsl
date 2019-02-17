#version 330

/**
 * This vertex shader is used to calculate the position of the cue model when
 * it is swinging and pass the result to the fragment shader.
 */

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float time;
uniform bool isActive;

out vec3 frag_position;
out vec3 frag_normal;
out vec3 frag_color;
out vec2 tex_coord;

void main() {
    vec3 new_pos;
    if (isActive) {
        new_pos = vec3(v_position.x, v_position.y, v_position.z + (cos(2.0f * time) - 1.0f) / 5.0f);
    } else {
        new_pos = v_position;
    }

    frag_position = new_pos;
    frag_normal = v_normal;
	frag_color = v_color;
	tex_coord = v_texture_coord;
    gl_Position = Projection * View * Model * vec4(new_pos, 1.0);
}
