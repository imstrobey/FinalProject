/*
 *  CSCI 441, Computer Graphics, Fall 2020
 *
 *  Project: Midterm
 *  File: main.cpp
 *
 *	Base Code Author: Jeffrey Paone - Fall 2020
 *	Additional Work: Alex Langfield, Vlad Muresan, Emelyn Pak & Liam Stacy
 *
 *  Description:
 *      Contains the midterm project source code for pitcrew VAOs VBOs and MVPs
 */

// include the OpenGL library headers
#include <GL/glew.h>
#include <GLFW/glfw3.h>			// include GLFW framework header
// include GLM libraries and matrix functions
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
// include C and C++ libraries
#include <cmath>				// for cos(), sin() functionality
#include <cstdio>				// for printf functionality
#include <cstdlib>			    // for exit functionality
#include <ctime>			    // for time() functionality
#include <vector>
#include <iostream>
// include our class libraries
#include <CSCI441/OpenGLUtils.hpp>
#include <CSCI441/objects.hpp>  // for our 3D objects
#include <CSCI441/SimpleShader.hpp>
// include environment objects
#include <CSCI441/ShaderProgram.hpp>
#include <CSCI441/TextureUtils.hpp>     // convenience for loading textures

//*************************************************************************************
//
// Global Parameters

const GLint WINDOW_WIDTH = 640, WINDOW_HEIGHT = 480;    //window width and height
const int MAX_POS = 77;
const GLfloat quadSize = 80.0f;

int leftMouseButton;    	 				// status of the mouse button
double mouseX = -99999, mouseY = -99999;    // last known X and Y of the mouse

glm::vec3 camPos, camPos0, camPos1, camPos2;    // camera POSITION in cartesian coordinates for arcball cam
GLdouble cameraTheta, cameraPhi;                // camera DIRECTION in spherical coordinates for arcball cam
glm::vec3 arcballLookAtPoint;                   // arcball camera look at point
float radius = 50;                              // arcball camera radius
bool ctrlPressed = false;                       // for arcball camera zoom

bool arcballCamActive = true, freeCamActive = false, fpCamActive = false;
int activeCamera = 0, currCharView = 0; // track which camera and which character to use

struct Trees {                       // keeps track of an individual tree's attributes
    glm::mat4 modelMatrix;          // its position and size
    glm::vec3 color;                // its color
};
std::vector<Trees> trees;            // list of trees

glm::vec3 freeCamPos;            		// camera position in cartesian coordinates
glm::vec3 freeCamAngles;               	// camera DIRECTION in spherical coordinates stored as (theta, phi, radius)
glm::vec3 freeCamDir; 			        // camera DIRECTION in cartesian coordinates
glm::vec2 freeCamSpeed;                 // cameraSpeed --> x = forward/backward delta, y = rotational delta

glm::vec3 fpCamPos;
glm::vec3 fpCamLookAtPoint;

struct Bushes {                     // keeps track of an individual bush's attributes
    glm::mat4 modelMatrix;          // position and size of bush
    glm::vec3 color;                // bush color
};
std::vector<Bushes> bushes;         // list of bushes

// TODO candy canes
struct CandyCanes {                     // keeps track of an individual bush's attributes
    glm::mat4 modelMatrix;          // position and size of bush
    glm::vec3 color;                // bush color
};
std::vector<CandyCanes> candyCanes;         // list of bushes

// TODO Christmas Tree
struct ChristmasTree {
    glm::mat4 modelMatrix;
    glm::vec3 color;
};
ChristmasTree christmasTree;

struct ChristmasTreeTrunk {
    glm::mat4 modelMatrix;
    glm::vec3 color;
};
ChristmasTreeTrunk christmasTrunk;

const GLfloat MOVEMENT_CONSTANT = 2.0;  // movement coefficient (speed)
const GLfloat ROTATE_CONSTANT = 0.2;    // rotation coefficient (for camera)
const GLfloat ZOOM_COEF = 0.1;          // speed of zoom coefficient (for camera)

GLuint groundVAO;                       // the VAO descriptor for our ground plane

// TODO evil santa
/// global santa variabls
float santaX = 60.0f, santaY = 0.0f, santaZ = 60.0f; //10.0f, 8.0f, 10.0f
float santaRotation = 0.0f;
bool santaMoveRight = true;


/// Global Blossom variables
float blossomX = 5.0f,  blossomY = 1.5f, blossomZ = 5.0f;   // blossom position
float blossomRotation = 0.0f;                               // blossom rotation angle
bool blossomMoving = false;                                 // checks if blossom is moving for animation

/// jarrison global variables
float jarrisonX = 10.0f, jarrisonY = 1.5f, jarrisonZ = 0.0f; // jarrison position
float jarrisonRadians = 0.0f;                                // jarrison rotation angle
bool jarrisonMoving = false;                                 // tracks if jarrison is moving
const float MAX_EYE_OFFSET = 0.25f;                          // variables to animate jarrison
float currentEyeOffset = 0.0f;
bool eyesMovingInward = false;

/// Voltorb global variables
glm::vec3 playerPos;
glm::vec3 playerMove;
float rotAngle = 0.0;
bool playerMoving = false;

/// Shader Program information
CSCI441::ShaderProgram *lightingShader = nullptr;   // the wrapper for our shader program
struct LightingShaderUniforms {         // stores the locations of all of our shader uniforms
    // TODO #1 add variables to store the new uniforms that were created
    GLint normalMatrix, lightDirection, lightColor;
    GLint mvpMatrix;
    GLint materialColor;
    GLint camPosition;
    GLint lightPosition;
    GLint pointLightColor;
    GLint spotLightTheta;
    GLint spotLightPosition;
    GLint spotLightDirection;
    GLint spotLightColor;
} lightingShaderUniforms;
struct LightingShaderAttributes {       // stores the locations of all of our shader attributes
    // TODO #2 add variables to store the new attributes that were created
    GLint vertexNormal;
    GLint vPos;
} lightingShaderAttributes;

CSCI441::ShaderProgram *candleShader = nullptr;
struct CandleShaderUniforms {
    GLint normalMtx;
    GLint mvpMatrix;
/*
    GLint modelMtx;
*/
    GLint lightPositionPoint;
    GLint lightColorPoint;
    GLint lightDirection;
    GLint materialColor;
    GLint camPos;

} candleShaderUniforms;
struct CandleShaderAttributes {
    GLint vPos;
    GLint vNormal;
} candleShaderAttributes;

// Billboard shader program
CSCI441::ShaderProgram *billboardShaderProgram = nullptr;
struct BillboardShaderProgramUniforms {
    GLint mvMatrix;                     // the ModelView Matrix to apply
    GLint projMatrix;                   // the Projection Matrix to apply
    GLint image;                        // the texture to bind
} billboardShaderProgramUniforms;
struct BillboardShaderProgramAttributes {
    GLint vPos;                         // the vertex position
} billboardShaderProgramAttributes;

// Tree topper shader program
CSCI441::ShaderProgram *treeTopperShaderProgram = nullptr;
struct TreeTopperShaderProgramUniforms {
    GLint mvpMatrix;
    GLint topperColor;
} treeTopperShaderProgramUniforms;
struct TreeTopperShaderProgramAttributes {
    GLint vPos;                         // the vertex position
} treeTopperShaderProgramAttributes;

glm::vec3 topperColor;

/// keeps track of sprite shader program
CSCI441::ShaderProgram *texShaderProgram = nullptr;
struct TexShaderProgramUniforms {
    GLint mvpMatrix;                    // the MVP Matrix to apply
    GLint textureMap;
} texShaderProgramUniforms;
struct TexShaderProgramAttributes {
    GLint vPos;                         // position of our vertex
    GLint vTexCoord;
} texShaderProgramAttributes;


// point sprite information
const GLuint NUM_SPRITES = 300;          // the number of sprites to draw /// TODO redraw sprites
const GLfloat MAX_BOX_SIZE = 80;        // our sprites exist within a box of this size
glm::vec3* spriteLocations = nullptr;   // the (x,y,z) location of each sprite
GLushort* spriteIndices = nullptr;      // the order to draw the sprites in
GLfloat* distances = nullptr;           // will be used to store the distance to the camera
GLuint spriteTextureHandle;             // the texture to apply to the sprite
const GLfloat NEW_BOX_SIZE = 35;
glm::vec3 gravity = glm::vec3(0,-1.0f,0);

// all drawing information
const struct VAO_IDS {
    GLuint PARTICLE_SYSTEM = 0;
} VAOS;
const GLuint NUM_VAOS = 1;
GLuint vaos[NUM_VAOS];                  // an array of our VAO descriptors
GLuint vbos[NUM_VAOS];                  // an array of our VBO descriptors
GLuint ibos[NUM_VAOS];                  // an array of our IBO descriptors

/// create texture handles for each side of the skybox
GLuint rightTextureHandle;
GLuint leftTextureHandle;
GLuint topTextureHandle;
//GLuint bottomTextureHandle;
GLuint frontTextureHandle;
GLuint backTextureHandle;

/// create VAOs and VBOs for each side of the skybox
GLuint rightVAO,rightVBO[2];
GLuint leftVAO,leftVBO[2];
GLuint topVAO,topVBO[2];
GLuint bottomVAO,bottomVBO[2];
GLuint frontVAO,frontVBO[2];
GLuint backVAO,backVBO[2];


void renderScene(glm::mat4 view, glm::mat4 proj);

bool mackHack = false;

// END GLOBAL VARIABLES
//********************************************************************************

//********************************************************************************
// Helper Functions

GLdouble getRand() { return rand() / (GLdouble)RAND_MAX; }

GLfloat randNumber( GLfloat max ) {
    return rand() / (GLfloat)RAND_MAX * max * 2.0 - max;
}

// update arcball camera position based on phi, theta and radius
void updateArcballCamera() {
    // convert our theta and phi spherical angles to a cartesian vector
    // camera position
    camPos0 = {radius*glm::sin(cameraTheta)*glm::sin(cameraPhi),
               -radius*glm::cos(cameraPhi),
               -radius*glm::cos(cameraTheta)*glm::sin(cameraPhi)};
    camPos1 = {radius*glm::sin(cameraTheta)*glm::sin(cameraPhi),
               -radius*glm::cos(cameraPhi),
               -radius*glm::cos(cameraTheta)*glm::sin(cameraPhi)};
    camPos2 = {radius*glm::sin(cameraTheta)*glm::sin(cameraPhi),
               -radius*glm::cos(cameraPhi),
               -radius*glm::cos(cameraTheta)*glm::sin(cameraPhi)};

    // look at point, based on character
    // jarrison cam
    if (currCharView % 3 == 0) {
        arcballLookAtPoint = glm::vec3{jarrisonX, jarrisonY, jarrisonZ};
        camPos = camPos0;
    }
        // voltorb cam
    else if (currCharView % 3 == 1) {
        arcballLookAtPoint = glm::vec3{playerMove.x, playerMove.y, playerMove.z};
        camPos = camPos1;
    }
        // blossom cam
    else {
        arcballLookAtPoint = glm::vec3{blossomX, blossomY, blossomZ};
        camPos = camPos2;
    }
}

// update free camera position based on user input
void updateFreeCamDirection() {
    // ensure phi doesn't flip our camera
    if(freeCamAngles.y <= 0 ) freeCamAngles.y = 0.0f + 0.001f;
    if(freeCamAngles.y >= M_PI ) freeCamAngles.y = M_PI - 0.001f;

    // convert from spherical to cartesian in our RH coord. sys.
    freeCamDir.x = freeCamAngles.z * sinf(freeCamAngles.x ) * sinf(freeCamAngles.y );
    freeCamDir.y = freeCamAngles.z * -cosf(freeCamAngles.y );
    freeCamDir.z = freeCamAngles.z * -cosf(freeCamAngles.x ) * sinf(freeCamAngles.y );

    // normalize the direction for a free cam
    freeCamDir = glm::normalize(freeCamDir);
}

// create scissor box for first person view in bottom left corner of screen
void updateFirstPerson() {
    // jarrison fp cam
    if (currCharView % 3 == 0) {
        fpCamPos = {jarrisonX + 5.0f * sin(jarrisonRadians), jarrisonY + 2.0, jarrisonZ + 5.0f * cos(jarrisonRadians)};
        fpCamLookAtPoint = {jarrisonX + 10.0f * sin(jarrisonRadians) , jarrisonY + 2.0f, (jarrisonZ) + 10.0f * cos(jarrisonRadians)};
    }
        // voltorb fp cam
    else if (currCharView % 3 == 1) {
        fpCamPos = {playerPos.x + 5.0f * sin(rotAngle), playerPos.y + 2.0, playerPos.z + 5.0f * cos(rotAngle)};
        fpCamLookAtPoint = {playerPos.x + 10.0f * sin(rotAngle) , playerPos.y + 2.0f, (playerPos.z) + 10.0f * cos(rotAngle)};
    }
        // blossom fp cam
    else {
        fpCamPos = {blossomX + 5.0f * sin(blossomRotation), blossomY + 2.0, blossomZ + 5.0f * cos(blossomRotation)};
        fpCamLookAtPoint = {blossomX + 10.0f * sin(blossomRotation) , blossomY + 2.0f, (blossomZ) + 10.0f * cos(blossomRotation)};
    }
}

void computeMtxUniformsCandle(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) {
    glm::mat4 mvpMtx = projMtx * viewMtx * modelMtx;
    glUniformMatrix4fv(candleShaderUniforms.mvpMatrix, 1, GL_FALSE, &mvpMtx[0][0]);

    glm::mat3 normalMtx = glm::mat3(glm::transpose(glm::inverse(modelMtx)));
    glUniformMatrix3fv(candleShaderUniforms.normalMtx, 1, GL_FALSE, &normalMtx[0][0]);

/*
    glUniformMatrix4fv(candleShaderUniforms.modelMtx, 1, GL_FALSE, &modelMtx[0][0]);
*/
}

