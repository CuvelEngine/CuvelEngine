#version 450

layout (location = 0) in vec4 color;
layout (location = 1) in vec3 normal;
layout (location = 2) in float ambient;

layout (location = 0) out vec4 outColor;

uniform vec3 lightDir;

void main() {
  vec3 norm = normalize(normal);
  vec3 light = normalize(lightDir); 
  float diffuse = max(dot(norm, light), 0.0);

  outColor = vec4(color.rgb * (ambient + diffuse), color.a);
}
