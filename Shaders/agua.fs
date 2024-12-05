#version 330 core
in vec2 fragTexCoords;
out vec4 FragColor;

uniform sampler2D waterTexture;
uniform float time;

void main() {
    // Modificar las coordenadas UV para simular movimiento
    vec2 movedTexCoords = fragTexCoords + vec2(sin(time * 0.1), cos(time * 0.1)) * 0.02;
    FragColor = texture(waterTexture, movedTexCoords);
}
