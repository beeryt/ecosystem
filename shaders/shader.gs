#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in vec3 vColor[];
out vec3 Color;

float dx = 2.0 / 8.0 / 2.0 - 0.003125 * 4;
float dy = 2.0 / 8.0 / 2.0 - 0.003125 * 4;

void build_square(vec4 position) {
  gl_Position = position + vec4(-dx, -dy, 0, 0);
  Color = vColor[0];
  EmitVertex();
  gl_Position = position + vec4( dx, -dy, 0, 0);
  Color = vColor[0];
  EmitVertex();
  gl_Position = position + vec4(-dx,  dy, 0, 0);
  Color = vColor[0];
  EmitVertex();
  gl_Position = position + vec4( dx,  dy, 0, 0);
  Color = vColor[0];
  EmitVertex();
  EndPrimitive();
}

void main() {
  build_square(gl_in[0].gl_Position);
}