void computeAndSendMatrixUniforms(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) {
    // precompute the Model-View-Projection matrix on the CPU, then send it to the shader on the GPU
    glm::mat4 mvpMtx = projMtx * viewMtx * modelMtx;
    glUniformMatrix4fv( lightingShaderUniforms.mvpMatrix, 1, GL_FALSE, &mvpMtx[0][0] );

    glm::mat3 normMtx = glm::mat3(glm::transpose(glm::inverse(modelMtx)));
    glUniformMatrix3fv(lightingShaderUniforms.normalMatrix,1,GL_FALSE,&normMtx[0][0]);
}

void computeAndSendTransformationMatrices5(glm::mat4 modelMatrix, glm::mat4 viewMatrix, glm::mat4 projectionMatrix,
                                          GLint mvMtxLocation, GLint projMtxLocation) {
    glm::mat4 mvMatrix = viewMatrix * modelMatrix;

    glUniformMatrix4fv(mvMtxLocation, 1, GL_FALSE, &mvMatrix[0][0]);
    glUniformMatrix4fv(projMtxLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
}

void computeAndSendTransformationMatrices6(glm::mat4 modelMatrix, glm::mat4 viewMatrix, glm::mat4 projectionMatrix,
                                          GLint mvpMtxLocation, GLint modelMtxLocation = -1, GLint normalMtxLocation = -1) {
    glm::mat4 mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;
    glm::mat3 normalMatrix = glm::mat3( glm::transpose( glm::inverse(modelMatrix) ) );

    glUniformMatrix4fv(mvpMtxLocation, 1, GL_FALSE, &mvpMatrix[0][0]);
    glUniformMatrix4fv(modelMtxLocation, 1, GL_FALSE, &modelMatrix[0][0]);
    glUniformMatrix3fv(normalMtxLocation, 1, GL_FALSE, &normalMatrix[0][0]);
}

// Event Callbacks

//	when an error within GLFW occurs, GLFW will tell us by calling this function.
static void error_callback( int error, const char* description ) {
    fprintf( stderr, "[ERROR]: %d\n\t%s\n", error, description );
}

// register keyboard keys to update program variables
static void keyboard_callback( GLFWwindow *window, int key, int scancode, int action, int mods ) {
    if( action == GLFW_PRESS ) {
        switch( key ) {
            case GLFW_KEY_ESCAPE:
            case GLFW_KEY_Q:
                glfwSetWindowShouldClose(window, GLFW_TRUE);
                break;
            default: break; // to remove CLion warning
        }
    }
    if( action == GLFW_PRESS || action == GLFW_REPEAT ) {
        ctrlPressed = key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL; // t/f if ctrl is pressed for zoom
        switch( key ) {
            case GLFW_KEY_W: // move jarrison forward
                jarrisonZ += cos(jarrisonRadians) * MOVEMENT_CONSTANT;
                jarrisonX += sin(jarrisonRadians) * MOVEMENT_CONSTANT;
                if (jarrisonZ > MAX_POS) jarrisonZ = MAX_POS - 0.01;
                if (jarrisonX > MAX_POS) jarrisonX = MAX_POS - 0.01;
                if (jarrisonZ < -MAX_POS) jarrisonZ = -MAX_POS + 0.01;
                if (jarrisonX < -MAX_POS) jarrisonX = -MAX_POS + 0.01;
                camPos0 += glm::vec3{sin(jarrisonRadians) * MOVEMENT_CONSTANT, 0.0f, cos(jarrisonRadians) * MOVEMENT_CONSTANT};
                jarrisonMoving = true;
                break;
            case GLFW_KEY_S: // move jarrison backward
                jarrisonZ -= cos(jarrisonRadians) * MOVEMENT_CONSTANT;
                jarrisonX -= sin(jarrisonRadians) * MOVEMENT_CONSTANT;
                if (jarrisonZ > MAX_POS) jarrisonZ = MAX_POS - 0.01;
                if (jarrisonX > MAX_POS) jarrisonX = MAX_POS - 0.01;
                if (jarrisonZ < -MAX_POS) jarrisonZ = -MAX_POS + 0.01;
                if (jarrisonX < -MAX_POS) jarrisonX = -MAX_POS + 0.01;
                camPos0 += glm::vec3{sin(jarrisonRadians) * MOVEMENT_CONSTANT, 0.0f, cos(jarrisonRadians) * MOVEMENT_CONSTANT};
                jarrisonMoving = true;
                break;
            case GLFW_KEY_A: // rotate jarrison left
                jarrisonRadians += ROTATE_CONSTANT;
                jarrisonMoving = true;
                break;
            case GLFW_KEY_D: // rotate jarrison right
                jarrisonRadians -= ROTATE_CONSTANT;
                jarrisonMoving = true;
                break;
            case GLFW_KEY_T: // move voltorb forward
                playerMove.z += sin(rotAngle) * MOVEMENT_CONSTANT;
                playerMove.x += cos(rotAngle) * MOVEMENT_CONSTANT;
                if (playerMove.z < -MAX_POS) playerMove.z = -MAX_POS + 0.01;
                if (playerMove.x < -MAX_POS) playerMove.x = -MAX_POS + 0.01;
                if (playerMove.z > MAX_POS) playerMove.z = MAX_POS - 0.01;
                if (playerMove.x > MAX_POS) playerMove.x = MAX_POS - 0.01;
                camPos1 += glm::vec3{cos(rotAngle) * MOVEMENT_CONSTANT, 0.0f, sin(rotAngle) * MOVEMENT_CONSTANT};
                playerMoving = true;
                break;
            case GLFW_KEY_G: // move voltorb backward
                playerMove.z -= sin(rotAngle) * MOVEMENT_CONSTANT;
                playerMove.x -= cos(rotAngle) * MOVEMENT_CONSTANT;
                if (playerMove.z < -MAX_POS) playerMove.z = -MAX_POS + 0.01;
                if (playerMove.x < -MAX_POS) playerMove.x = -MAX_POS + 0.01;
                if (playerMove.z > MAX_POS) playerMove.z = MAX_POS - 0.01;
                if (playerMove.x > MAX_POS) playerMove.x = MAX_POS - 0.01;
                camPos1 += glm::vec3{cos(rotAngle) * MOVEMENT_CONSTANT, 0.0f, sin(rotAngle) * MOVEMENT_CONSTANT};
                playerMoving = true;
                break;
            case GLFW_KEY_F: // rotate voltorb left
                rotAngle -= ROTATE_CONSTANT;
                playerMoving = true;
                break;
            case GLFW_KEY_H: // rotate voltorb right
                rotAngle += ROTATE_CONSTANT;
                playerMoving = true;
                break;
            case GLFW_KEY_I: // move blossom forward
                blossomZ += cos(blossomRotation) * MOVEMENT_CONSTANT;
                blossomX += sin(blossomRotation) * MOVEMENT_CONSTANT;
                if (blossomZ > MAX_POS) blossomZ = MAX_POS - 0.01;
                if (blossomX > MAX_POS) blossomX = MAX_POS - 0.01;
                if (blossomZ < -MAX_POS) blossomZ = -MAX_POS + 0.01;
                if (blossomX < -MAX_POS) blossomX = -MAX_POS + 0.01;
                camPos2 += glm::vec3{sin(blossomRotation) * MOVEMENT_CONSTANT, 0.0f, cos(blossomRotation) * MOVEMENT_CONSTANT};
                blossomMoving = true;
                break;
            case GLFW_KEY_K: // move blossom backward
                blossomZ -= cos(blossomRotation) * MOVEMENT_CONSTANT;
                blossomX -= sin(blossomRotation) * MOVEMENT_CONSTANT;
                if (blossomZ > MAX_POS) blossomZ = MAX_POS - 0.01;
                if (blossomX > MAX_POS) blossomX = MAX_POS - 0.01;
                if (blossomZ < -MAX_POS) blossomZ = -MAX_POS + 0.01;
                if (blossomX < -MAX_POS) blossomX = -MAX_POS + 0.01;
                camPos2 += glm::vec3{sin(blossomRotation) * MOVEMENT_CONSTANT, 0.0f, cos(blossomRotation) * MOVEMENT_CONSTANT};
                blossomMoving = true;
                break;
            case GLFW_KEY_J: // rotate blossom left
                blossomRotation += ROTATE_CONSTANT;
                blossomMoving = true;
                break;
            case GLFW_KEY_L: // rotate blossom right
                blossomRotation -= ROTATE_CONSTANT;
                blossomMoving = true;
                break;
            case GLFW_KEY_C:
                currCharView += 1;
                break;
            case GLFW_KEY_1:
                activeCamera += 1;
                if (activeCamera % 2 == 0) {
                    arcballCamActive = true;
                    freeCamActive = false;
                }
                else {
                    arcballCamActive = false;
                    freeCamActive = true;
                    freeCamPos = camPos; // makes camera stay in the same location when switching cameras
                }
                break;
            case GLFW_KEY_2:
                fpCamActive = !fpCamActive;
                arcballCamActive = true;
                freeCamActive = false;
                break;
            case GLFW_KEY_SPACE: // move free cam inward
                freeCamPos += freeCamDir * MOVEMENT_CONSTANT;
                break;
            case GLFW_KEY_X:  // move free cam outward
                freeCamPos -= freeCamDir * MOVEMENT_CONSTANT;
                break;
                // turn free cam right
            case GLFW_KEY_RIGHT:
                freeCamAngles.x += freeCamSpeed.y;
                updateFreeCamDirection();
                break;
                // turn free cam left
            case GLFW_KEY_LEFT:
                freeCamAngles.x -= freeCamSpeed.y;
                updateFreeCamDirection();
                break;
                // free cam pitch up
            case GLFW_KEY_UP:
                freeCamAngles.y += freeCamSpeed.y;
                updateFreeCamDirection();
                break;
                // free cam pitch down
            case GLFW_KEY_DOWN:
                freeCamAngles.y -= freeCamSpeed.y;
                updateFreeCamDirection();
                break;
            default: break; // to remove CLion warning
        }
    }
    else { // for animation and pressing control (camera zoom)
        jarrisonMoving = false;
        ctrlPressed = false;
        blossomMoving = false;
        playerMoving = false;
    }
    if (arcballCamActive) updateArcballCamera();      // update arcball camera (x,y,z) based on (radius,theta,phi)
    else if (freeCamActive) updateFreeCamDirection(); // update free camera
    if (fpCamActive) updateFirstPerson();             // if the first person cam is active,
}

// update program variables based on cursor
static void cursor_callback( GLFWwindow *window, double x, double y ) {
    // if mouse hasn't moved in the window, prevent camera from flipping out
    if(mouseX == -99999) {
        mouseX = x;
        mouseY = y;
    }
    if(leftMouseButton == GLFW_PRESS) {
        if (ctrlPressed) {
            radius -= ZOOM_COEF * (y - mouseY);
            if (radius < 0.5) radius = 0.51;
        }
        else {
            // update camera theta and phi
            // change theta based on mouses x position
            cameraTheta = cameraTheta - 0.005 * (x - mouseX);
            // change phi based on mouses y position
            cameraPhi = cameraPhi + 0.005 * (y - mouseY);
            // bound phi
            if (cameraPhi <= 0) cameraPhi = 0.0f + 0.001f;
            if (cameraPhi >= M_PI) cameraPhi = M_PI - 0.001f;
        }
        if (arcballCamActive) updateArcballCamera();     //update camera (x,y,z) based on (radius,theta,phi)
        else if (freeCamActive) updateFreeCamDirection();
        else if (fpCamActive) updateFirstPerson();
    }

    mouseX = x;
    mouseY = y;
}

static void mouse_button_callback( GLFWwindow *window, int button, int action, int mods ) {
    if( button == GLFW_MOUSE_BUTTON_LEFT) {
        leftMouseButton = action;
    }
}

/// drawing trees for the environment
void drawTree(Trees tree, glm::mat4 viewMtx, glm::mat4 projMtx) {
    /// base of tree
    computeAndSendMatrixUniforms(tree.modelMatrix, viewMtx, projMtx);
    glUniform3fv(lightingShaderUniforms.materialColor, 1, &tree.color[0]);
    CSCI441::drawSolidCylinder(1.0f, 1.0f, 15.0f, 10, 10);

    /// top of tree
    glm::mat4 transMtx = glm::translate(glm::mat4(1.0f), glm::vec3(0, 15.0f, 0));
    glm::mat4 scaleMtx = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 3.0f, 1.0f));
    glm::mat4 leafModelMtx =  transMtx * scaleMtx * tree.modelMatrix;
    computeAndSendMatrixUniforms(leafModelMtx, viewMtx, projMtx);
    glUniform3fv(lightingShaderUniforms.materialColor, 1, &tree.color[0]);
    CSCI441::drawSolidSphere(3.0f, 30, 30);
}

