#include "mmatrix.h"
#include <cmath>

MMat4 translate(const MMat4 &mat, const MVec3 &vec3)
{
    MMat4 idt;
    idt.identityMatrix();
    idt.matrix[0][3] += vec3.x;
    idt.matrix[1][3] += vec3.y;
    idt.matrix[2][3] += vec3.z;

    return idt * mat;
}

MVec4 operator*(const MMat4 &mat4, const MVec4 &vec4)
{
    float x = vec4.x * mat4.matrix[0][0]
            + vec4.y * mat4.matrix[0][1]
            + vec4.z * mat4.matrix[0][2]
            + vec4.w * mat4.matrix[0][3];

    float y = vec4.x * mat4.matrix[1][0]
            + vec4.y * mat4.matrix[1][1]
            + vec4.z * mat4.matrix[1][2]
            + vec4.w * mat4.matrix[1][3];

    float z = vec4.x * mat4.matrix[2][0]
            + vec4.y * mat4.matrix[2][1]
            + vec4.z * mat4.matrix[2][2]
            + vec4.w * mat4.matrix[2][3];

    float w = vec4.x * mat4.matrix[3][0]
            + vec4.y * mat4.matrix[3][1]
            + vec4.z * mat4.matrix[3][2]
            + vec4.w * mat4.matrix[3][3];

    return {x, y, z, w};
}

MMat4::MMat4(const float all)
{
    for(size_t i = 0; i < 4; ++i)
    {
        for(size_t j = 0; j < 4; ++j)
        {
            matrix[i][j] = all;
        }
    }
}

MMat4::MMat4(const MMat4 &m)
{
    for(size_t i = 0; i < 4; ++i)
    {
        for(size_t j = 0; j < 4; ++j)
        {
            matrix[i][j] = m.matrix[i][j];
        }
    }
}

MMat4 &MMat4::operator*=(const MMat4 &mat4)
{
    for(size_t i = 0; i < 4; ++i)
    {
        for(size_t j = 0; j < 4; ++j)
        {
            this->matrix[i][j] = this->matrix[i][0] * mat4.matrix[0][j]
                    + this->matrix[i][1] * mat4.matrix[1][j]
                    + this->matrix[i][2] * mat4.matrix[2][j]
                    + this->matrix[i][3] * mat4.matrix[3][j];
        }
    }

    return *this;
}

MMat4 rotation(const MMat4 &mat, const MVec3 &vec3)
{
    MMat4 temp = makeIdentityMatrix();

    if(vec3.x != 0.0f)
    {
        MMat4 rota = makeIdentityMatrix();
        rota.matrix[1][1] = cos(vec3.x);
        rota.matrix[1][2] = -sin(vec3.x);
        rota.matrix[2][1] = sin(vec3.x);
        rota.matrix[2][2] = cos(vec3.x);
        temp = rota * temp;
    }

    if(vec3.y != 0.0f)
    {
        MMat4 rota = makeIdentityMatrix();
        rota.matrix[0][0] = cos(vec3.y);
        rota.matrix[2][0] = -sin(vec3.y);
        rota.matrix[0][2] = sin(vec3.y);
        rota.matrix[2][2] = cos(vec3.y);
        temp = rota * temp;
    }

    if(vec3.z != 0.0f)
    {
        MMat4 rota = makeIdentityMatrix();
        rota.matrix[0][0] = cos(vec3.z);
        rota.matrix[0][1] = -sin(vec3.z);
        rota.matrix[1][0] = sin(vec3.z);
        rota.matrix[1][1] = cos(vec3.z);
        temp = rota * temp;
    }

    return temp * mat;
}

MMat4 scale(const MMat4 &mat, const MVec3 &vec3)
{
    MMat4 temp = makeIdentityMatrix();

    if(vec3.x != 0.0f)
    {
        temp.matrix[0][0] = vec3.x;
    }

    if(vec3.y != 0.0f)
    {
        temp.matrix[1][1] = vec3.y;
    }

    if(vec3.z != 0.0f)
    {
        temp.matrix[2][2] = vec3.z;
    }

    return temp * mat;
}

MMat4 projective(float f)
{
    MMat4 temp = makeIdentityMatrix();
    temp.matrix[3][2] = -1 / f;

    return temp;
}

MMat4 MMat4::operator*(const MMat4 &mat4) const
{
    auto temp = *this;
    return temp *= mat4;
}

void MMat4::identityMatrix()
{
    for(size_t i = 0; i < 4; ++i)
    {
        for(size_t j = 0; j < 4; ++j)
        {
            if(i != j)
                matrix[i][j] = 0;
            else
                matrix[i][j] = 1;
        }
    }
}
