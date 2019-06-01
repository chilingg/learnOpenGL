#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
//uniform缓冲对象
layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

out vec2 TexCoords;

uniform mat4 model;//变换矩阵

void main(void)
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoords = aTexCoords;
}
