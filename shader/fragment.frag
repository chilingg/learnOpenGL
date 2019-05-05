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
    vec3 ambient = 0.1 * lightColor;
    FragColor = vec4(objectColor * ambient, 1.0);
}
