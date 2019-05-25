#version 330 core

in vec2 TexCoords;
uniform sampler2D screenTexture;

const float offset = 1.0/300.0;
vec2 offsets[9] = vec2[](
    vec2(-offset,  offset), // 左上
    vec2( 0.0f,    offset), // 正上
    vec2( offset,  offset), // 右上
    vec2(-offset,  0.0f),   // 左
    vec2( 0.0f,    0.0f),   // 中
    vec2( offset,  0.0f),   // 右
    vec2(-offset, -offset), // 左下
    vec2( 0.0f,   -offset), // 正下
    vec2( offset, -offset)  // 右下
);
float kernel[9] = float[](
    -1, -1, -1,
    -1, 9, -1,
    -1, -1, -1
);

void main(void)
{
    vec3 sampleTex[9];
    for(int i = 0; i < 9; ++i)
        sampleTex[i] = vec3(texture2D(screenTexture, TexCoords+offsets[i]));

    vec3 color = vec3(0.0);
    for(int i = 0; i < 9; ++i)
        color += sampleTex[i] * kernel[i];

    gl_FragColor = vec4(color, 1.0);
}
