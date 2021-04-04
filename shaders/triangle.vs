#version 330 core

layout (location = 0) in vec3 position;
out vec3 VertexColor;

uniform vec3 Color = vec3(1.0);

uniform mat4 Projection = mat4(1.0);
uniform mat4 View = mat4(1.0);
uniform mat4 Model = mat4(1.0);

void main() {
  gl_Position = Projection * View * Model * vec4(position, 1.0);
  VertexColor = Color;
}