// TODO Christmas Tree
/// drawing christmas tree
void drawChristmasTree(ChristmasTree tree, glm::mat4 viewMtx, glm::mat4 projMtx){
    computeAndSendMatrixUniforms(tree.modelMatrix, viewMtx, projMtx);
    glUniform3fv(lightingShaderUniforms.materialColor, 1, &tree.color[0]);
    CSCI441::drawSolidCone(20.0f, 20.0f, 10, 10);

    glm::mat4 transMtx = glm::translate(glm::mat4(1.0f), glm::vec3(-35.0f, 18.0f, -25.0f));
    computeAndSendMatrixUniforms(transMtx, viewMtx, projMtx);
    CSCI441::drawSolidCone(14.0f, 14.0f, 10, 10);

    transMtx = glm::translate(transMtx, glm::vec3(0.0f, 9.0f, 0.0f));
    computeAndSendMatrixUniforms(transMtx, viewMtx, projMtx);
    CSCI441::drawSolidCone(10.0f, 10.0f, 10, 10);

    transMtx = glm::translate(transMtx, glm::vec3(0.0f, 6.0f, 0.0f));
    computeAndSendMatrixUniforms(transMtx, viewMtx, projMtx);
    CSCI441::drawSolidCone(6.0f, 6.0f, 10, 10);

    transMtx = glm::translate(transMtx, glm::vec3(0.0f, 4.0f, 0.0f));
    computeAndSendMatrixUniforms(transMtx, viewMtx, projMtx);
    CSCI441::drawSolidCone(4.0f, 4.0f, 10, 10);
}

void drawChristmasTreeTrunk(ChristmasTreeTrunk trunk, glm::mat4 viewMtx, glm::mat4 projMtx){
    computeAndSendMatrixUniforms(trunk.modelMatrix, viewMtx, projMtx);
    glUniform3fv(lightingShaderUniforms.materialColor, 1, &trunk.color[0]);
    CSCI441::drawSolidCylinder(4.0f, 4.0f, 10.0f, 10, 10);
}

void drawCandyCane(CandyCanes cane, glm::mat4 viewMtx, glm::mat4 projMtx){
    computeAndSendMatrixUniforms(cane.modelMatrix, viewMtx, projMtx);
    glm::vec3 candyColor(glm::vec3{1.0,0.0,0.0});
    glUniform3fv(lightingShaderUniforms.materialColor, 1, &candyColor[0]);
    glm::mat4 transMtx = cane.modelMatrix;

    glm::vec3 candyColorRed = glm::vec3{1.0,0.0,0.0};
    glm::vec3 candyColorWhite = glm::vec3{1.0,1.0,1.0};
    float base = 2.0f,top = 1.8f;

    for(int i = 0; i < 8; i++){
        if(i%2) { candyColor = candyColorRed; }
        else { candyColor = candyColorWhite; }
        base -= 0.2;
        top -= 0.2;
        glUniform3fv(lightingShaderUniforms.materialColor, 1, &candyColor[0]);
        CSCI441::drawSolidCylinder(base,top,3.0f,10,10);
        transMtx = glm::translate(transMtx,glm::vec3(0.0f,3.0f,0.0f));
        cane.modelMatrix = transMtx;
        computeAndSendMatrixUniforms(cane.modelMatrix, viewMtx, projMtx);
    }

    candyColor = candyColorWhite;
    glUniform3fv(lightingShaderUniforms.materialColor, 1, &candyColor[0]);
    CSCI441::drawSolidCone(0.2f,3.0f,10,10);
    transMtx = glm::translate(transMtx,glm::vec3(0.0f,3.0f,0.0f));
    cane.modelMatrix = transMtx;
    computeAndSendMatrixUniforms(cane.modelMatrix, viewMtx, projMtx);
}


// generateEnvironment() ///////////////////////////////////////////////////////
//
//  This function creates our world which will consist of a grid in the XZ-Plane
//      and randomly placed and sized buildings of varying colors.
//
////////////////////////////////////////////////////////////////////////////////

//  This function creates our world which consists of trees, bushes and a grid
void generateEnvironment() {
    // parameters to size our world
    const GLint GRID_WIDTH = 100;
    const GLint GRID_LENGTH = 100;
    const GLfloat GRID_SPACING_WIDTH = 1.0f;
    const GLfloat GRID_SPACING_LENGTH = 1.0f;
    const GLfloat LEFT_END_POINT = -GRID_WIDTH / 2.0f - 5;
    const GLfloat RIGHT_END_POINT = GRID_WIDTH / 2.0f + 5;
    const GLfloat BOTTOM_END_POINT = -GRID_LENGTH / 2.0f - 5;
    const GLfloat TOP_END_POINT = GRID_LENGTH / 2.0f + 5;

    // TODO Christmas Tree
    glm::mat4 transMtx = glm::translate(glm::mat4(1.0f), glm::vec3(-35.0f, 5.0f, -25.0f));
    christmasTree.modelMatrix = transMtx;
    christmasTree.color = glm::vec3(0.0f,1.0f,0.0f);

    transMtx = glm::translate(transMtx, glm::vec3(0.0f,-5.0f,0.0f));
    christmasTrunk.modelMatrix = transMtx;
    christmasTrunk.color = glm::vec3(0.4f, 0.2f, 0.2f);


    // generate random trees and bushes to put around the scene
    for (int i = LEFT_END_POINT; i < RIGHT_END_POINT; i += GRID_SPACING_WIDTH) { // go left to right
        for (int j = BOTTOM_END_POINT; j < TOP_END_POINT; j += GRID_SPACING_LENGTH) { // go bottom to top
            if (i % 3 == 0 && j % 3 == 0 && getRand() < 0.01) { // if the point is even and random_variable < 0.4
                // TODO Christmas Canes
                CandyCanes cane;
                glm::mat4 transMtx = glm::translate(glm::mat4(1.0f), glm::vec3(i, 0.0f, j));
                cane.modelMatrix = transMtx;
                candyCanes.push_back(cane);
            }
        }
    }
}

// TODO Evil Santa
/// drawing santa hierarchically
void drawSantaBody(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx){
    modelMtx = glm::translate(modelMtx, glm::vec3(10.0f, 8.0f, 10.0f));
    modelMtx = glm::scale(modelMtx, glm::vec3(5.0f, 5.0f, 5.0f));
    computeAndSendMatrixUniforms(modelMtx,viewMtx,projMtx);
    glm::vec3 santaBodyColor(glm::vec3{1.0,0.0,0.0});
    glUniform3fv(lightingShaderUniforms.materialColor,1,&santaBodyColor[0]);
    CSCI441::drawSolidSphere(1.0f,10,10);
}
void drawSantaLeftArm(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx){
    modelMtx = glm::translate(modelMtx, glm::vec3(6.0f, 9.0f, 10.0f));
    modelMtx = glm::scale(modelMtx, glm::vec3(5.0f, 5.0f, 5.0f));
    modelMtx = glm::rotate(modelMtx, glm::radians(90.0f), glm::vec3(1.0f,1.0f,10.0f));
    computeAndSendMatrixUniforms(modelMtx,viewMtx,projMtx);
    glm::vec3 santaHeadColor(glm::vec3{1.0,0.0,0.0});
    glUniform3fv(lightingShaderUniforms.materialColor,1,&santaHeadColor[0]);
    CSCI441::drawSolidCylinder(0.4f, 0.3f, 1.5, 10, 10);
}
void drawSantaLeftHand(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx){
    modelMtx = glm::translate(modelMtx, glm::vec3(-1.8f, 9.0f, 11.5f));
    modelMtx = glm::scale(modelMtx, glm::vec3(1.3f, 2.0f, 2.0f));
    computeAndSendMatrixUniforms(modelMtx,viewMtx,projMtx);
    glm::vec3 santaBodyColor(glm::vec3{0.4,0.4,0.4});
    glUniform3fv(lightingShaderUniforms.materialColor,1,&santaBodyColor[0]);
    CSCI441::drawSolidSphere(0.8f,10,10);
}

void drawSantaBazookaBody(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx){
    modelMtx = glm::translate(modelMtx, glm::vec3(-2.0f, 12.0f, 2.0f));
    modelMtx = glm::scale(modelMtx, glm::vec3(5.0f, 5.0f, 5.0f));
    modelMtx = glm::rotate(modelMtx, glm::radians(90.0f), glm::vec3(10.0f,1.0f,1.0f));
    computeAndSendMatrixUniforms(modelMtx,viewMtx,projMtx);
    glm::vec3 santaHeadColor(glm::vec3{0.0,0.0,0.0});
    glUniform3fv(lightingShaderUniforms.materialColor,1,&santaHeadColor[0]);
    CSCI441::drawSolidCylinder(0.2f, 0.2f, 2.5, 10, 10);
}

void drawSantaBazookaFrontEnd(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx){
    modelMtx = glm::translate(modelMtx, glm::vec3(-2.0f, 12.0f, 2.4f));
    modelMtx = glm::scale(modelMtx, glm::vec3(2.0f, 2.0f, 1.0f));
    computeAndSendMatrixUniforms(modelMtx,viewMtx,projMtx);
    glm::vec3 santaBodyColor(glm::vec3{0.0,1.0,0.0});
    glUniform3fv(lightingShaderUniforms.materialColor,1,&santaBodyColor[0]);
    CSCI441::drawSolidSphere(0.8f,10,10);
}

void drawSantaBazookaBackEnd(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx){
    modelMtx = glm::translate(modelMtx, glm::vec3(-2.0f, 12.0f, 15.2f));
    modelMtx = glm::scale(modelMtx, glm::vec3(2.0f, 2.0f, 1.0f));
    computeAndSendMatrixUniforms(modelMtx,viewMtx,projMtx);
    glm::vec3 santaBodyColor(glm::vec3{0.0,1.0,0.0});
    glUniform3fv(lightingShaderUniforms.materialColor,1,&santaBodyColor[0]);
    CSCI441::drawSolidSphere(0.8f,10,10);
}
void drawSantaBazookaHandle(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx){
    modelMtx = glm::translate(modelMtx, glm::vec3(-2.0f, 10.0f, 11.5f));
    modelMtx = glm::scale(modelMtx, glm::vec3(2.0f, 5.0f, 2.0f));
    computeAndSendMatrixUniforms(modelMtx,viewMtx,projMtx);
    glm::vec3 santaBeltBuckleColor(glm::vec3{0.0,0.0,0.0});
    glUniform3fv(lightingShaderUniforms.materialColor,1,&santaBeltBuckleColor[0]);
    CSCI441::drawSolidCube(0.5f);
}

void drawSantaBazookaScopeBody(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx){
    modelMtx = glm::translate(modelMtx, glm::vec3(-2.0f, 13.0f, 5.5f));
    modelMtx = glm::scale(modelMtx, glm::vec3(2.0f, 5.0f, 2.0f));
    computeAndSendMatrixUniforms(modelMtx,viewMtx,projMtx);
    glm::vec3 santaBeltBuckleColor(glm::vec3{0.0,0.0,0.0});
    glUniform3fv(lightingShaderUniforms.materialColor,1,&santaBeltBuckleColor[0]);
    CSCI441::drawSolidCube(0.5f);
}

void drawSantaBazookaScopeVisor(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx){
    modelMtx = glm::translate(modelMtx, glm::vec3(-2.0f, 15.0f, 5.5f));
    modelMtx = glm::scale(modelMtx, glm::vec3(6.0f, 6.0f, 1.0f));
    computeAndSendMatrixUniforms(modelMtx,viewMtx,projMtx);
    glm::vec3 santaBeltBuckleColor(glm::vec3{1.0,0.0,0.0});
    glUniform3fv(lightingShaderUniforms.materialColor,1,&santaBeltBuckleColor[0]);
    CSCI441::drawSolidCube(0.5f);
}

void drawSantaRightArm(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx){
    modelMtx = glm::translate(modelMtx, glm::vec3(14.0f, 9.0f, 10.0f));
    modelMtx = glm::scale(modelMtx, glm::vec3(5.0f, 5.0f, 5.0f));
    modelMtx = glm::rotate(modelMtx, glm::radians(90.0f), glm::vec3(1.0f,1.0f,-10.0f));
    computeAndSendMatrixUniforms(modelMtx,viewMtx,projMtx);
    glm::vec3 santaHeadColor(glm::vec3{1.0,0.0,0.0});
    glUniform3fv(lightingShaderUniforms.materialColor,1,&santaHeadColor[0]);
    CSCI441::drawSolidCylinder(0.4f, 0.3f, 1.5, 10, 10);
}
void drawSantaRightHand(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx){
    modelMtx = glm::translate(modelMtx, glm::vec3(21.3f, 9.0f, 10.0f));
    modelMtx = glm::scale(modelMtx, glm::vec3(1.3f, 2.0f, 2.0f));
    computeAndSendMatrixUniforms(modelMtx,viewMtx,projMtx);
    glm::vec3 santaBodyColor(glm::vec3{0.4,0.4,0.4});
    glUniform3fv(lightingShaderUniforms.materialColor,1,&santaBodyColor[0]);
    CSCI441::drawSolidSphere(0.8f,10,10);
}

void drawSantaBelt(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx){
    modelMtx = glm::translate(modelMtx, glm::vec3(10.0f, 5.0f, 10.0f));
    modelMtx = glm::scale(modelMtx, glm::vec3(5.0f, 5.0f, 5.0f));
    computeAndSendMatrixUniforms(modelMtx,viewMtx,projMtx);
    glm::vec3 santaBeltColor(glm::vec3{0.0,0.0,0.0});
    glUniform3fv(lightingShaderUniforms.materialColor,1,&santaBeltColor[0]);
    CSCI441::drawSolidCylinder(1.05f,1.05f,0.5f,10,10);
}

void drawSantaBeltBuckle(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx){
    modelMtx = glm::translate(modelMtx, glm::vec3(10.0f, 6.0f, 5.5f));
    modelMtx = glm::scale(modelMtx, glm::vec3(5.0f, 5.0f, 5.0f));
    computeAndSendMatrixUniforms(modelMtx,viewMtx,projMtx);
    glm::vec3 santaBeltBuckleColor(glm::vec3{0.9,0.8,0.0});
    glUniform3fv(lightingShaderUniforms.materialColor,1,&santaBeltBuckleColor[0]);
    CSCI441::drawSolidCube(0.5f);
}

