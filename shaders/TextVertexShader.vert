#version 460 core
layout(location=0) in vec3 pos;
layout(location=1) in vec2 uv;

uniform mat4 transform;
uniform mat4 projection;

out vec2 uvCoord;

void main() {
	gl_Position = projection * transform * vec4(pos, 1);
	uvCoord = uv;
}