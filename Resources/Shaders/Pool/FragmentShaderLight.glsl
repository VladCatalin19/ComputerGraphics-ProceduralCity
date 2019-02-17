#version 330

/**
 * This fragment shader will take the color from the vertex shader and will
 * pass it out.
 */

in vec3 color;

layout(location = 0) out vec4 out_color;

void main() {
	out_color = vec4(color, 1);
}