#version 330 core

in vec2 TexCoord;
//out vec4 fragColor;
uniform sampler2D ourTexture1;//内建采样器类型
uniform sampler2D ourTexture2;

void main(void)
{
    gl_FragColor = mix(texture2D(ourTexture1, TexCoord), texture2D(ourTexture2, vec2(TexCoord.x*2, TexCoord.y*2)), 0.2f);
}
