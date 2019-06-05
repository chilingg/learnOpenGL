#version 330 core
layout(location = 0) in vec3 aPos;//位置变量的属性位置值为0
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
//uniform缓冲对象
layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

out VsOut
{
    vec3 NormalV;
    vec3 FragPosV;
    vec2 TexCoordV;
} vsOut;

uniform mat4 model;//变换矩阵
uniform mat3 normalMat;

void main(void)
{
    vsOut.NormalV = mat3(model) * aNormal;
    vsOut.FragPosV = vec3(model * vec4(aPos, 1.0));
    vsOut.TexCoordV = aTexCoord;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
