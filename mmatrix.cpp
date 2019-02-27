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
    MMat4 temp(*this);
    for(size_t i = 0; i < 4; ++i)
    {
        for(size_t j = 0; j < 4; ++j)
        {
            this->matrix[i][j] = temp.matrix[i][0] * mat4.matrix[0][j]
                    + temp.matrix[i][1] * mat4.matrix[1][j]
                    + temp.matrix[i][2] * mat4.matrix[2][j]
                    + temp.matrix[i][3] * mat4.matrix[3][j];
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
        rota.matrix[1][1] = cosf(vec3.x);
        rota.matrix[1][2] = -sinf(vec3.x);
        rota.matrix[2][1] = sinf(vec3.x);
        rota.matrix[2][2] = cosf(vec3.x);
        temp = rota * temp;
    }

    if(vec3.y != 0.0f)
    {
        MMat4 rota = makeIdentityMatrix();
        rota.matrix[0][0] = cosf(vec3.y);
        rota.matrix[2][0] = -sinf(vec3.y);
        rota.matrix[0][2] = sinf(vec3.y);
        rota.matrix[2][2] = cosf(vec3.y);
        temp = rota * temp;
    }

    if(vec3.z != 0.0f)
    {
        MMat4 rota = makeIdentityMatrix();
        rota.matrix[0][0] = cosf(vec3.z);
        rota.matrix[0][1] = -sinf(vec3.z);
        rota.matrix[1][0] = sinf(vec3.z);
        rota.matrix[1][1] = cosf(vec3.z);
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
    temp.matrix[2][2] = -1;
    temp.matrix[3][2] = -1 / f;

    return temp;
}

MMat4 lookAt(const MMat4 &mat, MVec3 target)
{
    MMat4 lookAtMat = makeIdentityMatrix();

    MVec4 targetPos = {target.x, target.y, target.z, 1.0f};
    targetPos = mat * targetPos;
    std::cout << '\n' << targetPos.x << targetPos.y << targetPos.z << std::endl;

    float rotateX = 0.0f;
    float rotateY = 0.0f;
    if(targetPos.y != 0.0f)
    {
        rotateX = radians(90.0f) - atanf(targetPos.z / targetPos.y);
        std::cout << "rotateX\t" << degrees(rotateX) << std::endl;
    }
    targetPos = rotation(lookAtMat, {rotateX, 0.0f, 0.0f}) * targetPos;
    std::cout << targetPos.x << targetPos.y << targetPos.z << std::endl;

    if(targetPos.x != 0.0f)
    {
        rotateY = radians(90.0f) + atanf(targetPos.z / targetPos.x);
        std::cout << "rotateY\t" << degrees(rotateY) << std::endl;
    }
    targetPos = rotation(lookAtMat, {0.0f, rotateY, 0.0f}) * targetPos;
    std::cout << targetPos.x << targetPos.y << targetPos.z << std::endl;

    lookAtMat = rotation(mat, {rotateX, rotateY, 0.0f});
    if(targetPos.z > 0.0f)
        lookAtMat = rotation(mat, {0.0f, radians(180.0f), 0.0f});
    //std::cout << targetPos.x << targetPos.y << targetPos.z << std::endl;

    return lookAtMat;
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

MMat4 camera(const MMat4 &mat, const MVec3 &vec3)
{
    MMat4 idt;
    idt.identityMatrix();
    idt.matrix[0][3] -= vec3.x;
    idt.matrix[1][3] -= vec3.y;
    idt.matrix[2][3] -= vec3.z;

    return idt * mat;
}