void drawSantaShirtHem(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx){
    modelMtx = glm::translate(modelMtx, glm::vec3(10.0f, 3.0f, 10.0f));
    modelMtx = glm::scale(modelMtx, glm::vec3(5.0f, 5.0f, 5.0f));
    computeAndSendMatrixUniforms(modelMtx,viewMtx,projMtx);
    glm::vec3 santaShirtHemColor(glm::vec3{1.0,1.0,1.0});
    glUniform3fv(lightingShaderUniforms.materialColor,1,&santaShirtHemColor[0]);
    CSCI441::drawSolidCylinder(1.05f, 1.05f, 0.4, 10, 10);
}

void drawSantaLegs(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx){
    modelMtx = glm::translate(modelMtx, glm::vec3(7.0f, 0.0f, 10.0f));
    modelMtx = glm::scale(modelMtx, glm::vec3(5.0f, 5.0f, 5.0f));
    computeAndSendMatrixUniforms(modelMtx,viewMtx,projMtx);
    glm::vec3 santaLegColor(glm::vec3{1.0,0.0,0.0});
    glUniform3fv(lightingShaderUniforms.materialColor,1,&santaLegColor[0]);
    CSCI441::drawSolidCylinder(0.25f, 0.25f, 1.0f, 10, 10);

    modelMtx = glm::translate(modelMtx, glm::vec3(1.0f,0.0f,0.0f));
    computeAndSendMatrixUniforms(modelMtx,viewMtx,projMtx);
    CSCI441::drawSolidCylinder(0.25f, 0.25f, 1.0f, 10, 10);
}

void drawSantaShoes(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx){
    modelMtx = glm::translate(modelMtx, glm::vec3(7.0f, 0.0f, 10.0f));
    modelMtx = glm::scale(modelMtx, glm::vec3(5.0f, 5.0f, 5.0f));
    computeAndSendMatrixUniforms(modelMtx,viewMtx,projMtx);
    glm::vec3 santaShoeColor(glm::vec3{0.18,0.18,0.18});
    glUniform3fv(lightingShaderUniforms.materialColor,1,&santaShoeColor[0]);
    CSCI441::drawSolidCylinder(0.32f, 0.32f, 0.4f, 10, 10);

    modelMtx = glm::translate(modelMtx, glm::vec3(1.0f,0.0f,0.0f));
    computeAndSendMatrixUniforms(modelMtx,viewMtx,projMtx);
    CSCI441::drawSolidCylinder(0.32f, 0.32f, 0.4f, 10, 10);
}

void drawSantaHead(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx){
    modelMtx = glm::translate(modelMtx, glm::vec3(10.0f, 12.0f, 10.0f));
    modelMtx = glm::scale(modelMtx, glm::vec3(5.0f, 5.0f, 5.0f));
    computeAndSendMatrixUniforms(modelMtx,viewMtx,projMtx);
    glm::vec3 santaHeadColor(glm::vec3{0.4,0.4,0.4});
    glUniform3fv(lightingShaderUniforms.materialColor,1,&santaHeadColor[0]);
    CSCI441::drawSolidCylinder(0.5f, 0.4f, 1.0, 10, 10);
}

void drawSantaHat(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx){
    modelMtx = glm::translate(modelMtx, glm::vec3(10.0f, 17.0f, 10.0f));
    modelMtx = glm::scale(modelMtx, glm::vec3(5.0f, 5.0f, 5.0f));
    computeAndSendMatrixUniforms(modelMtx,viewMtx,projMtx);
    glm::vec3 santaHatColor(glm::vec3{1.0,0.0,0.0});
    glUniform3fv(lightingShaderUniforms.materialColor,1,&santaHatColor[0]);
    CSCI441::drawSolidCone(0.45,1.0,10,10);

    glm::vec3 santaHatTrimColor(glm::vec3{1.0,1.0,1.0});
    glUniform3fv(lightingShaderUniforms.materialColor,1,&santaHatTrimColor[0]);
    CSCI441::drawSolidCylinder(0.5,0.4,0.3,10,10);

    modelMtx = glm::translate(modelMtx, glm::vec3(0.0f, 1.0f, 0.0f));
    computeAndSendMatrixUniforms(modelMtx,viewMtx,projMtx);
    glUniform3fv(lightingShaderUniforms.materialColor,1,&santaHatTrimColor[0]);
    CSCI441::drawSolidSphere(0.15,10,10);
}

// TODO santa eyes hemispheres???
void drawSantaEyes(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx){
    modelMtx = glm::translate(modelMtx, glm::vec3(9.0f, 15.0f, 9.0f));
    modelMtx = glm::scale(modelMtx, glm::vec3(5.0f, 5.0f, 5.0f));
    computeAndSendMatrixUniforms(modelMtx,viewMtx,projMtx);
    glm::vec3 santaEyeColor(glm::vec3{1.0,0.0,0.0});
    glUniform3fv(lightingShaderUniforms.materialColor,1,&santaEyeColor[0]);
    CSCI441::drawSolidSphere(0.2,10,10);

    modelMtx = glm::translate(modelMtx, glm::vec3(0.4, 0.0f, 0.0f));
    computeAndSendMatrixUniforms(modelMtx,viewMtx,projMtx);
    CSCI441::drawSolidSphere(0.2,10,10);
}

void drawEvilSanta(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) {
    drawSantaBody(modelMtx,viewMtx,projMtx);
    drawSantaBelt(modelMtx,viewMtx,projMtx);
    drawSantaBeltBuckle(modelMtx,viewMtx,projMtx);
    drawSantaShirtHem(modelMtx,viewMtx,projMtx);
    drawSantaLegs(modelMtx,viewMtx,projMtx);
    drawSantaShoes(modelMtx,viewMtx,projMtx);
    drawSantaHead(modelMtx,viewMtx,projMtx);
    drawSantaHat(modelMtx,viewMtx,projMtx);
    drawSantaEyes(modelMtx,viewMtx,projMtx);
    drawSantaLeftArm(modelMtx,viewMtx,projMtx);
    drawSantaLeftHand(modelMtx,viewMtx,projMtx);
    drawSantaRightArm(modelMtx,viewMtx,projMtx);
    drawSantaRightHand(modelMtx,viewMtx,projMtx);
    drawSantaBazookaBody(modelMtx,viewMtx,projMtx);
    drawSantaBazookaFrontEnd(modelMtx,viewMtx,projMtx);
    drawSantaBazookaBackEnd(modelMtx,viewMtx,projMtx);
    drawSantaBazookaHandle(modelMtx,viewMtx,projMtx);
    drawSantaBazookaScopeBody(modelMtx,viewMtx,projMtx);
    drawSantaBazookaScopeVisor(modelMtx,viewMtx,projMtx);
}

void drawCandles(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx){
    modelMtx = glm::translate(modelMtx, glm::vec3(10,10,10));
    computeMtxUniformsCandle(modelMtx,viewMtx,projMtx);
    glm::vec3 candleColor(glm::vec3{1.0,1.0,1.0});
    glUniform3fv(candleShaderUniforms.materialColor,1,&candleColor[0]);
    CSCI441::drawSolidSphere(5.0f,10,10);
}
void drawCandleBase(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx){
    modelMtx = glm::translate(modelMtx, glm::vec3(-10.3f, 6.0f, 10.0f));
    modelMtx = glm::scale(modelMtx, glm::vec3(2.0f, 7.0f, 2.0f));
    computeAndSendMatrixUniforms(modelMtx,viewMtx,projMtx);
    glm::vec3 santaBeltBuckleColor(glm::vec3{1.0,0.0,0.0});
    glUniform3fv(lightingShaderUniforms.materialColor,1,&santaBeltBuckleColor[0]);
    CSCI441::drawSolidCube(0.5f);
}

void drawCandleString(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx){
    modelMtx = glm::translate(modelMtx, glm::vec3(-10.3f, 9.0f, 10.0f));
    modelMtx = glm::scale(modelMtx, glm::vec3(0.5f, 1.5f, 0.5f));
    computeAndSendMatrixUniforms(modelMtx,viewMtx,projMtx);
    glm::vec3 santaBodyColor(glm::vec3{1.0,1.0,0.0});
    glUniform3fv(lightingShaderUniforms.materialColor,1,&santaBodyColor[0]);
    CSCI441::drawSolidSphere(0.8f,10,10);
}

void drawCandleModel(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx){
    drawCandleBase(modelMtx,viewMtx,projMtx);
    drawCandleString(modelMtx,viewMtx,projMtx);
}

/// Blossom drawing functions
void drawBlossomBody(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx){
    modelMtx = glm::translate(modelMtx, glm::vec3(blossomX, blossomY+1.5f, blossomZ));
    modelMtx = glm::scale(modelMtx, glm::vec3(1.5f, 1.5f, 1.5f));
    computeAndSendMatrixUniforms(modelMtx,viewMtx,projMtx);
    glm::vec3 bodyColor(glm::vec3{0.9,0.3,0.8});
    glUniform3fv(lightingShaderUniforms.materialColor,1,&bodyColor[0]);
    CSCI441::drawSolidCube(1.0f);
}

void drawBlossomHead(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx){
    modelMtx = glm::translate(modelMtx, glm::vec3(blossomX, blossomY+3.25f, blossomZ));
    modelMtx = glm::scale(modelMtx, glm::vec3(1.5f, 1.5f, 1.5f));
    computeAndSendMatrixUniforms(modelMtx,viewMtx,projMtx);
    glm::vec3 bodyColor(glm::vec3{1.0,1.0,0.9});
    glUniform3fv(lightingShaderUniforms.materialColor,1,&bodyColor[0]);
    CSCI441::drawSolidSphere(0.5f,10.0f,10.0f);
}

void drawBlossomNeck(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx){
    modelMtx = glm::translate(modelMtx, glm::vec3(blossomX,blossomY+2.5f,blossomZ));
    modelMtx = glm::scale(modelMtx, glm::vec3(1.5f, 2.0f, 1.5f));
    computeAndSendMatrixUniforms(modelMtx,viewMtx,projMtx);
    glm::vec3 bodyColor(glm::vec3{1.0,1.0,0.9});
    glUniform3fv(lightingShaderUniforms.materialColor,1,&bodyColor[0]);
    CSCI441::drawSolidCube(0.25f);
}

void drawBlossomRightArm(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx){
    modelMtx = glm::translate(modelMtx, glm::vec3(blossomX-1.0f, blossomY+1.5f, blossomZ));
    modelMtx = glm::scale(modelMtx, glm::vec3(1.5f,3.0f,1.5f));
    computeAndSendMatrixUniforms(modelMtx,viewMtx,projMtx);
    glm::vec3 bodyColor(glm::vec3{1.0,1.0,0.9});
    glUniform3fv(lightingShaderUniforms.materialColor,1,&bodyColor[0]);
    CSCI441::drawSolidCube(0.5f);
}

void drawBlossomLeftArm(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx){
    modelMtx = glm::translate(modelMtx, glm::vec3(blossomX+1.0f, blossomY+1.5f, blossomZ));
    modelMtx = glm::scale(modelMtx, glm::vec3(1.5f,3.0f,1.5f));
    computeAndSendMatrixUniforms(modelMtx,viewMtx,projMtx);
    glm::vec3 bodyColor(glm::vec3{1.0,1.0,0.9});
    glUniform3fv(lightingShaderUniforms.materialColor,1,&bodyColor[0]);
    CSCI441::drawSolidCube(0.5f);
}

void drawBlossomRightLeg(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx){
    modelMtx = glm::translate(modelMtx, glm::vec3(blossomX-0.5f, blossomY, blossomZ));
    modelMtx = glm::scale(modelMtx, glm::vec3(1.5f,3.0f,1.5f));
    computeAndSendMatrixUniforms(modelMtx,viewMtx,projMtx);
    glm::vec3 bodyColor(glm::vec3{0.5,0.5,0.0});
    glUniform3fv(lightingShaderUniforms.materialColor,1,&bodyColor[0]);
    CSCI441::drawSolidCube(0.5f);
}

void drawBlossomLeftLeg(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx){
    modelMtx = glm::translate(modelMtx, glm::vec3(blossomX+0.5f, blossomY, blossomZ));
    modelMtx = glm::scale(modelMtx, glm::vec3(1.5f,3.0f,1.5f));
    computeAndSendMatrixUniforms(modelMtx,viewMtx,projMtx);
    glm::vec3 bodyColor(glm::vec3{0.5,0.5,0.0});
    glUniform3fv(lightingShaderUniforms.materialColor,1,&bodyColor[0]);
    CSCI441::drawSolidCube(0.5f);
}

void drawBlossomNose(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx){
    modelMtx = glm::translate(modelMtx, glm::vec3(blossomX,blossomY+3.2f,blossomZ+0.5f));
    modelMtx = glm::scale(modelMtx, glm::vec3(1.5f,1.5f,1.5f));
    computeAndSendMatrixUniforms(modelMtx,viewMtx,projMtx);
    glm::vec3 bodyColor(glm::vec3{1.0,1.0,0.8});
    glUniform3fv(lightingShaderUniforms.materialColor,1,&bodyColor[0]);
    CSCI441::drawSolidSphere(0.20f,10.0f,10.0f);
}

