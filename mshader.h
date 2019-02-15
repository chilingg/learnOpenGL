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
    void setUniform1I(const string &name, int value) const;

    unsigned int shaderProgramID;
};

inline void MShader::use()
{
    glUseProgram(shaderProgramID);
}

inline void MShader::setUniform1F(const std::string &name, float value) const
{
    int loc = glGetUniformLocation(shaderProgramID, name.c_str());
    if(loc != -1)
        glUniform1f(loc, value);
    else
        std::cerr << "No find uniform location of the " << name.c_str() << std::endl;
}

inline void MShader::setUniform1I(const std::string &name, int value) const
{
    int loc = glGetUniformLocation(shaderProgramID, name.c_str());
    if(loc != -1)
        glUniform1i(loc, value);
    else
        std::cerr << "No find uniform location of the " << name.c_str() << std::endl;
}

#endif // MSHADER_H
