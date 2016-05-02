#version 330
#extension GL_ARB_explicit_uniform_location : require

layout(location = 3) uniform vec3 cameraPosition;

float materialShininess = 80;
vec3 materialSpecularColor = vec3(1,1,1);

uniform Light
{
     vec3 lightposition;
     vec3 lightcolor;
    float lightattenuation;
    float lightambientCoefficient;
};

in vec3 fragPosition;
in vec3 fragNormal;
in vec4 fragColor;

out vec4 finalColor;

void main()
{
    vec3 surfaceToLight = normalize(lightposition - fragPosition);
    vec3 surfaceToCamera = normalize(cameraPosition - fragPosition);
    
    //ambient
    vec3 ambient = lightambientCoefficient * fragColor.rgb * lightcolor;

    //diffuse
    float diffuseCoefficient = max(0.0, dot(fragNormal, surfaceToLight));
    vec3 diffuse = diffuseCoefficient * fragColor.rgb * lightcolor;
    
    //specular
    float specularCoefficient = 0.0;
    if(diffuseCoefficient > 0.0)
        specularCoefficient = pow(max(0.0, dot(surfaceToCamera, reflect(-surfaceToLight, fragNormal))), materialShininess);
    vec3 specular = specularCoefficient * materialSpecularColor * lightcolor;
    
    //attenuation
    float distanceToLight = length(lightposition - fragPosition);
    float attenuation = 1.0 / (1.0 + lightattenuation * pow(distanceToLight, 2));

    //linear color (color before gamma correction)
    vec3 linearColor = ambient + attenuation*(diffuse + specular);
    
    //final color (after gamma correction)
    vec3 gamma = vec3(1.0/2.2);
    finalColor = vec4(pow(linearColor, gamma), fragColor.a);
}
