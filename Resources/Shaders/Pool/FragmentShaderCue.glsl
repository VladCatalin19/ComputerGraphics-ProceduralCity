#version 330

in vec3 frag_position;
in vec3 frag_normal;
in vec2 tex_coord;

uniform sampler2D u_texture_0;

layout(location = 0) out vec4 out_color;
layout(location = 1) out vec4 out_position;

void main() {
	out_position = vec4(frag_position, 1);

    out_color = texture(u_texture_0, tex_coord);
	if(out_color.a < 0.9)
		discard;
}