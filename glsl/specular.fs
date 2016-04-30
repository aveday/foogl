#version 330

uniform mat4 model;
uniform vec3 cameraPosition;
uniform vec4 colorOverride;

float materialShininess = 80;
vec3 materialSpecularColor = vec3(1,1,1);

uniform vec3 lightPosition;
uniform vec3 lightColor;
float lightAttenuation = 0.2f;
float lightAmbientCoefficient = 0.005f;

in vec3 fragVert;
in vec4 fragColor;
in vec3 fragNormal;

out vec4 finalColor;

void main()
{
    vec3 normal = normalize(transpose(inverse(mat3(model))) * fragNormal);
    vec3 surfacePos = vec3(model * vec4(fragVert, 1));
    
    vec4 surfaceColor = fragColor;
    if(colorOverride.a != 0)
        surfaceColor = colorOverride;

    vec3 surfaceToLight = normalize(lightPosition - surfacePos);
    vec3 surfaceToCamera = normalize(cameraPosition - surfacePos);
    
    //ambient
    vec3 ambient = lightAmbientCoefficient * surfaceColor.rgb * lightColor;

    //diffuse
    float diffuseCoefficient = max(0.0, dot(normal, surfaceToLight));
    vec3 diffuse = diffuseCoefficient * surfaceColor.rgb * lightColor;
    
    //specular
    float specularCoefficient = 0.0;
    if(diffuseCoefficient > 0.0)
        specularCoefficient = pow(max(0.0, dot(surfaceToCamera, reflect(-surfaceToLight, normal))), materialShininess);
    vec3 specular = specularCoefficient * materialSpecularColor * lightColor;
    
    //attenuation
    float distanceToLight = length(lightPosition - surfacePos);
    float attenuation = 1.0 / (1.0 + lightAttenuation * pow(distanceToLight, 2));

    //linear color (color before gamma correction)
    vec3 linearColor = ambient + attenuation*(diffuse + specular);
    
    //final color (after gamma correction)
    vec3 gamma = vec3(1.0/2.2);
    finalColor = vec4(pow(linearColor, gamma), surfaceColor.a);
}
