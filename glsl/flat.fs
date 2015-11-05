#version 330

uniform mat4 model;
uniform vec3 lightPosition;

in vec3 fragVert;
in vec4 fragColor;
in vec3 fragNormal;

out vec4 finalColor;

vec3 lightColor = vec3(1, 1, 1);

void main()
{
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 normal = normalize(normalMatrix * fragNormal);
    
    // calculate the location of this fragment (pixel) in world coordinates
    vec3 fragPosition = vec3(model * vec4(fragVert, 1));
    
    // calculate the vector from this pixels surface to the light source
    vec3 surfaceToLight = lightPosition - fragPosition;

    // calculate the cosine of the angle of incidence
    float brightness = dot(normal, surfaceToLight) / (length(surfaceToLight) * length(normal));
    brightness = clamp(brightness, 0, 1);

    // calculate final color of the pixel, based on:
    // 1. The angle of incidence: brightness
    // 2. The color/intensities of the light: lightColor
    // 3. The texture and texture coord: texture(tex, fragTexCoord)
    finalColor = vec4(brightness * lightColor * fragColor.rgb, fragColor.a);
}
