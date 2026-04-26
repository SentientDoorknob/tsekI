#version 450 core

uniform sampler2D container;
uniform sampler2D face;

uniform float time;

layout(location = 0) in vec4 fCol;
layout(location = 1) in vec2 texCoords;

layout(location = 0) out vec4 oCol;

void main() {
  oCol = mix(texture(container, texCoords), texture(face, texCoords), sin(time));
}
