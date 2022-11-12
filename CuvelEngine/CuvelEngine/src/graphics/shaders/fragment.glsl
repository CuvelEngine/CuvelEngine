#version 450

layout (location = 0) in vec4 color;
layout (location = 1) in vec3 normal;
layout (location = 2) in float ambient;

layout (location = 0) out vec4 outColor;

uniform vec3 lightDir;
uniform uint hasLighting;

void main() {
  float final = 1.0;
  if (hasLighting == 1){
    vec3 norm = normalize(normal);
    vec3 light = normalize(-lightDir); 
    float diffuse = max(dot(norm, light), 0.0);
    final = ambient + diffuse;
  }
  outColor = vec4(color.rgb * final, color.a);
}
