#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in uint normalIndex;

layout(location = 0) out vec4 out_color;
layout(location = 1) out vec3 out_normal;
layout(location = 2) out float out_ambient;

uniform mat4 ModelMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

uniform uint hasLighting;


vec3 normals[6] = vec3[](
	vec3( 0.0,  1.0,  0.0), //UP = 0
	vec3( 0.0, -1.0,  0.0), //DOWN = 1
	vec3( 0.0,  0.0,  1.0), //NORTH = 2
	vec3( 0.0,  0.0, -1.0), //SOUTH = 3
	vec3( 1.0,  0.0,  0.0), //WEST = 4
	vec3(-1.0,  0.0,  0.0)  //EAST = 5
);

void main() {
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(position, 1.0);
	out_color = color;
	out_normal = NormalMatrix * normals[normalIndex];
	out_ambient = 0.1 + (1 - hasLighting);
}