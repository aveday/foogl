#version 330

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

in vec3 Position;
in vec4 Color;
in vec3 Normal;

out vec3 fragVert;
out vec3 fragNormal;
out vec4 fragColor;

void main()
{
    fragVert = vec3(model * vec4(Position, 1));
    fragNormal = normalize(transpose(inverse(mat3(model))) * Normal);
    fragColor = Color;

    gl_Position = projection * view * model * vec4(Position, 1.0);
}
