#version 330

in vec3 vVertex;

out vec4 fragColor;

void main() {
	fragColor = vec4((vec2(1.0, 1.0) + vVertex.xy) * 0.5, 1.0, 1.0);
}