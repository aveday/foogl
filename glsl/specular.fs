#version 330
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_shading_language_420pack : require

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
//uniform sampler2D SpecularTextureSampler;
uniform mat4 V;
uniform mat4 M;
uniform mat3 MV3x3;

// Interpolated values from the vertex shaders
in vec2 UV;
in vec3 Position_worldspace;
in vec3 EyeDirection_cameraspace;
in vec3 EyeDirection_tangentspace;
in mat3 TBN;

// Ouput data
out vec4 finalColor;

void main(){
    // Material properties
    vec3 MaterialDiffuseColor = texture(DiffuseTextureSampler, UV).rgb;
    vec3 MaterialAmbientColor = vec3(0.1,0.1,0.1) * MaterialDiffuseColor;
    vec3 MaterialSpecularColor = vec3(0.3);

    // Local normal, in tangent space. V tex coordinate is inverted because normal map is in TGA (not in DDS) for better quality
    vec3 TextureNormal_tangentspace = normalize(texture( NormalTextureSampler, UV ).rgb*2.0 - 1.0);

    vec3 linearColor = vec3(0);
    for(int i = 0; i < num_lights; ++i) {
        // Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
        vec3 LightPosition_cameraspace = ( V * vec4(lights[i].position, 1) ).xyz;
        vec3 LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;
        vec3 LightDirection_tangentspace = TBN * LightDirection_cameraspace;

        // Distance to the light
        float distance = length( lights[i].position - Position_worldspace );

        // Normal of the computed fragment, in camera space
        vec3 n = TextureNormal_tangentspace;
        // Direction of the light (from the fragment to the light)
        vec3 l = normalize(LightDirection_tangentspace);
        // Cosine of the angle between the normal and the light direction, 
        // clamped above 0
        //  - light is at the vertical of the triangle -> 1
        //  - light is perpendicular to the triangle -> 0
        //  - light is behind the triangle -> 0
        float cosTheta = clamp( dot( n,l ), 0,1 );

        // Eye vector (towards the camera)
        vec3 E = normalize(EyeDirection_tangentspace);
        // Direction in which the triangle reflects the light
        vec3 R = reflect(-l,n);
        // Cosine of the angle between the Eye vector and the Reflect vector,
        // clamped to 0
        //  - Looking into the reflection -> 1
        //  - Looking elsewhere -> < 1
        float cosAlpha = clamp( dot( E,R ), 0,1 );
        
        linearColor += 
            // Ambient : simulates indirect lighting
            MaterialAmbientColor +
            // Diffuse : "color" of the object
            MaterialDiffuseColor * lights[i].intensity * cosTheta / (distance*distance) +
            // Specular : reflective highlight, like a mirror
            MaterialSpecularColor * lights[i].intensity * pow(cosAlpha,5) / (distance*distance);
    }
    vec3 gamma = vec3(1.0/1.5);
    finalColor = vec4(pow(linearColor, gamma), 1);
}

