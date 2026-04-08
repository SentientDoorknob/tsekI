#version 450 core

layout(location = 0) in vec2 pos;
layout(location = 1) in vec4 col;

uniform float time;

layout(location = 0) out vec4 fCol;

void main() {
  gl_Position = vec4(pos.x, time * pos.y, 0, 1);
  fCol = col;
}
