#version 410 core

out vec4 fragColorOut;
uniform vec3 topperColor;

void main() {
    fragColorOut = vec4(topperColor, 1.0);
}
