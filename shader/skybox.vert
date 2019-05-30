#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat3 view;
uniform mat4 projection;

void main(void)
{
    TexCoords = aPos;
    vec3 position = view * aPos;
    gl_Position = projection * vec4(position, 1.0);
    gl_Position = gl_Position.xyww;
}
