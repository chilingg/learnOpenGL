#ifndef MMATRIX_H
#define MMATRIX_H

#include <utility>
#include <cstddef>
#include "mvec.h"

class MMat4
{
    friend MMat4 translate(MMat4 mat, const MVec3 &vec3);
    friend MVec4 operator*(MMat4 &mat4, MVec4 &vec4);

public:
    MMat4(float f = 0.0f);
    MMat4(const MMat4 &m);
    MMat4(MMat4 &&m);

    auto getMatrixPtr() const -> const float(*)[4];
    void identityMatrix();

private:
    float (*matrix)[4];
};

inline MMat4 translate(MMat4 mat, const MVec3 &vec3)
{
    mat.matrix[3][0] += vec3.x;
    mat.matrix[3][1] += vec3.y;
    mat.matrix[3][2] += vec3.z;

    return mat;
}

MVec4 operator*(MMat4 &mat4, MVec4 &vec4)
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

MMat4::MMat4(float f):
    matrix(new float[4][4])
{
    for(size_t i = 0; i < 4; ++i)
    {
        for(size_t j = 0; j < 4; ++j)
        {
            matrix[i][j] = f;
        }
    }
}

MMat4::MMat4(const MMat4 &m)
{
    auto p = m.getMatrixPtr();
    for(size_t i = 0; i < 4; ++i)
    {
        for(size_t j = 0; j < 4; ++j)
        {
            matrix[i][j] = p[i][j];
        }
    }
}

MMat4::MMat4(MMat4 &&m)
{
    this->matrix = m.matrix;
}

inline auto MMat4::getMatrixPtr() const -> const float(*)[4]
{
    return matrix;
}

void MMat4::identityMatrix()
{
    for(size_t i = 0; i < 4; ++i)
    {
        matrix[i][i] = 1;
    }
}

#endif // MMATRIX_H
