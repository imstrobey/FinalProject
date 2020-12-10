#version 410 core

in vec3 topperFragColor;

out vec4 fragColorOut;

void main() {
    fragColorOut = vec4(topperFragColor, 1.0);
}