/// Draw Blossom
void drawBlossom(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx){
    drawBlossomBody(modelMtx,viewMtx,projMtx);
    drawBlossomNeck(modelMtx,viewMtx,projMtx);
    drawBlossomRightArm(modelMtx,viewMtx,projMtx);
    drawBlossomLeftArm(modelMtx,viewMtx,projMtx);
    drawBlossomRightLeg(modelMtx,viewMtx,projMtx);
    drawBlossomLeftLeg(modelMtx,viewMtx,projMtx);
    if(blossomMoving){
        modelMtx = glm::translate(modelMtx, glm::vec3(0.0f,-0.5f,0.0f));
        drawBlossomHead(modelMtx,viewMtx,projMtx);
        drawBlossomNose(modelMtx,viewMtx,projMtx);
    }
    else{
        drawBlossomHead(modelMtx,viewMtx,projMtx);
        drawBlossomNose(modelMtx,viewMtx,projMtx);
    }
}

/// Jarrison functions
void drawJarrisonBody(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) {
    // translate and scale body
    modelMtx = glm::translate(modelMtx, glm::vec3(jarrisonX, jarrisonY, jarrisonZ));
    modelMtx = glm::scale(modelMtx, glm::vec3(1.5f, 1.5f, 5.0f));
    computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);
    // set color to turquoise
    glm::vec3 bodyColor( glm::vec3{0.117, 0.898, 0.945});
    glUniform3fv(lightingShaderUniforms.materialColor, 1, &bodyColor[0]);
    // draw oval
    CSCI441::drawSolidSphere(1.0f, 10, 10);
}

void drawJarrisonShell(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) {
    // translate and scale shell
    modelMtx = glm::translate(modelMtx, glm::vec3(jarrisonX, jarrisonY + 2.5, jarrisonZ - 1.0));
    modelMtx = glm::scale(modelMtx, glm::vec3(1.0f, 3.0f, 3.0f));
    computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);
    // set color to pink
    glm::vec3 shellColor( glm::vec3{0.941, 0.576, 0.776});
    glUniform3fv(lightingShaderUniforms.materialColor, 1, &shellColor[0]);
    // draw sphere
    CSCI441::drawSolidSphere(1.0f, 10.0f, 10.0f);
}


void drawJarrisonRightEye(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) {
    /// eye stem
    // apply constant animation
    glm::mat4 eyeStemModelMtx = glm::translate(modelMtx, glm::vec3(currentEyeOffset, 0.0f, 0.0f));
    // translate eye stem
    eyeStemModelMtx = glm::translate(eyeStemModelMtx, glm::vec3(jarrisonX - 0.75f, jarrisonY, jarrisonZ + 2.3f));
    computeAndSendMatrixUniforms(eyeStemModelMtx, viewMtx, projMtx);
    // set eye stem color
    glm::vec3 leftEyeStemColor( glm::vec3{0.117, 0.898, 0.945});
    glUniform3fv(lightingShaderUniforms.materialColor, 1, &leftEyeStemColor[0]);
    CSCI441::drawSolidCylinder(0.3f, 0.3f, 4.0f, 10.0f, 10.0f);

    /// eyeball
    glm::mat4 eyeballModelMtx = glm::translate(modelMtx, glm::vec3(currentEyeOffset, 0.0f, 0.0f));
    eyeballModelMtx = glm::translate(eyeballModelMtx, glm::vec3(jarrisonX - 0.75f, jarrisonY + 4.0f, jarrisonZ + 2.3f));
    computeAndSendMatrixUniforms(eyeballModelMtx, viewMtx, projMtx);
    // set color to white
    glm::vec3 eyeballColor( glm::vec3{1, 1, 1});
    glUniform3fv(lightingShaderUniforms.materialColor, 1, &eyeballColor[0]);
    CSCI441::drawSolidSphere(0.6f, 10.0f, 10.0f);

    /// pupil
    glm::mat4 pupilModelMtx = glm::translate(modelMtx, glm::vec3(currentEyeOffset, 0.0f, 0.0f));
    pupilModelMtx = glm::translate(pupilModelMtx, glm::vec3(jarrisonX - 0.75f, jarrisonY + 4.0f, jarrisonZ + 2.8f));
    computeAndSendMatrixUniforms(pupilModelMtx, viewMtx, projMtx);
    // set color to black
    glm::vec3 pupilColor( glm::vec3{0, 0, 0});
    glUniform3fv(lightingShaderUniforms.materialColor, 1, &pupilColor[0]);
    // draw sphere
    CSCI441::drawSolidSphere(0.2f, 10.0f, 10.0f);
}

void drawJarrisonLeftEye(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) {
    /// eye stem
    // apply constant animation
    glm::mat4 eyeStemModelMtx = glm::translate(modelMtx, glm::vec3(-currentEyeOffset, 0.0f, 0.0f));
    // translate eye stem
    eyeStemModelMtx = glm::translate(eyeStemModelMtx, glm::vec3(jarrisonX + 0.75f, jarrisonY, jarrisonZ + 2.3f));
    computeAndSendMatrixUniforms(eyeStemModelMtx, viewMtx, projMtx);
    // set eye stem color
    glm::vec3 leftEyeStemColor( glm::vec3{0.117, 0.898, 0.945});
    glUniform3fv(lightingShaderUniforms.materialColor, 1, &leftEyeStemColor[0]);
    CSCI441::drawSolidCylinder(0.3f, 0.3f, 4.0f, 10.0f, 10.0f);

    /// eyeball
    glm::mat4 eyeballModelMtx = glm::translate(modelMtx, glm::vec3(-currentEyeOffset, 0.0f, 0.0f));
    eyeballModelMtx = glm::translate(eyeballModelMtx, glm::vec3(jarrisonX + 0.75f, jarrisonY + 4.0f, jarrisonZ + 2.3f));
    computeAndSendMatrixUniforms(eyeballModelMtx, viewMtx, projMtx);
    // set color to white
    glm::vec3 eyeballColor( glm::vec3{1, 1, 1});
    glUniform3fv(lightingShaderUniforms.materialColor, 1, &eyeballColor[0]);
    CSCI441::drawSolidSphere(0.6f, 10.0f, 10.0f);

    /// pupil
    glm::mat4 pupilModelMtx = glm::translate(modelMtx, glm::vec3(-currentEyeOffset, 0.0f, 0.0f));
    pupilModelMtx = glm::translate(pupilModelMtx, glm::vec3(jarrisonX + 0.75f, jarrisonY + 4.0f, jarrisonZ + 2.8f));
    computeAndSendMatrixUniforms(pupilModelMtx, viewMtx, projMtx);
    // set color to black
    glm::vec3 pupilColor( glm::vec3{0, 0, 0});
    glUniform3fv(lightingShaderUniforms.materialColor, 1, &pupilColor[0]);
    // draw sphere
    CSCI441::drawSolidSphere(0.2f, 10.0f, 10.0f);
}

void drawJarrisonMouth(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) {
    // translate and scale mouth
    modelMtx = glm::translate(modelMtx, glm::vec3(jarrisonX, jarrisonY, jarrisonZ + 4.6));
    modelMtx = glm::scale(modelMtx, glm::vec3(1.0f, 0.4f, 0.3f));
    computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);
    // set color to black
    glm::vec3 mouthColor( glm::vec3{0, 0, 0});
    glUniform3fv(lightingShaderUniforms.materialColor, 1, &mouthColor[0]);
    // draw sphere
    CSCI441::drawSolidSphere(1.0f, 10.0f, 10.0f);
}

void drawJarrison( glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) {
    // apply movement
    drawJarrisonBody(modelMtx, viewMtx, projMtx);
    drawJarrisonShell(modelMtx, viewMtx, projMtx);
    // apply constant animation
    drawJarrisonRightEye(modelMtx, viewMtx, projMtx);
    drawJarrisonLeftEye(modelMtx, viewMtx, projMtx);
    if (jarrisonMoving) drawJarrisonMouth(modelMtx, viewMtx, projMtx);
}

/// Voltorb functions
void drawVoltorbRightEye(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx){
    //right eye

    glm::mat4 rightEyeModelMtx(1.0f);

    if(playerMoving){
        glm::vec3 bodyColor3( glm::vec3{0.0f,0.0f,1.0f});
        glUniform3fv(lightingShaderUniforms.materialColor, 1, &bodyColor3[0]);
    }else{
        glm::vec3 bodyColor3( glm::vec3{1.0f,1.0f,1.0f});
        glUniform3fv(lightingShaderUniforms.materialColor, 1, &bodyColor3[0]);
    }


    //put it in the correct place
    rightEyeModelMtx = glm::scale(modelMtx, glm::vec3(0.4, 0.4, 0.4) );
    rightEyeModelMtx = glm::translate( rightEyeModelMtx, glm::vec3( playerPos.x+4, playerPos.y+5, playerPos.z-4 ) );
    rightEyeModelMtx = glm::rotate(rightEyeModelMtx, 2.5f, glm::vec3(1.0f, 1.0f, 1.0f));

    computeAndSendMatrixUniforms(rightEyeModelMtx, viewMtx, projMtx);

    CSCI441::drawSolidCone(2.0f, 4.0f, 1, 3);
}
void drawVoltorbLeftEye(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx){
    glm::mat4 leftEyeModelMtx(1.0f);

    if(playerMoving){
        glm::vec3 bodyColor3( glm::vec3{0.0f,0.0f,1.0f});
        glUniform3fv(lightingShaderUniforms.materialColor, 1, &bodyColor3[0]);
    }else{
        glm::vec3 bodyColor3( glm::vec3{1.0f,1.0f,1.0f});
        glUniform3fv(lightingShaderUniforms.materialColor, 1, &bodyColor3[0]);
    }

    //put it in the correct place
    leftEyeModelMtx = glm::scale(modelMtx, glm::vec3(0.4, 0.4, 0.4) );
    leftEyeModelMtx = glm::translate( leftEyeModelMtx, glm::vec3( playerPos.x+4, playerPos.y+5, playerPos.z+4 ) );
    leftEyeModelMtx = glm::rotate(leftEyeModelMtx, 2.5f, glm::vec3(1.0f, 1.0f, 1.0f));
    leftEyeModelMtx = glm::rotate(leftEyeModelMtx, 2.7f, glm::vec3(1.0f, 0.0f, 0.0f));
    leftEyeModelMtx = glm::rotate(leftEyeModelMtx, 6.8f, glm::vec3(0.0f, 0.0f, 1.0f));

    computeAndSendMatrixUniforms(leftEyeModelMtx, viewMtx, projMtx);

    CSCI441::drawSolidCone(2.0f, 4.0f, 1, 3);
}
void drawVoltorbLeftPupil(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) {
    glm::mat4 leftPupilModelMtx(1.0f);

    leftPupilModelMtx = glm::scale(modelMtx, glm::vec3(0.2, 0.4, 0.2) );
    leftPupilModelMtx = glm::translate( leftPupilModelMtx, glm::vec3( playerPos.x+9, playerPos.y+6, playerPos.z+6 ) );

    computeAndSendMatrixUniforms(leftPupilModelMtx, viewMtx, projMtx);

    CSCI441::drawSolidSphere(1.0f, 10, 10);
}
void drawVoltorbRightPupil(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) {
    glm::mat4 rightPupilModelMtx(1.0f);

    glm::vec3 bodyColor3( glm::vec3{0.0f,0.0f,0.0f});
    glUniform3fv(lightingShaderUniforms.materialColor, 1, &bodyColor3[0]);

    //need to make this black and in the right place and also a left pupil
    rightPupilModelMtx = glm::scale(modelMtx, glm::vec3(0.2, 0.4, 0.2) );
    rightPupilModelMtx = glm::translate( rightPupilModelMtx, glm::vec3( playerPos.x+9, playerPos.y+6, playerPos.z-6 ) );

    computeAndSendMatrixUniforms(rightPupilModelMtx, viewMtx, projMtx);

    CSCI441::drawSolidSphere(1.0f, 10, 10);

}
void drawVoltorbBody(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) {
    glm::mat4 topHemisphereModelMtx(1.0f);

    //make sure to include the file C:\MinGW\include\CSCI441\Vlad_A2\include\CSCI441\objects.hpp
    topHemisphereModelMtx = glm::translate( modelMtx, glm::vec3( playerPos.x, playerPos.y, playerPos.z ) );

    computeAndSendMatrixUniforms(topHemisphereModelMtx, viewMtx, projMtx);

}
void drawVoltorb(glm::mat4 modelMtx,glm::mat4 viewMtx,glm::mat4 projectMtx){
    // voltorb body
    drawVoltorbBody(modelMtx, viewMtx, projectMtx);
    //we still need the eyes
    //right eye
    drawVoltorbRightEye(modelMtx, viewMtx, projectMtx);
    //left eye
    drawVoltorbLeftEye(modelMtx, viewMtx, projectMtx);
    //right pupil
    drawVoltorbRightPupil(modelMtx, viewMtx, projectMtx);
    //left pupil
    drawVoltorbLeftPupil(modelMtx, viewMtx, projectMtx);
}

void drawTreeTopper(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) {
    modelMtx = glm::translate(modelMtx, glm::vec3(-35.0f, 45.0f, -25.0f));
    computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);
    glUniform3fv(treeTopperShaderProgramUniforms.topperColor, 1, &topperColor[0]);
    CSCI441::drawSolidSphere(5.0f, 10, 10);
}

void drawRedOrnamentTop(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) {
    modelMtx = glm::translate(modelMtx, glm::vec3(0.3f, 9.8f, 10.0f));
    modelMtx = glm::scale(modelMtx, glm::vec3(1.0f, 1.0f, 1.0f));
    computeAndSendMatrixUniforms(modelMtx,viewMtx,projMtx);
    glm::vec3 santaBeltBuckleColor(glm::vec3{0.5,0.5,0.5});
    glUniform3fv(lightingShaderUniforms.materialColor,1,&santaBeltBuckleColor[0]);
    CSCI441::drawSolidCube(0.5f);
}

