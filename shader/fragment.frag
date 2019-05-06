#version 330 core
out vec4 FragColor;
in vec3 normal;
in vec3 fragPos;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

void main(void)
{
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(normalize(normal), lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 ambient = 0.15 * lightColor;//环境光
    FragColor = vec4((ambient + diffuse) * objectColor, 1.0);
}
