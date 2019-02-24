#version 330 core

layout(location = 0) in vec3 aPos;//位置变量的属性位置值为0
layout(location = 1) in vec2 aTexCoord;//纹理变量的属性位置值为1
out vec2 TexCoord;//向片段着色器输出一个纹理坐标
uniform mat4 model;//变换矩阵
uniform mat4 view;
uniform mat4 projection;//投影矩阵

//vec4 gl_TempPosition;

void main(void)
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    TexCoord = aTexCoord;
}
