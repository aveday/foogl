#version 330
#extension GL_ARB_explicit_uniform_location : require

layout(location = 2) uniform vec3 cameraPosition;
layout(location = 3) uniform vec4 materialColor;

float materialShininess = 80;
vec3 materialSpecularColor = vec3(1,1,1);

struct Light
{
    vec3 position;
    float attenuation;
    vec3 intensity;
    float ambientCoefficient;
};

#define MAX_LIGHTS 10
uniform LIGHTS
{
    int num_lights;
    Light lights[MAX_LIGHTS];
};


in vec3 fragPosition;
in vec3 fragNormal;

out vec4 finalColor;

vec3 ApplyLight(Light light, vec3 surfaceColor, vec3 normal, vec3 surfacePos, vec3 surfaceToCamera) {
    vec3 surfaceToLight;

    surfaceToLight = normalize(light.position.xyz - surfacePos);
    float distanceToLight = length(light.position.xyz - surfacePos);
    float attenuation = 1.0 / (1.0 + light.attenuation * pow(distanceToLight, 2));

    //ambient
    vec3 ambient = light.ambientCoefficient * surfaceColor.rgb * light.intensity;

    //diffuse
    float diffuseCoefficient = max(0.0, dot(normal, surfaceToLight));
    vec3 diffuse = diffuseCoefficient * surfaceColor.rgb * light.intensity;
    
    //specular
    float specularCoefficient = 0.0;
    if(diffuseCoefficient > 0.0)
        specularCoefficient = pow(max(0.0, dot(surfaceToCamera, reflect(-surfaceToLight, normal))), materialShininess);
    vec3 specular = specularCoefficient * materialSpecularColor * light.intensity;

    //linear color (color before gamma correction)
    return ambient + attenuation*(diffuse + specular);
}

void main()
{
    vec3 surfaceToCamera = normalize(cameraPosition - fragPosition);
    vec3 surfaceColor = vec3(materialColor); //TODO update for transparency

    //combine color from all the lights
    vec3 linearColor = vec3(0);
    for(int i = 0; i < num_lights; ++i) {
        linearColor += ApplyLight(lights[i], surfaceColor, fragNormal, fragPosition, surfaceToCamera);
    }
    
    //final color (after gamma correction)
    vec3 gamma = vec3(1.0/2.2);
    finalColor = vec4(pow(linearColor, gamma), 1);
}
