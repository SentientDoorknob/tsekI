#version 450 core

layout(location = 0) in vec2 pos;
layout(location = 1) in vec4 col;
layout(location = 2) in vec2 texCoords;

uniform float time;

layout(location = 0) out vec4 fCol;
layout(location = 1) out vec2 fTexCoords;

void main() {
  gl_Position = vec4(cos(time) * pos.x, sin(time) * pos.y, 0, 1);
  fTexCoords = texCoords;
  fCol = col;
}
