#version 330

uniform mat4 uViewProjectionMatrix;

in vec2 aVertex;
in vec2 aTexCoord;

out vec4 vVertex;
out vec2 vTexCoord;

void main() {
    vTexCoord = aTexCoord;
	vVertex = vec4(aVertex, 0.0, 1.0);
	gl_Position = uViewProjectionMatrix * vVertex;
}