void drawRedOrnamentSphere(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) {
    modelMtx = glm::translate(modelMtx, glm::vec3(0.3f, 9.0f, 10.0f));
    modelMtx = glm::scale(modelMtx, glm::vec3(1.0f, 1.0f, 1.0f));
    computeAndSendMatrixUniforms(modelMtx,viewMtx,projMtx);
    glm::vec3 santaBodyColor(glm::vec3{1.0,0.0,0.0});
    glUniform3fv(lightingShaderUniforms.materialColor,1,&santaBodyColor[0]);
    CSCI441::drawSolidSphere(0.8f,10,10);
}
void drawRedOrnament(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) {
    drawRedOrnamentTop(modelMtx,viewMtx,projMtx);
    drawRedOrnamentSphere(modelMtx,viewMtx,projMtx);
}

void drawGreenOrnamentTop(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) {
    modelMtx = glm::translate(modelMtx, glm::vec3(0.3f, 9.8f, 10.0f));
    modelMtx = glm::scale(modelMtx, glm::vec3(1.0f, 1.0f, 1.0f));
    computeAndSendMatrixUniforms(modelMtx,viewMtx,projMtx);
    glm::vec3 santaBeltBuckleColor(glm::vec3{1.5,1.0,0.0});
    glUniform3fv(lightingShaderUniforms.materialColor,1,&santaBeltBuckleColor[0]);
    CSCI441::drawSolidCube(0.5f);
}

void drawGreenOrnamentSphere(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) {
    modelMtx = glm::translate(modelMtx, glm::vec3(0.3f, 9.0f, 10.0f));
    modelMtx = glm::scale(modelMtx, glm::vec3(1.0f, 1.0f, 1.0f));
    computeAndSendMatrixUniforms(modelMtx,viewMtx,projMtx);
    glm::vec3 santaBodyColor(glm::vec3{0.0,1.0,0.0});
    glUniform3fv(lightingShaderUniforms.materialColor,1,&santaBodyColor[0]);
    CSCI441::drawSolidSphere(0.8f,10,10);
}
void drawGreenOrnament(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) {
    drawGreenOrnamentTop(modelMtx,viewMtx,projMtx);
    drawGreenOrnamentSphere(modelMtx,viewMtx,projMtx);
}

void transposeFlake(glm::vec3 landingPosition, glm::mat4 viewMtx, glm::mat4 projMtx) {
    // TODO transpose flake
}


// renderScene() ///////////////////////////////////////////////////////////////
void renderScene(glm::mat4 viewMtx, glm::mat4 projMtx) {
    // draw our grid
    lightingShader->useProgram();
    glUniform3fv(lightingShaderUniforms.camPosition,1,&camPos[0]);

    //// BEGIN DRAWING THE GROUND PLANE ////
    // draw the ground plane
    glm::mat4 groundModelMtx = glm::scale( glm::mat4(1.0f), glm::vec3(80.0f,1.0f,80.0f));
    computeAndSendMatrixUniforms(groundModelMtx, viewMtx, projMtx);

    glm::vec3 groundColor(1.0f,1.0f,1.0f);
    glUniform3fv(lightingShaderUniforms.materialColor, 1, &groundColor[0]);

    glBindVertexArray(groundVAO);
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, (void*)0);
    //// END DRAWING THE GROUND PLANE ////

    /************************************** sky box drawing (each plane) **********************************************/

    /////////////////////////////////////////////////////// left //////////////////////////////////////////////////////////////
    texShaderProgram->useProgram();

    computeAndSendTransformationMatrices6(glm::mat4(1.0f), viewMtx, projMtx, texShaderProgramUniforms.mvpMatrix);
    glBindTexture(GL_TEXTURE_2D,leftTextureHandle);

    glBindVertexArray( leftVAO );
    glDrawElements( GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, (void*)0 );
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////// right /////////////////////////////////////////////////////////////////
    computeAndSendTransformationMatrices6(glm::mat4(1.0f), viewMtx, projMtx, texShaderProgramUniforms.mvpMatrix);
    glBindTexture(GL_TEXTURE_2D,rightTextureHandle);

    glBindVertexArray( rightVAO );
    glDrawElements( GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, (void*)0 );
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /////////////////////////////////////////////////////// back //////////////////////////////////////////////////////////////
    computeAndSendTransformationMatrices6(glm::mat4(1.0f), viewMtx, projMtx, texShaderProgramUniforms.mvpMatrix);
    glBindTexture(GL_TEXTURE_2D,backTextureHandle);

    glBindVertexArray( backVAO );
    glDrawElements( GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, (void*)0 );
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /////////////////////////////////////////////////////// front //////////////////////////////////////////////////////////////
    computeAndSendTransformationMatrices6(glm::mat4(1.0f), viewMtx, projMtx, texShaderProgramUniforms.mvpMatrix);
    glBindTexture(GL_TEXTURE_2D,frontTextureHandle);

    glBindVertexArray( frontVAO );
    glDrawElements( GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, (void*)0 );
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /////////////////////////////////////////////////////// top //////////////////////////////////////////////////////////////
    computeAndSendTransformationMatrices6(glm::mat4(1.0f), viewMtx, projMtx, texShaderProgramUniforms.mvpMatrix);
    glBindTexture(GL_TEXTURE_2D,topTextureHandle);

    glBindVertexArray( topVAO );
    glDrawElements( GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, (void*)0 );
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    lightingShader->useProgram();

    // TODO Evil Santa
    /// render scene
    glm::mat4 santaModelMtx(1.0f);
    santaModelMtx = glm::translate(glm::mat4(1.0f), glm::vec3(santaX,0.0f,santaZ));
    drawEvilSanta(santaModelMtx, viewMtx, projMtx);

    glm::mat4 redOrnamentModelMtx(1.0f);
    glm::mat4 greenOrnamentModelMtx(1.0f);
    drawRedOrnament(redOrnamentModelMtx, viewMtx, projMtx);

    greenOrnamentModelMtx = glm::translate(greenOrnamentModelMtx, glm::vec3(0.3f, 13.0f, 10.0f));
    drawGreenOrnament(greenOrnamentModelMtx, viewMtx, projMtx);

    glm::mat4 candleModelModelMtx(1.0f);
    drawCandleModel(candleModelModelMtx, viewMtx, projMtx);

    /// draw jarrison
    glm::mat4 jarrisonModelMtx(1.0f);
    jarrisonModelMtx = glm::translate( glm::mat4(1.0), glm::vec3( jarrisonX, 0.0f, jarrisonZ ) );
    jarrisonModelMtx = glm::rotate(jarrisonModelMtx, jarrisonRadians , glm::vec3(0.0f, 1.0f, 0.0f));
    jarrisonModelMtx = glm::translate( jarrisonModelMtx, glm::vec3( -jarrisonX, 0.0f, -jarrisonZ ) );
    drawJarrison(jarrisonModelMtx, viewMtx, projMtx);

    /// draw voltorb
    /*glm::mat4 voltorbModelMtx(1.0f);
    voltorbModelMtx = glm::translate( glm::mat4(1.0f), glm::vec3( playerMove.x, playerMove.y, playerMove.z ) );
    voltorbModelMtx = glm::rotate(voltorbModelMtx, -rotAngle, glm::vec3(0.0f, 1.0f, 0.0f));
    drawVoltorb(voltorbModelMtx, viewMtx, projMtx);*/

    /// draw blossom
    glm::mat4 drawBlossomModelMtx(1.0f);
    drawBlossomModelMtx = glm::translate(glm::mat4(1.0f), glm::vec3(blossomX, 0.0f, blossomZ));
    drawBlossomModelMtx = glm::rotate(drawBlossomModelMtx, blossomRotation, glm::vec3(0.0f,1.0f,0.0f));
    drawBlossomModelMtx = glm::translate(drawBlossomModelMtx, glm::vec3(-blossomX,0.0f,-blossomZ));
    /// drawBlossom(drawBlossomModelMtx,viewMtx,projMtx);

    // TODO Christmas Tree
    drawChristmasTree(christmasTree, viewMtx, projMtx);
    drawChristmasTreeTrunk(christmasTrunk, viewMtx, projMtx);

    // TODO candy canes
    for(CandyCanes c: candyCanes){
        drawCandyCane(c , viewMtx, projMtx);
    }

    candleShader->useProgram();
    glUniform3fv(candleShaderUniforms.camPos,1,&camPos[0]);
    glm::mat4 candleModelMtx = glm::mat4(1.0f);
    drawCandles(candleModelMtx,viewMtx,projMtx);

    lightingShader->useProgram();

    glm::mat4 modelMatrix = glm::mat4( 1.0f );

    // LOOKHERE #3
    billboardShaderProgram->useProgram();

    computeAndSendTransformationMatrices5( modelMatrix, viewMtx, projMtx,
                                          billboardShaderProgramUniforms.mvMatrix, billboardShaderProgramUniforms.projMatrix);
    glBindVertexArray( vaos[VAOS.PARTICLE_SYSTEM] );
    glBindVertexArray( vaos[VAOS.PARTICLE_SYSTEM] );
    glBindTexture(GL_TEXTURE_2D, spriteTextureHandle);

    // TODO #1
    for(int i = 0; i < NUM_SPRITES; i++){

        if(spriteLocations[ spriteIndices[i]].y <= 0){
            // TODO transpose flake
            glm::vec3 landingPosition = spriteLocations[ spriteIndices[i]];
            transposeFlake(landingPosition, viewMtx, projMtx);
            spriteLocations[ spriteIndices[i]].y += MAX_BOX_SIZE;
            // TODO move flake a lil'
        }
        else {
            spriteLocations[ spriteIndices[i]].y -= 0.05;
        }
        glm::vec3 currentSprite = spriteLocations[ spriteIndices[i] ];
        glm::vec4 p = modelMatrix * glm::vec4(currentSprite, 1.0f);
        glm::vec4 eyePoints = p - glm::vec4(camPos, 1.0f);
        float dist = dot(glm::vec4(arcballLookAtPoint - camPos,1), eyePoints);
        distances[i] = dist;
    }

    // TODO #2
    for(int i = 0; i < NUM_SPRITES - 1; i++){
        for(int j = 1; j < NUM_SPRITES; j++){
            if (distances[j - 1] > distances[j]){
                float temporary = distances[j];
                distances[j] = distances[j - 1];
                distances[j - 1] = temporary;

                int neat = spriteIndices[j];
                spriteIndices[j] = spriteIndices[j - 1];
                spriteIndices[j - 1] = neat;
            }
        }
    }

    glBindVertexArray( vaos[VAOS.PARTICLE_SYSTEM] );

    glBindBuffer( GL_ARRAY_BUFFER, vbos[VAOS.PARTICLE_SYSTEM] );
    glBufferData( GL_ARRAY_BUFFER, NUM_SPRITES * sizeof(glm::vec3), spriteLocations, GL_STATIC_DRAW );

    glEnableVertexAttribArray( billboardShaderProgramAttributes.vPos );
    glVertexAttribPointer( billboardShaderProgramAttributes.vPos, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0 );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibos[VAOS.PARTICLE_SYSTEM] );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, NUM_SPRITES * sizeof(GLushort), spriteIndices, GL_STATIC_DRAW );

    // TODO #3
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibos[VAOS.PARTICLE_SYSTEM] );
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLushort) * NUM_SPRITES, spriteIndices);

    glDrawElements( GL_POINTS, NUM_SPRITES, GL_UNSIGNED_SHORT, (void*)0 );

    treeTopperShaderProgram->useProgram();
    drawTreeTopper(modelMatrix, viewMtx, projMtx);

    lightingShader->useProgram();
}

// make jarrisons eyes move
void updateJarrisonAnimation() {
    if (eyesMovingInward) {
        if (currentEyeOffset >= MAX_EYE_OFFSET) {
            eyesMovingInward = false;
        } else {
            currentEyeOffset += 0.03;
        }
    } else {
        if (currentEyeOffset <= -1 * MAX_EYE_OFFSET) {
            eyesMovingInward = true;
        } else {
            currentEyeOffset -= 0.03;
        }
    }
}

void updateSantaDirection(){
    if(santaMoveRight){
        if (santaX < 65){
            santaX += 0.75;
        }
        else {
            santaMoveRight = false;
            santaX -= 0.75;
        }
    }
    else {
        if (santaX > -65){
            santaX -= 0.75;
        }
        else {
            santaMoveRight = true;
            santaX += 0.75;
        }
    }
}

//*************************************************************************************
// Setup Functions

GLFWwindow* setupGLFW() {
    // set what function to use when registering errors
    // this is the ONLY GLFW function that can be called BEFORE GLFW is initialized
    // all other GLFW calls must be performed after GLFW has been initialized
    glfwSetErrorCallback( error_callback );

    // initialize GLFW
    if( !glfwInit() ) {
        fprintf( stderr, "[ERROR]: Could not initialize GLFW\n" );
        exit( EXIT_FAILURE );
    } else {
        fprintf( stdout, "[INFO]: GLFW initialized\n" );
    }

    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );  // request forward compatible OpenGL context
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );        // request OpenGL Core Profile context
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );	// request OpenGL v4.X
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 1 );	// request OpenGL vX.1
    glfwWindowHint( GLFW_RESIZABLE, GLFW_FALSE );		    // do not allow our window to be able to be resized
    glfwWindowHint( GLFW_DOUBLEBUFFER, GLFW_TRUE );         // request double buffering

    // create a window for a given size, with a given title
    GLFWwindow *window = glfwCreateWindow( WINDOW_WIDTH, WINDOW_HEIGHT, "A3", nullptr, nullptr );
    if( !window ) {						// if the window could not be created, NULL is returned
        fprintf( stderr, "[ERROR]: GLFW Window could not be created\n" );
        glfwTerminate();
        exit( EXIT_FAILURE );
    } else {
        fprintf( stdout, "[INFO]: GLFW Window created\n" );
    }

    glfwMakeContextCurrent(window);		                    // make the created window the current window
    glfwSwapInterval(1);				     	    // update our screen after at least 1 screen refresh

    glfwSetKeyCallback( window, keyboard_callback );		// set our keyboard callback function
    glfwSetCursorPosCallback( window, cursor_callback );	// set our cursor position callback function
    glfwSetMouseButtonCallback( window, mouse_button_callback );	// set our mouse button callback function

    return window;						                     // return the window that was created
}

