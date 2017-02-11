#version 330
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_shading_language_420pack : require

float specularCoefficient = 5.0;
float ambientCoeffient = 0.1;
float gamma = 1.0 / 1.5;

#define MAX_LIGHTS 10
struct Light
{
    vec3 position;
    vec3 intensity;
};

layout (binding = 0, std140) uniform LIGHTS
{
    int num_lights;
    Light lights[MAX_LIGHTS];
};

// Values that stay constant for the whole mesh.
layout(location = 3) uniform sampler2D DiffuseTextureSampler;
layout(location = 4) uniform sampler2D NormalTextureSampler;
layout(location = 5) uniform sampler2D SpecularTextureSampler;
uniform mat4 V;

// Interpolated values from the vertex shaders
in vec2 UV;
in vec3 Position_worldspace;
in vec3 EyeDirection_cameraspace;
in vec3 EyeDirection_tangentspace;
in mat3 TBN;

in vec3 vertexNormal_tangentspace;

// Ouput data
out vec4 finalColor;

uniform bool debug = false;

void main(){
    // Material properties

    vec3 MaterialDiffuseColor  = debug ? vec3(0.3) :
        texture(DiffuseTextureSampler,  UV).rgb;

    vec3 MaterialSpecularColor = debug ? vec3(0.1) :
        texture(SpecularTextureSampler, UV).rgb;

    vec3 MaterialAmbientColor  = vec3(ambientCoeffient) * MaterialDiffuseColor;

    // Local normal, in tangent space
    vec3 TextureNormal_tangentspace = debug ? vec3(0,0,1) :
        normalize( texture(NormalTextureSampler, UV).rgb * 2 - 1 );

    vec3 linearColor = vec3(0);

    for(int i = 0; i < num_lights; ++i) {
        // Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
        vec3 LightPosition_cameraspace = ( V * vec4(lights[i].position, 1) ).xyz;
        vec3 LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;

        vec3 LightPosition_tangentspace = lights[i].position - Position_worldspace;
        vec3 LightDirection_tangentspace = TBN * LightDirection_cameraspace;

        // Normal of the computed fragment, in camera space
        vec3 n = TextureNormal_tangentspace;
        // Direction of the light (from the fragment to the light)
        vec3 l = normalize(LightDirection_tangentspace);
        // Cosine of the angle between the normal and the light direction, 
        float cosTheta = clamp( dot( n, l ), 0, 1 );

        // Eye vector (towards the camera)
        vec3 E = normalize(EyeDirection_tangentspace);
        // Direction in which the triangle reflects the light
        vec3 R = reflect(-l, n);
        // Cosine of the angle between the Eye vector and the Reflect vector,
        float cosAlpha = clamp( dot( E, R ), 0, 1 );
        
        float distanceSq = dot(LightPosition_tangentspace, LightPosition_tangentspace);
        vec3 attenuation = lights[i].intensity / distanceSq;
        linearColor += 
            MaterialAmbientColor +
            MaterialDiffuseColor * cosTheta * attenuation +
            MaterialSpecularColor * pow(cosAlpha, specularCoefficient) * attenuation; 
    }
    finalColor = vec4( pow(linearColor, vec3(gamma)), 1 );
    //finalColor = vec4 (vertexNormal_tangentspace * .5 + .5, 1);
}

