#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;

layout(location = 0) out vec4 out_color;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main() {
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(position, 1.0f);
	out_color = color;
}