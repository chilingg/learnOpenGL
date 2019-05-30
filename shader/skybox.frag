#version 330 core

in vec3 TexCoords;
uniform samplerCube skybox;

void main(void)
{
    gl_FragColor = textureCube(skybox, TexCoords);
}
