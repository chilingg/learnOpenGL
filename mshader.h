#ifndef MSHADER_H
#define MSHADER_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using std::string;
using std::ifstream;
using std::stringstream;

class MShader
{
public:
    MShader(const GLchar* vertexPath, const GLchar* fragmentPath);
    void use();//激活并使用
    void setUniform1F(const string &name, float value) const;

    unsigned int shaderProgramID;
};

inline void MShader::use()
{
    glUseProgram(shaderProgramID);
}

inline void MShader::setUniform1F(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(shaderProgramID, name.c_str()), value);
}

#endif // MSHADER_H
