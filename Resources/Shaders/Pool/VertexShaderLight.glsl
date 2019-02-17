#version 330

/**
 * This vertex shader is used to render spheres using Gouraud shading model.
 */

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Uniforms for light properties
uniform vec3 light_position;
uniform vec3 eye_position;
uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform vec3 object_color;

// Output value to fragment shader
out vec3 color;

void main() {
    vec3 world_pos = (Model * vec4(v_position,1)).xyz;
	vec3 world_normal = normalize( mat3(Model) * v_normal );
	vec3 L = normalize( light_position - world_pos );
	vec3 V = normalize( eye_position - world_pos );
	vec3 H = normalize( L + V );

	// Define ambient light component
	float ambient_light = 0.8;
	vec3 ambientColor = ambient_light * object_color;


	// Compute diffuse light component
	float diffuse_light = material_kd * max(dot(world_normal,L),0);
	vec3 diffuseColor = diffuse_light * object_color;

	// Compute specular light component
	vec3 specular_color = vec3(0);
	float specular_light = 0;
	float specular_pow = 0;
	float attenuation = 0;


	specular_pow = pow (max(dot(world_normal,H), 0 ), material_shininess) * material_ks;
	specular_light = specular_pow;
	attenuation = 10 / (distance(light_position,world_pos) * distance(light_position,world_pos));
	specular_color = specular_light * object_color;

	// Send color light output to fragment shader
	color = (ambientColor + attenuation * (diffuseColor  + specular_color)) * vec3(1);
	gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
