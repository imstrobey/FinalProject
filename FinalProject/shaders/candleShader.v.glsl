#version 410 core

//uniforms
uniform mat4 mvpMtx;
uniform mat3 normalMtx;
uniform mat4 modelMtx;
uniform vec3 camPos; //position of the camera

//point light uniforms
uniform vec3 lightPositionPoint;
uniform vec3 lightColorPoint;
uniform vec3 lightDirection;

uniform vec3 materialColor; //color of the object

//attributes
layout(location = 0) in vec3 vPos; //position of the vertex
in vec3 vNormal; //position of the normal (y-axis)

//varyings
out vec3 light;

void main() {
    //create normal vector
    vec3 normVec;
    normVec = normalize(normalMtx * vNormal);

    vec4 vPosition = modelMtx * vec4(vPos, 1.0); // vertex position in world space

    vec3 viewVector = normalize(camPos - vPosition.xyz); //view vector from diff. b/t camera and vertex positions

    vec3 ambient = (vec3(0.5, 0.5, 0.5) * materialColor); //ambient lighting (gray * the material's color)

    //point light - taking difference between vector pos
    //and light point pos to get a vector
    vec3 lightVecPoint;
    lightVecPoint = lightPositionPoint - vPosition.xyz;

    //point light - create halfway vector
    vec3 halfwayVecPoint;
    halfwayVecPoint = lightVecPoint + viewVector;

    //attenuating point light
    vec3 colorAttenuatedPoint;
    colorAttenuatedPoint = lightColorPoint * 1.0 / (1.0 + (0.14*abs(vPosition.xyz - lightVecPoint)) + (0.007*dot(abs(vPosition.xyz - lightVecPoint), abs(vPosition.xyz - lightVecPoint))));
    //point light - calculating diffusion intensity
    vec3 diffuseResultPoint;
    diffuseResultPoint = colorAttenuatedPoint * materialColor * max(dot(lightVecPoint, normVec), 0.0);

    //point light - calculating specular intesity
    vec3 specularResultPoint;
    specularResultPoint = colorAttenuatedPoint * materialColor * max(dot(halfwayVecPoint, normVec), 0.0);

    vec3 diffusion; //diffusion intensity
    vec3 specular; //specular intensity
    diffusion = diffuseResultPoint /*+ (5*diffuseResultDirVec) + diffuseResultSpot) + diffuseResultPoint2*/;
    specular = specularResultPoint /*+ (5*specularResultDirVec) + specularResultSpot)*/;

    light = ambient + specular + diffusion;

    gl_Position = mvpMtx * vec4(vPos, 1.0);
}