#version 330

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

in vec3 Position;
in vec4 Color;
in vec3 Normal;

out vec3 fragVert;
out vec4 fragColor;
out vec3 fragNormal;

void main()
{
    fragVert = Position;
    fragColor = Color;
    fragNormal = Normal;

    gl_Position = projection * view * model * vec4(Position, 1.0);
}
