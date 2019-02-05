#version 330 core

layout(location = 0) in vec3 aPos;//位置变量的属性位置值为0
layout(location = 1) in vec2 aTexCoord;//纹理变量的属性位置值为1
out vec2 TexCoord;//向片段着色器输出一个纹理坐标

void main(void)
{
    gl_Position = vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}
