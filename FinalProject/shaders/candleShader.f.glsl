#version 410 core

//varyings
layout(location = 0) in vec3 light;
/*layout(location = 0) in vec3 color;     // interpolated color for this fragment
*/

//output
out vec4 colorOut;

void main() {
    //implementation might be wrong here. colorAmbient is always there so it's
    //additive, but not too certain on specular and diffusion.
    colorOut = vec4(light, 1.0);
}

