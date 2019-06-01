#version 330 core
layout (location = 0) in vec3 aPos;
layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

out vec3 TexCoords;

void main(void)
{
    mat3 view3x3 = mat3(view);
    TexCoords = aPos;
    vec3 position = view3x3 * aPos;
    gl_Position = projection * vec4(position, 1.0);
    gl_Position = gl_Position.xyww;
}
