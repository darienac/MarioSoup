#version 330

#define PALLETE_SIZE 4

uniform sampler2D uTexture;

uniform vec4 uStartColors[PALLETE_SIZE];
uniform vec4 uEndColors[PALLETE_SIZE];

in vec4 vVertex;
in vec2 vTexCoord;

out vec4 fragColor;

void main() {
    vec4 baseColor = texture(uTexture, vTexCoord);
    for (int i = 0; i < PALLETE_SIZE; i++) {
        if (distance(baseColor, uStartColors[i]) < 0.03) {
            fragColor = uEndColors[i];
            return;
        }
    }
    fragColor = baseColor;
}