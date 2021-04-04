#version 330 core

layout (location = 0) in vec3 Position;
out vec3 vColor;

uniform mat4 Model = mat4(1.0);
uniform mat4 View = mat4(1.0);
uniform mat4 Projection = mat4(1.0);

void main() {
  mat4 mvp = Projection * View * Model;
  gl_Position = mvp * vec4(Position, 1.0);
  vColor = (Position + vec3(1.0)) / vec3(2.0);
}
