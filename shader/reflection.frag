#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 CameraPos;
uniform samplerCube skybox;

void main(void)
{
    float ratio = 1.00 / 1.52;
    vec3 viewDir = normalize(FragPos - CameraPos);//顶点指向摄影机
    vec3 normal = normalize(Normal);//经由法线矩阵变换后，标准化法向量
    vec3 reflection = refract(viewDir, normal, ratio);

    FragColor = vec4(textureCube(skybox, reflection).rgb, 1.0);
}
