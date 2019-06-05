#version 330 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in VsOut
{
    vec3 NormalV;
    vec3 FragPosV;
    vec2 TexCoordV;
} gs_in[];

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoord;

uniform float Time;

vec3 getNormal();

void main()
{
    vec4 explodeRange = 2 * vec4(getNormal(), 0.0);
    float magnitude = (sin(Time) + 1.0) * 0.5;
    magnitude *= (1 - magnitude) * (1 - magnitude) + 1.0;
    explodeRange = magnitude * explodeRange;

    Normal = gs_in[0].NormalV;
    FragPos = gs_in[0].FragPosV;
    TexCoord = gs_in[0].TexCoordV;
    gl_Position = gl_in[0].gl_Position + explodeRange;
    EmitVertex();

    Normal = gs_in[1].NormalV;
    FragPos = gs_in[1].FragPosV;
    TexCoord = gs_in[1].TexCoordV;
    gl_Position = gl_in[1].gl_Position + explodeRange;
    EmitVertex();

    Normal = gs_in[2].NormalV;
    FragPos = gs_in[2].FragPosV;
    TexCoord = gs_in[2].TexCoordV;
    gl_Position = gl_in[2].gl_Position + explodeRange;
    EmitVertex();
    EndPrimitive();
}

vec3 getNormal()
{
    vec3 a = gs_in[0].NormalV - gs_in[1].NormalV;
    vec3 b = gs_in[0].NormalV - gs_in[2].NormalV;

    return normalize(cross(b, a));
}
