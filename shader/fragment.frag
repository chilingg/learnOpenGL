#version 330 core
out vec4 FragColor;
in float R;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main(void)
{
    vec3 ambient = lightColor * 0.1;
    FragColor = vec4(objectColor * ambient, 1.0);
}
