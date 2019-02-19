#ifndef MMATRIX_H
#define MMATRIX_H

#include <cstddef>
#include "mvec.h"

constexpr float M_PI_F = 3.1415926f;

class MMat4
{
    friend MMat4 translate(const MMat4 &mat, const MVec3 &vec3);
    friend MMat4 rotation(const MMat4 &mat, const MVec3 &vec3);
    friend MMat4 scale(const MMat4 &mat, const MVec3 &vec3);
    friend MMat4 projective(float f);
    friend MVec4 operator*(const MMat4 &mat4, const MVec4 &vec4);

public:
    MMat4(const float all = 0.0f);
    MMat4(const MMat4 &m);
    MMat4 &operator*=(const MMat4 &mat4);
    MMat4 operator*(const MMat4 &mat4) const;
    auto operator[](const size_t &t) const -> const float(&) [4];

    const float *matrixPtr() const;
    void identityMatrix();

private:
    float matrix[4][4];
};

MMat4 translate(const MMat4 &mat, const MVec3 &vec3);
MMat4 rotation(const MMat4 &mat, const MVec3 &vec3);
MMat4 scale(const MMat4 &mat, const MVec3 &vec3);
MMat4 projective(float f);
void printMat(const MMat4& mat);

inline auto MMat4::operator[](const size_t &t) const -> const float(&) [4]
{
    return this->matrix[t];
}

inline const float *MMat4::matrixPtr() const
{
    return *matrix;
}

inline MMat4 makeIdentityMatrix()
{
    MMat4 im;
    im.identityMatrix();
    return im;
}

inline float radians(float degrees)
{
    return degrees / 180.0f * M_PI_F;
}

#endif // MMATRIX_H
