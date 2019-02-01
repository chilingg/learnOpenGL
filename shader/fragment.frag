#version 330 core

in vec3 ourColor;
//out vec4 fragColor;

void main(void)
{
    gl_FragColor = vec4(ourColor, 1.0f);
}