void setupShaders() {
    lightingShader = new CSCI441::ShaderProgram( "shaders/midterm.v.glsl", "shaders/midterm.f.glsl" );
    lightingShaderUniforms.mvpMatrix      = lightingShader->getUniformLocation("mvpMatrix");
    // TODO #3 assign the uniform and attribute locations
    lightingShaderUniforms.materialColor  = lightingShader->getUniformLocation("materialColor");
    lightingShaderUniforms.normalMatrix   = lightingShader->getUniformLocation("normalMatrix");
    lightingShaderUniforms.lightDirection = lightingShader->getUniformLocation("lightDirection");
    lightingShaderUniforms.lightPosition = lightingShader->getUniformLocation("lightPosition");
    lightingShaderUniforms.lightColor     = lightingShader->getUniformLocation("lightColor");
    lightingShaderUniforms.pointLightColor     = lightingShader->getUniformLocation("pointLightColor");
    lightingShaderAttributes.vPos         = lightingShader->getAttributeLocation("vPos");
    lightingShaderAttributes.vertexNormal = lightingShader->getAttributeLocation("vertexNormal");
    lightingShaderUniforms.camPosition         = lightingShader->getUniformLocation("camPosition");

    lightingShaderUniforms.spotLightTheta   = lightingShader->getUniformLocation("spotLightTheta");
    lightingShaderUniforms.spotLightPosition = lightingShader->getUniformLocation("spotLightPosition");
    lightingShaderUniforms.spotLightDirection = lightingShader->getUniformLocation("spotLightDirection");
    lightingShaderUniforms.spotLightColor     = lightingShader->getUniformLocation("spotLightColor");

    billboardShaderProgram = new CSCI441::ShaderProgram( "shaders/billboardQuadShader.v.glsl",
                                                         "shaders/billboardQuadShader.g.glsl",
                                                         "shaders/billboardQuadShader.f.glsl" );
    billboardShaderProgramUniforms.mvMatrix = billboardShaderProgram->getUniformLocation( "mvMatrix");
    billboardShaderProgramUniforms.projMatrix = billboardShaderProgram->getUniformLocation( "projMatrix");
    billboardShaderProgramUniforms.image = billboardShaderProgram->getUniformLocation( "image");
    billboardShaderProgramAttributes.vPos = billboardShaderProgram->getAttributeLocation( "vPos");

    billboardShaderProgram->useProgram();
    glUniform1i(billboardShaderProgramUniforms.image, 0);

    texShaderProgram = new CSCI441::ShaderProgram("shaders/texShader.v.glsl","shaders/texShader.f.glsl");
    texShaderProgramUniforms.mvpMatrix = texShaderProgram->getUniformLocation("mvpMatrix");
    texShaderProgramUniforms.textureMap = texShaderProgram->getUniformLocation("textureMap");
    texShaderProgramAttributes.vPos = texShaderProgram->getAttributeLocation("vPos");
    texShaderProgramAttributes.vTexCoord = texShaderProgram->getAttributeLocation("vTexCoord");
    glUniform1i(texShaderProgramUniforms.textureMap,0);

    treeTopperShaderProgram = new CSCI441::ShaderProgram("shaders/treeTopper.v.glsl", "shaders/treeTopper.f.glsl");
    treeTopperShaderProgramUniforms.mvpMatrix = treeTopperShaderProgram->getUniformLocation("mvpMatrix");
    treeTopperShaderProgramUniforms.topperColor = treeTopperShaderProgram->getUniformLocation("topperColor");
    treeTopperShaderProgramAttributes.vPos = treeTopperShaderProgram->getAttributeLocation("vPos");

    candleShader = new CSCI441::ShaderProgram("shaders/candleShader.v.glsl", "shaders/candleShader.f.glsl");
    candleShaderUniforms.mvpMatrix = candleShader->getUniformLocation("mvpMtx");
    candleShaderUniforms.normalMtx = candleShader->getUniformLocation("normalMtx");
/*
    candleShaderUniforms.modelMtx = candleShader->getUniformLocation("modelMtx");
*/

    candleShaderUniforms.lightPositionPoint = candleShader->getUniformLocation("lightPositionPoint");
    candleShaderUniforms.lightColorPoint = candleShader->getUniformLocation("lightColorPoint");
    candleShaderUniforms.lightDirection = candleShader->getUniformLocation("lightDirection");

    candleShaderUniforms.materialColor  = candleShader->getUniformLocation("materialColor");
    candleShaderUniforms.camPos = candleShader->getUniformLocation("camPos");


    candleShaderAttributes.vPos = candleShader->getAttributeLocation("vPos");
    candleShaderAttributes.vNormal = candleShader->getAttributeLocation("vNormal");
}

