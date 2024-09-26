#version 460 core
out vec4 fragColor;
in vec2 uvCoord;
uniform sampler2D texTarget;
uniform vec3 color;

void main() {
	fragColor = vec4(color, 1) * texture(texTarget, vec2(uvCoord.x, uvCoord.y));
}