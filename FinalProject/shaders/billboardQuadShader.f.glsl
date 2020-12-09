/*
 *   Fragment Shader
 *
 *   CSCI 441, Computer Graphics, Colorado School of Mines
 */

#version 410 core

// TODO #J - input varying variable
in vec2 texCoord;
// TODO #K
uniform sampler2D image;

out vec4 fragColorOut;

void main() {

    /*****************************************/
    /******* Final Color Calculations ********/
    /*****************************************/

    // TODO #L - loading the texel
    fragColorOut = texture(image,texCoord);
    //fragColorOut = vec4(1.0f, 1.0f, 0.0f, 1.0f);
}