void setupBuffers() {

    /// ground
    struct VertexNormal {
        GLfloat x, y, z;
        GLfloat xNorm, yNorm, zNorm;
    };

    VertexNormal groundQuad[4] = {
            {-1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f},
            {1.0f,  0.0f, -1.0f, 0.0f, 1.0f, 0.0f},
            {-1.0f, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f},
            {1.0f,  0.0f, 1.0f,  0.0f, 1.0f, 0.0f}
    };

    GLushort indices[4] = {0, 1, 2, 3};

    /// sprite
    glGenVertexArrays(NUM_VAOS, vaos);
    glGenBuffers(NUM_VAOS, vbos);
    glGenBuffers(NUM_VAOS, ibos);

    // --------------------------------------------------------------------------------------------------
    // LOOKHERE #2 - generate sprites

    spriteLocations = (glm::vec3 *) malloc(sizeof(glm::vec3) * NUM_SPRITES);
    spriteIndices = (GLushort *) malloc(sizeof(GLushort) * NUM_SPRITES);
    distances = (GLfloat *) malloc(sizeof(GLfloat) * NUM_SPRITES);
    for (int i = 0; i < NUM_SPRITES; i++) {
        glm::vec3 pos(randNumber(MAX_BOX_SIZE), randNumber(NEW_BOX_SIZE), randNumber(MAX_BOX_SIZE));
        spriteLocations[i] = pos;
        spriteIndices[i] = i;
    }

    glBindVertexArray(vaos[VAOS.PARTICLE_SYSTEM]);

    glBindBuffer(GL_ARRAY_BUFFER, vbos[VAOS.PARTICLE_SYSTEM]);
    glBufferData(GL_ARRAY_BUFFER, NUM_SPRITES * sizeof(glm::vec3), spriteLocations, GL_STATIC_DRAW);

    glEnableVertexAttribArray(billboardShaderProgramAttributes.vPos);
    glVertexAttribPointer(billboardShaderProgramAttributes.vPos, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibos[VAOS.PARTICLE_SYSTEM]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, NUM_SPRITES * sizeof(GLushort), spriteIndices, GL_STATIC_DRAW);

    fprintf(stdout, "[INFO]: point sprites read in with VAO/VBO/IBO %d/%d/%d\n", vaos[VAOS.PARTICLE_SYSTEM],
            vbos[VAOS.PARTICLE_SYSTEM], ibos[VAOS.PARTICLE_SYSTEM]);
    /// end sprite

    ////////////////////////////////////
    glEnableVertexAttribArray(candleShaderAttributes.vPos);
    glVertexAttribPointer(candleShaderAttributes.vPos, 3, GL_FLOAT, GL_FALSE, sizeof(VertexNormal), (void*)0);

    glEnableVertexAttribArray(candleShaderAttributes.vNormal);
    glVertexAttribPointer(candleShaderAttributes.vNormal, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    /////////////////////////////////

    /// send tree topper vPos
    glEnableVertexAttribArray(treeTopperShaderProgramAttributes.vPos);
    glVertexAttribPointer(treeTopperShaderProgramAttributes.vPos, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);

    glGenVertexArrays(1, &groundVAO);
    glBindVertexArray(groundVAO);

    GLuint vbods[2];       // 0 - VBO, 1 - IBO
    glGenBuffers(2, vbods);
    glBindBuffer(GL_ARRAY_BUFFER, vbods[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(groundQuad), groundQuad, GL_STATIC_DRAW);

    glEnableVertexAttribArray(lightingShaderAttributes.vPos);
    glVertexAttribPointer(lightingShaderAttributes.vPos, 3, GL_FLOAT, GL_FALSE, sizeof(VertexNormal), (void *) 0);

    glEnableVertexAttribArray(lightingShaderAttributes.vertexNormal);
    glVertexAttribPointer(lightingShaderAttributes.vertexNormal, 3, GL_FLOAT, GL_FALSE, sizeof(VertexNormal),
                          (void *) (3 * sizeof(float)));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbods[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    ////////////////////////////////////////////////// left plane /////////////////////////////////////////////////////

    struct VertexTextured { /// struct for drawing each plane of our skybox
        float x, y, z;
        float s, t;
    };

    VertexTextured leftTexVertices[4] = {
            {quadSize, 0.0f,     -quadSize, 0.0f, 0.0f},
            {quadSize, 0.0f,     quadSize,  1.0f, 0.0f},
            {quadSize, quadSize, -quadSize, 0.0f, 1.0f},
            {quadSize, quadSize, quadSize,  1.0f, 1.0f}
    };
    unsigned short leftTexIndices[4] = {0, 1, 2, 3};

    glGenVertexArrays(1, &leftVAO);
    glBindVertexArray(leftVAO);

    glGenBuffers(2, leftVBO);

    glBindBuffer(GL_ARRAY_BUFFER, leftVBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(leftTexVertices), leftTexVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(texShaderProgramAttributes.vPos);
    glVertexAttribPointer(texShaderProgramAttributes.vPos, 3, GL_FLOAT, GL_FALSE, sizeof(VertexTextured),
                          (void *) 0);

    glEnableVertexAttribArray(texShaderProgramAttributes.vTexCoord);
    glVertexAttribPointer(texShaderProgramAttributes.vTexCoord, 2, GL_FLOAT, GL_FALSE, sizeof(VertexTextured),
                          (void *) (sizeof(float) * 3));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, leftVBO[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(leftTexIndices), leftTexIndices, GL_STATIC_DRAW);

    fprintf(stdout, "[INFO]: platform read in with VAO %d\n", leftVAO);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////// right plane /////////////////////////////////////////////////////
    VertexTextured rightTexVertices[4] = {
            {-quadSize, 0.0f,     quadSize,  0.0f, 0.0f},
            {-quadSize, 0.0f,     -quadSize, 1.0f, 0.0f},
            {-quadSize, quadSize, quadSize,  0.0f, 1.0f},
            {-quadSize, quadSize, -quadSize, 1.0f, 1.0f}
    };
    unsigned short rightTexIndices[4] = {0, 1, 2, 3};

    glGenVertexArrays(1, &rightVAO);
    glBindVertexArray(rightVAO);

    glGenBuffers(2, rightVBO);

    glBindBuffer(GL_ARRAY_BUFFER, rightVBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rightTexVertices), rightTexVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(texShaderProgramAttributes.vPos);
    glVertexAttribPointer(texShaderProgramAttributes.vPos, 3, GL_FLOAT, GL_FALSE, sizeof(VertexTextured),
                          (void *) 0);

    glEnableVertexAttribArray(texShaderProgramAttributes.vTexCoord);
    glVertexAttribPointer(texShaderProgramAttributes.vTexCoord, 2, GL_FLOAT, GL_FALSE, sizeof(VertexTextured),
                          (void *) (sizeof(float) * 3));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rightVBO[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rightTexIndices), rightTexIndices, GL_STATIC_DRAW);

    fprintf(stdout, "[INFO]: platform read in with VAO %d\n", rightVAO);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////// back plane /////////////////////////////////////////////////////
    VertexTextured backTexVertices[4] = {
            {-quadSize, 0.0f,     -quadSize, 0.0f, 0.0f},
            {quadSize,  0.0f,     -quadSize, 1.0f, 0.0f},
            {-quadSize, quadSize, -quadSize, 0.0f, 1.0f},
            {quadSize,  quadSize, -quadSize, 1.0f, 1.0f}
    };
    unsigned short backTexIndices[4] = {0, 1, 2, 3};

    glGenVertexArrays(1, &backVAO);
    glBindVertexArray(backVAO);

    glGenBuffers(2, backVBO);

    glBindBuffer(GL_ARRAY_BUFFER, backVBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(backTexVertices), backTexVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(texShaderProgramAttributes.vPos);
    glVertexAttribPointer(texShaderProgramAttributes.vPos, 3, GL_FLOAT, GL_FALSE, sizeof(VertexTextured),
                          (void *) 0);

    glEnableVertexAttribArray(texShaderProgramAttributes.vTexCoord);
    glVertexAttribPointer(texShaderProgramAttributes.vTexCoord, 2, GL_FLOAT, GL_FALSE, sizeof(VertexTextured),
                          (void *) (sizeof(float) * 3));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, backVBO[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(backTexIndices), backTexIndices, GL_STATIC_DRAW);

    fprintf(stdout, "[INFO]: platform read in with VAO %d\n", backVAO);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////// front plane /////////////////////////////////////////////////////
    VertexTextured frontTexVertices[4] = {
            {quadSize,  0.0f,     quadSize, 0.0f, 0.0f},
            {-quadSize, 0.0f,     quadSize, 1.0f, 0.0f},
            {quadSize,  quadSize, quadSize, 0.0f, 1.0f},
            {-quadSize, quadSize, quadSize, 1.0f, 1.0f}
    };
    unsigned short frontTexIndices[4] = {0, 1, 2, 3};

    glGenVertexArrays(1, &frontVAO);
    glBindVertexArray(frontVAO);

    glGenBuffers(2, frontVBO);

    glBindBuffer(GL_ARRAY_BUFFER, frontVBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(frontTexVertices), frontTexVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(texShaderProgramAttributes.vPos);
    glVertexAttribPointer(texShaderProgramAttributes.vPos, 3, GL_FLOAT, GL_FALSE, sizeof(VertexTextured),
                          (void *) 0);

    glEnableVertexAttribArray(texShaderProgramAttributes.vTexCoord);
    glVertexAttribPointer(texShaderProgramAttributes.vTexCoord, 2, GL_FLOAT, GL_FALSE, sizeof(VertexTextured),
                          (void *) (sizeof(float) * 3));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, frontVBO[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(frontTexIndices), frontTexIndices, GL_STATIC_DRAW);

    fprintf(stdout, "[INFO]: platform read in with VAO %d\n", frontVAO);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////// top plane /////////////////////////////////////////////////////
    VertexTextured topTexVertices[4] = {
            {quadSize,  quadSize, -quadSize, 0.0f, 1.0f},
            {-quadSize, quadSize, -quadSize, 1.0f, 1.0f},
            {quadSize,  quadSize, quadSize,  0.0f, 0.0f},
            {-quadSize, quadSize, quadSize,  1.0f, 0.0f}
    };
    unsigned short topTexIndices[4] = {0, 1, 2, 3};

    glGenVertexArrays(1, &topVAO);
    glBindVertexArray(topVAO);

    glGenBuffers(2, topVBO);

    glBindBuffer(GL_ARRAY_BUFFER, topVBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(topTexVertices), topTexVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(texShaderProgramAttributes.vPos);
    glVertexAttribPointer(texShaderProgramAttributes.vPos, 3, GL_FLOAT, GL_FALSE, sizeof(VertexTextured),
                          (void *) 0);

    glEnableVertexAttribArray(texShaderProgramAttributes.vTexCoord);
    glVertexAttribPointer(texShaderProgramAttributes.vTexCoord, 2, GL_FLOAT, GL_FALSE, sizeof(VertexTextured),
                          (void *) (sizeof(float) * 3));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, topVBO[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(topTexIndices), topTexIndices, GL_STATIC_DRAW);

    fprintf(stdout, "[INFO]: platform read in with VAO %d\n", topVAO);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}

void setupOpenGL() {
    // initialize GLEW
    glewExperimental = GL_TRUE;
    GLenum glewResult = glewInit();

    // check for an error
    if( glewResult != GLEW_OK ) {
        printf( "[ERROR]: Error initializing GLEW\n");
        exit(EXIT_FAILURE);
    }

    glEnable( GL_DEPTH_TEST );

    glEnable(GL_BLEND);									                        // enable blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	                        // use one minus blending equation

    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );	// set the clear color to black
}

void setTreeTopperColor(glm::vec3 &color) {
    if (getRand() < 0.5) color.r += 0.01;
    else color.r -= 0.01;
    if (color.r > 1.0f) color.r = 1.0f;
    if (color.r < 0.0f) color.r = 0.0f;

    if (getRand() < 0.5) color.b += 0.01;
    else color.b -= 0.01;
    if (color.b > 1.0f) color.b = 1.0f;
    if (color.b < 0.0f) color.b = 0.0f;

    if (getRand() < 0.5) color.g += 0.01;
    else color.g -= 0.01;
    if (color.g > 1.0f) color.g = 1.0f;
    if (color.g < 0.0f) color.g = 0.0f;
}

void setupScene() {
    // give the camera a scenic starting point.
    camPos.x = 60;
    camPos.y = 40;
    camPos.z = 30;

    //give the voltorb a default start point
    playerPos.x = 0;
    playerPos.y = 2;
    playerPos.z = 0;

    //keep track of voltorb movement
    playerMove.x = 0;
    playerMove.y = 0;
    playerMove.z = 0;

    float light_cutoff = glm::radians(0.785f);
    //glUniform1f(candleShaderUniforms.cutoffSpotLight, light_cutoff);

    // configure arcball camera
    cameraTheta = M_PI / 3.0;
    cameraPhi = M_PI / 1.6;
    updateArcballCamera();

    // configure free cam
    freeCamAngles = glm::vec3(-M_PI / 3.0f, M_PI / 2.8f, 1.0f);
    freeCamSpeed = glm::vec2(0.25f, 0.02f);

    srand(time(nullptr));    // seed our random number generator
    generateEnvironment();

    treeTopperShaderProgram->useProgram();
    // send tree topper color data

    topperColor = glm::vec3(0.5, 0.5, 0.5);
    glUniform3fv(treeTopperShaderProgramUniforms.topperColor, 1, &topperColor[0]);

    lightingShader->useProgram();           // use our lighting shader program so when
    // assign uniforms, they get sent to this shader

    // TODO #4 set the light direction and color
    glm::vec3 lightDirection = glm::vec3(-1, -1, 1);
    glm::vec3 lightColor = glm::vec3(1, 1, 1);
    glm::vec3 pointLightColor = glm::vec3(1, 1,1);
    // for point light
    glm::vec3 lightPosition = glm::vec3(1, 1, 0);
    glUniform3fv(lightingShaderUniforms.lightPosition, 1, &lightPosition[0]);

    glUniform3fv(lightingShaderUniforms.lightDirection, 1, &lightDirection[0]);
    glUniform3fv(lightingShaderUniforms.lightColor, 1, &lightColor[0]);
    glUniform3fv(lightingShaderUniforms.pointLightColor, 1, &pointLightColor[0]);

    // SPOT LIGHT VARIABLES
    // for spot light
    GLfloat spotLightTheta = 0.17f;
    glUniform1f(lightingShaderUniforms.spotLightTheta, spotLightTheta);

    glm::vec3 spotLightPosition = glm::vec3(1, 5, 0);
    glm::vec3 spotLightDirection = glm::vec3(0, -1, 0);
    glm::vec3 spotLightColor = glm::vec3(0, 0, 1);

    glUniform3fv(lightingShaderUniforms.spotLightPosition, 1, &spotLightPosition[0]);
    glUniform3fv(lightingShaderUniforms.spotLightDirection, 1, &spotLightDirection[0]);
    glUniform3fv(lightingShaderUniforms.spotLightColor, 1, &spotLightColor[0]);

    /////////////////////////////////////////////
    candleShader->useProgram();

    //glm::vec3 lightPoint = glm::vec3(-55.0, 5.0, -55.0);
    glm::vec3 candleLightPos = glm::vec3(10.0,9.0,10.0);
    glm::vec3 candleLightColor = glm::vec3(0,0,1);
    glm::vec3 candleLightDirection = glm::vec3(0,1,0);

    /*glm::vec3 lightVec = glm::vec3(-1.0, -1.0, -1.0);
    glm::vec3 lightVecColr = glm::vec3(.1, .1, 1);*/

    glUniform3fv(candleShaderUniforms.lightPositionPoint, 1, &candleLightPos[0]);
    glUniform3fv(candleShaderUniforms.lightColorPoint, 1, &candleLightColor[0]);
    //glUniform3fv(candleShaderUniforms.camPos, 1, &camPos[0]);
    glUniform3fv(candleShaderUniforms.lightDirection, 1, &candleLightDirection[0]);

    lightingShader->useProgram();
}

void updateScene() {
    setTreeTopperColor(topperColor);
}

void setupTextures() {
    // LOOKHERE #4
    spriteTextureHandle = CSCI441::TextureUtils::loadAndRegisterTexture("assets/textures/snowflake.png");
    /// loading in the files for each plane's texture
    rightTextureHandle = CSCI441::TextureUtils::loadAndRegisterTexture("assets/textures/posx.jpg");
    leftTextureHandle = CSCI441::TextureUtils::loadAndRegisterTexture("assets/textures/negx.jpg");
    topTextureHandle = CSCI441::TextureUtils::loadAndRegisterTexture("assets/textures/posy.jpg");
    frontTextureHandle = CSCI441::TextureUtils::loadAndRegisterTexture("assets/textures/posz.jpg");
    backTextureHandle = CSCI441::TextureUtils::loadAndRegisterTexture("assets/textures/negz.jpg");
}

///*************************************************************************************
// Our main function

int main() {
    // GLFW sets up our OpenGL context so must be done first
    GLFWwindow *window = setupGLFW();	                // initialize all of the GLFW specific information related to OpenGL and our window
    setupOpenGL();										// initialize all of the OpenGL specific information
    CSCI441::OpenGLUtils::printOpenGLInfo();

    setupShaders();                                         // load our shader program into memory
    setupBuffers();
    setupTextures();
    setupScene();

    // TODO #5 connect the CSCI441 objects library to our shader attribute inputs
    // needed to connect our 3D Object Library to our shader
    CSCI441::setVertexAttributeLocations( lightingShaderAttributes.vPos, lightingShaderAttributes.vertexNormal);

    printf("Controls:\n");
    printf("\tWASD - Move Jarrison\n");
    printf("\tTFGH - Move Voltorb\n");
    printf("\tIJKL - Move Blossom\n");
    printf("\tC - Switch between characters\n");
    printf("\t1 - Switch between free cam and arcball cam\n");
    printf("\tX - Move backward in free cam\n");\
    printf("\tSpace Bar - Move forward in free cam\n");
    printf("\tArrow Keys - Move free cam around\n");
    printf("\tLeft Click and move mouse - Move arcball around\n");
    printf("\tCtrl and Left Click and move mouse - Zoom arcball cam\n");
    printf("\t2 - Toggle first person camera window\n");
    printf("\tQ / ESC - Quit program\n");


    //  This is our draw loop - all rendering is done here.  We use a loop to keep the window open
    //	until the user decides to close the window and quit the program.  Without a loop, the
    //	window will display once and then the program exits.
    while( !glfwWindowShouldClose(window) ) {	        // check if the window was instructed to be closed
        glDrawBuffer( GL_BACK );				        // work with our back frame buffer
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );	// clear the current color contents and depth buffer in the window

        // Get the size of our framebuffer.  Ideally this should be the same dimensions as our window, but
        // when using a Retina display the actual window can be larger than the requested window.  Therefore
        // query what the actual size of the window we are rendering to is.
        GLint framebufferWidth, framebufferHeight;
        glfwGetFramebufferSize( window, &framebufferWidth, &framebufferHeight );

        // update the viewport - tell OpenGL we want to render to the whole window
        glViewport( 0, 0, framebufferWidth, framebufferHeight );

        // update the projection matrix based on the window size
        // the GL_PROJECTION matrix governs properties of the view coordinates;
        // i.e. what gets seen - use a perspective projection that ranges
        // with a FOV of 45 degrees, for our current aspect ratio, and Z ranges from [0.001, 1000].
        glm::mat4 projMtx = glm::perspective( 45.0f, (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.001f, 1000.0f );
        //CSCI441::SimpleShader3::setProjectionMatrix(projMtx);

        // set up our look at matrix to position our camera
        glm::mat4 viewMtx;
        // arcball camera
        if (arcballCamActive) {
            viewMtx = glm::lookAt(camPos + arcballLookAtPoint,
                                  arcballLookAtPoint,
                                  glm::vec3(0, 1, 0));
        }
            // free camera
        else if (freeCamActive) {
            viewMtx = glm::lookAt(freeCamPos,
                                  freeCamPos + freeCamDir,
                                  glm::vec3(0, 1, 0));
        }

        renderScene(viewMtx,projMtx);					                // draw everything to the window
        updateScene();

        // draw another viewport if first person camera is active
        if (fpCamActive) {
            glEnable(GL_SCISSOR_TEST);
            glScissor(WINDOW_WIDTH - (0.4*WINDOW_WIDTH), 0.6*WINDOW_HEIGHT, 0.4*WINDOW_WIDTH, 0.4*WINDOW_HEIGHT);
            glViewport( WINDOW_WIDTH - (0.4*WINDOW_WIDTH), 0.6*WINDOW_HEIGHT, 0.4*WINDOW_WIDTH, 0.4*WINDOW_HEIGHT );
            viewMtx = glm::lookAt(fpCamPos,
                                  fpCamLookAtPoint,
                                  glm::vec3(  0,  1,  0 ) );
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            renderScene(viewMtx, projMtx);
            updateScene();
            glDisable(GL_SCISSOR_TEST);
        }

        glfwSwapBuffers(window);                        // flush the OpenGL commands and make sure they get rendered!
        glfwPollEvents();				                // check for any events and signal to redraw screen

        updateJarrisonAnimation();
        updateSantaDirection();

        // the following code is a hack for OSX Mojave
        // the window is initially black until it is moved
        // so instead of having the user manually move the window,
        // we'll automatically move it and then move it back
        if( !mackHack ) {
            GLint xPos, yPos;
            glfwGetWindowPos(window, &xPos, &yPos);
            glfwSetWindowPos(window, xPos+10, yPos+10);
            glfwSetWindowPos(window, xPos, yPos);
            mackHack = true;
        }
    }

    glfwDestroyWindow( window );// clean up and close our window
    glfwTerminate();						// shut down GLFW to clean up our context

    return EXIT_SUCCESS;				// exit our program successfully!
}