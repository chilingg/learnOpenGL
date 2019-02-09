#ifndef MMATRIX_H
#define MMATRIX_H

#include <cstddef>
#include "mvec.h"

class MMat4
{
    friend MMat4 translate(const MMat4 &mat, const MVec3 &vec3);
    friend MVec4 operator*(const MMat4 &mat4, const MVec4 &vec4);
    friend MVec4 operator*(const MVec4 &vec4, const MMat4 &mat4);

public:
    MMat4(const float all = 0.0f);
    MMat4(const MMat4 &m);
    MMat4 &operator*=(const MMat4 &mat4);

    auto getMatrixPtr() const -> const float(*)[4];
    void identityMatrix();

private:
    float matrix[4][4];
};

inline MMat4 translate(const MMat4 &mat, const MVec3 &vec3)
{
    MMat4 idt;
    idt.identityMatrix();
    idt.matrix[3][0] += vec3.x;
    idt.matrix[3][1] += vec3.y;
    idt.matrix[3][2] += vec3.z;

    idt *= mat;
    return idt;
}

MVec4 operator*(const MMat4 &mat4, const MVec4 &vec4)
{
    float x = vec4.x * mat4.matrix[0][0]
            + vec4.y * mat4.matrix[1][0]
            + vec4.z * mat4.matrix[2][0]
            + vec4.w * mat4.matrix[3][0];

    float y = vec4.x * mat4.matrix[0][1]
            + vec4.y * mat4.matrix[1][1]
            + vec4.z * mat4.matrix[2][1]
            + vec4.w * mat4.matrix[3][1];

    float z = vec4.x * mat4.matrix[0][2]
            + vec4.y * mat4.matrix[1][2]
            + vec4.z * mat4.matrix[2][2]
            + vec4.w * mat4.matrix[3][2];

    float w = vec4.x * mat4.matrix[0][3]
            + vec4.y * mat4.matrix[1][3]
            + vec4.z * mat4.matrix[2][3]
            + vec4.w * mat4.matrix[3][3];

    return {x, y, z, w};
}

inline MVec4 operator*(const MVec4 &vec4, const MMat4 &mat4)
{
    return operator*(mat4, vec4);
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

inline auto MMat4::getMatrixPtr() const -> const float(*)[4]
{
    return matrix;
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

#endif // MMATRIX_H
