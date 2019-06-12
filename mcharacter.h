#ifndef MCHARACTER_H
#define MCHARACTER_H

#include <ft2build.h>
#include FT_FREETYPE_H
#include <string>
#include <map>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include "mshader.h"

class MCharacter
{
public:
    struct Character {
        GLuint     TextureID;  // 字形纹理的ID
        glm::ivec2 Size;       // 字形大小
        glm::ivec2 Bearing;    // 从基准线到字形左部/顶部的偏移值
        GLint     Advance;    // 原点距下一个字形原点的距离
    };

    MCharacter(const char *fontFile);
    void RenderText(MShader &s, std::string text, glm::vec2 coord, GLfloat scale, glm::vec3 color);

private:
    std::map<GLchar, Character> Characters;
    GLuint VAO, VBO;
};

#endif // MCHARACTER_H
