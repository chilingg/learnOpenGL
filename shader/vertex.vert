#version 330 core
layout(location = 0) in vec3 aPos;//位置变量的属性位置值为0
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoord;

uniform mat4 model;//变换矩阵
uniform mat4 view;//视图矩阵
uniform mat4 projection;//投影矩阵
uniform mat3 normalMat;

void main(void)
{
    Normal = normalMat * aNormal;
    FragPos = vec3(model * vec4(aPos, 1.0));
    TexCoord = aTexCoord;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
