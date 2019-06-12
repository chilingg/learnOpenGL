#version 330 core
in vec2 TexCoords;

uniform sampler2D text;
uniform vec3 textColor;

void main()
{
    float sampled = texture2D(text, TexCoords).r;
    if(sampled < 0.1)
        discard;
    gl_FragColor = vec4(textColor, sampled);
}
