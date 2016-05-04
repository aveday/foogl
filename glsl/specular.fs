#version 330
#extension GL_ARB_explicit_uniform_location : require

layout(location = 3) uniform vec3 cameraPosition;

float materialShininess = 80;
vec3 materialSpecularColor = vec3(1,1,1);

#define MAX_LIGHTS 10
struct Light
{
     vec3 position;
     vec3 color;
    float attenuation;
    float ambientCoefficient;
};

uniform UBO
{
    Light lights[MAX_LIGHTS];
    int num_lights;
};

in vec3 fragPosition;
in vec3 fragNormal;
in vec4 fragColor;

out vec4 finalColor;

vec3 ApplyLight(Light light, vec3 surfaceColor, vec3 normal, vec3 surfacePos, vec3 surfaceToCamera) {
    vec3 surfaceToLight;

    surfaceToLight = normalize(light.position.xyz - surfacePos);
    float distanceToLight = length(light.position.xyz - surfacePos);
    float attenuation = 1.0 / (1.0 + light.attenuation * pow(distanceToLight, 2));

    //ambient
    vec3 ambient = light.ambientCoefficient * surfaceColor.rgb * light.color;

    //diffuse
    float diffuseCoefficient = max(0.0, dot(normal, surfaceToLight));
    vec3 diffuse = diffuseCoefficient * surfaceColor.rgb * light.color;
    
    //specular
    float specularCoefficient = 0.0;
    if(diffuseCoefficient > 0.0)
        specularCoefficient = pow(max(0.0, dot(surfaceToCamera, reflect(-surfaceToLight, normal))), materialShininess);
    vec3 specular = specularCoefficient * materialSpecularColor * light.color;

    //linear color (color before gamma correction)
    return ambient + attenuation*(diffuse + specular);
}

void main()
{
    vec3 surfaceToCamera = normalize(cameraPosition - fragPosition);

    //combine color from all the lights
    vec3 linearColor = vec3(0);
    for(int i = 0; i < num_lights; ++i){
        linearColor += ApplyLight(lights[i], fragColor.rgb, fragNormal, fragPosition, surfaceToCamera);
    }
    
    //final color (after gamma correction)
    vec3 gamma = vec3(1.0/2.2);
    finalColor = vec4(pow(linearColor, gamma),fragColor.a);
}
