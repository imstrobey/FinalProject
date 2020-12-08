#version 410 core

// uniform inputs
uniform mat4 mvpMatrix;                 // the precomputed Model-View-Projection Matrix
// TODO #D add our normal matrix
// the normal matrix
uniform mat3 normalMatrix;

// DIRECTIONAL & POINT LIGHT VARIABLES
// the direction the incident ray of light is traveling
uniform vec3 lightDirection;
uniform vec3 lightPosition;
// the color of the light
uniform vec3 lightColor;
uniform vec3 pointLightColor;
uniform vec3 materialColor;             // the material color for our vertex (& whole object)
uniform vec3 camPosition;

// SPOT LIGHT VARIABLES
uniform float spotLightTheta;
uniform vec3 spotLightPosition;
uniform vec3 spotLightDirection;
uniform vec3 spotLightColor;

// attribute inputs
layout(location = 0) in vec3 vPos;      // the position of this specific vertex in object space
// TODO #C add our vertex normal
// the normal of this specific vertex in object space
in vec3 vertexNormal;

// varying outputs
layout(location = 0) out vec3 color;    // color to apply to this vertex

void main() {
    // transform & output the vertex in clip space
    gl_Position = mvpMatrix * vec4(vPos, 1.0);

    // TODO #B convert the light direction to our normalized light vector
    vec3 lightVec = normalize(-1 * lightDirection);

    // TODO #E transform the vertex normal in to world space
    vec3 vertexNormalWorld = normalize(normalMatrix * vertexNormal);

    // TODO #F compute the diffuse component of the Phong Illumination Model

    // TODO DIRECTIONAL LIGHT ----------
    vec3 directionalDiffuse = (lightColor * materialColor * max(dot(lightVec, vertexNormalWorld), 0));

    float directionalSpecularStrength = 0.5;
    vec3 directionalViewDir = vec3(normalize(camPosition - vPos));
    vec3 directionalReflectDir = -lightVec + 2 * (dot(vertexNormalWorld,lightVec) * vertexNormalWorld);
    float directionalSpec = pow(max(dot(directionalViewDir,directionalReflectDir),0),32.0f);
    vec3 directionalSpecular = vec3(directionalSpecularStrength * directionalSpec * lightColor * materialColor);

    // TODO POINT LIGHT ------------
    vec3 pointLightDir = normalize(lightPosition - vPos);
    float pointDiff = max(dot(vertexNormalWorld, pointLightDir), 0.0);
    vec3 pointDiffuse = pointLightColor * materialColor * pointDiff;

    vec3 pointViewDir = normalize(camPosition - vPos);
    vec3 pointReflectDir = -lightVec + 2 * (dot(vertexNormalWorld,lightVec) * vertexNormalWorld);

    float pointSpec = pow(max(dot(pointViewDir, pointReflectDir), 0.0), 32.0f);
    vec3 pointSpecular = pointLightColor * materialColor * pointSpec;

    vec3 ambient = vec3(0.1, 0.1, 0.1);

    // TODO SPOT LIGHT

    vec3 vPosDirection = normalize(vPos - spotLightPosition);

    float phi = acos(max(dot(spotLightDirection, vPosDirection), 0));

    vec3 spotDiffuse;
    vec3 spotSpecular;

    if (phi < spotLightTheta) {
        vec3 spotLightDir = normalize(lightPosition - vPos);

        float spotDiff = max(dot(vertexNormalWorld, spotLightDir), 0.0);
        spotDiffuse = spotLightColor * materialColor * spotDiff;

        float spotSpecularStrength = 0.5;
        vec3 spotViewDir = vec3(normalize(lightPosition - vPos));
        vec3 spotReflectDir = -spotLightDir + 2 * (dot(vertexNormalWorld,spotLightDir) * vertexNormalWorld);
        float spotSpec = pow(max(dot(spotViewDir,spotReflectDir),0),32.0f);
        spotSpecular = vec3(spotSpecularStrength * spotSpec * spotLightColor * materialColor);
    }
    else {
        spotDiffuse = vec3(0, 0, 0);
    }

    // TODO

    // implement attenuation
    float constant = 1.0f;
    float linear = 0.1f;
    float quadratic = 0.032f;

    float pointDistance = length(lightPosition - vPos);
    float pointAttenuation = 1.0f / (constant + linear * pointDistance + quadratic * (pointDistance * pointDistance));

    pointDiffuse *= pointAttenuation;
    pointSpecular *= pointAttenuation;
    ambient *= pointAttenuation;

    constant = 1.0f;
    linear = 0.9f;
    quadratic = 0.9f;

    float spotDistance = length(spotLightPosition - vPos);
    float spotAttenuation = 1.0f / (constant + linear * spotDistance + quadratic * (spotDistance * spotDistance));

    spotDiffuse *= spotAttenuation;
    spotSpecular *= spotAttenuation;

    // TODO #G output the illumination color of this vertex
    // assign the color for this vertex


    //color = pointDiffuse;
    color = directionalDiffuse + directionalSpecular + pointDiffuse + pointSpecular + ambient + 10 * (spotDiffuse + spotSpecular);
}