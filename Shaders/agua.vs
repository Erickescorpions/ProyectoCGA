#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoords;
layout(location = 2) in vec3 normal;

out vec2 fragTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    fragTexCoords = texCoords;
    gl_Position = projection * view * model * vec4(position, 1.0);
}
