#version 450 core

layout(location = 0) in vec4 fCol;

layout(location = 0) out vec4 oCol;

void main() {
  oCol = fCol;
}
