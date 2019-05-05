#version 330 core
layout(location = 0) in vec3 aPos;//位置变量的属性位置值为0

uniform mat4 model;//变换矩阵
uniform mat4 view;
uniform mat4 projection;//投影矩阵

void main(void)
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
