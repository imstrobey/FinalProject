#version 410 core

//uniforms
uniform mat4 mvpMtx;
uniform mat3 normalMtx;
//uniform mat4 modelMtx;
uniform vec3 camPos; //position of the camera

//point light uniforms
uniform vec3 lightPositionPoint;
uniform vec3 lightColorPoint;
uniform vec3 lightDirection;

uniform vec3 attenuationChange;

uniform vec3 materialColor; //color of the object

//uniform vec3 attentuationChange; // takes in 3 floats (constant, linear, quadratic)

//attributes
layout(location = 0) in vec3 vPos; //position of the vertex
in vec3 vNormal; //position of the normal (y-axis)

//varyings
out vec3 light;

void main() {
    gl_Position = mvpMtx * vec4(vPos,1.0);

    /*vec3 ambient;
    vec3 diffuse;
    vec3 specular;*/

    //float constant = 1.0f;
    //float linear = 0.14f;
    //float quadratic = 0.007f;

    float constantChange = attenuation.x;
    float linearChange = attentuation.y;
    float quadraticChange = attentuation.z;

    //float distance = length(lightPositionPoint - vPos);
    //float attenuation;// = 1.0f/(constant + linear * distance + quadratic * (distance * distance));

    ////////////////////////////////////////////////////////////////////////
    //vec3 normalLightVec = normalize(normalMtx * vNormal);
    vec3 normalLightVec = normalize(-1 * lightDirection);
    //vec4 vPosition = modelMtx * vec4(vPos, 1.0);
    //vec3 viewVector = normalize(camPos - vPos);
    vec3 vNormalWorld = normalize(normalMtx * vNormal);
//////////////////////////////////////////////////////////////////////////////////////
    vec3 pointLightDir = normalize(lightPositionPoint - vPos);
    float pointDiff = max(dot(vNormalWorld,pointLightDir), 0.0);
    vec3 pointDiffuse = lightColorPoint * materialColor * pointDiff;

    vec3 pointViewDir = normalize(camPos - vPos);
    vec3 pointReflectDir = -normalLightVec + 2 * (dot(vNormalWorld,normalLightVec) * vNormalWorld);

    float pointSpec = pow(max(dot(pointViewDir, pointReflectDir), 0.0), 32.0f);
    vec3 pointSpecular = lightColorPoint * materialColor * pointSpec;

    vec3 ambient = vec3(0.1, 0.1, 0.1);

    float pointDistance = length(lightPositionPoint - vPos);
    float newAttenuation = 1.0f / (constantChange + linearChange * pointDistance + quadraticChange * (pointDistance * pointDistance));

    ////////////////////////////////////////////////////////////////////////

    ambient *= newAttenuation;
    pointDiffuse *= newAttenuation;
    pointSpecular *= newAttenuation;

    light = pointDiffuse + pointSpecular + ambient;

    //create normal vector
    /*vec3 normVec;
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
    diffusion = diffuseResultPoint *//*+ (5*diffuseResultDirVec) + diffuseResultSpot) + diffuseResultPoint2*//*;
    specular = specularResultPoint *//*+ (5*specularResultDirVec) + specularResultSpot)*//*;

    light = ambient + specular + diffusion;

    gl_Position = mvpMtx * vec4(vPos, 1.0);*/
}