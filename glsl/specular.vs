#version 330
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_shading_language_420pack : require

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexNormal_modelspace;
layout(location = 2) in vec3 vertexTangent_modelspace;
layout(location = 3) in vec3 vertexBitangent_modelspace;
layout(location = 4) in vec2 vertexUV;

// Output data ; will be interpolated for each fragment.
out vec2 UV;
out vec3 Position_worldspace;
out vec3 EyeDirection_cameraspace;
out vec3 EyeDirection_tangentspace;
out mat3 TBN;

out vec3 vertexNormal_tangentspace;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;
uniform mat3 MV3x3;

void main(){

    // Output position of the vertex, in clip space : MVP * position
    gl_Position =  MVP * vec4(vertexPosition_modelspace, 1);
    
    // Position of the vertex, in worldspace : M * position
    Position_worldspace = (M * vec4(vertexPosition_modelspace, 1)).xyz;
    
    // Vector that goes from the vertex to the camera, in camera space.
    // In camera space, the camera is at the origin (0, 0, 0).
    vec3 vertexPosition_cameraspace = ( V * M * vec4(vertexPosition_modelspace, 1)).xyz;
    EyeDirection_cameraspace = -vertexPosition_cameraspace;
    
    // UV of the vertex. No special space for this one.
    UV = vertexUV;
    
    // model to camera = ModelView
    vec3 vertexTangent_cameraspace   = normalize(MV3x3 * vertexTangent_modelspace);
    vec3 vertexBitangent_cameraspace = normalize(MV3x3 * vertexBitangent_modelspace);
    vec3 vertexNormal_cameraspace    = normalize(MV3x3 * vertexNormal_modelspace);
    
    TBN = transpose(mat3(
        vertexTangent_cameraspace,
        vertexBitangent_cameraspace,
        vertexNormal_cameraspace
    )); // You can use dot products instead of building this matrix and transposing it. See References for details.

    EyeDirection_tangentspace =  TBN * EyeDirection_cameraspace;
    vertexNormal_tangentspace =  normalize(TBN * vertexBitangent_cameraspace);
}

