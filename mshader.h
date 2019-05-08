#ifndef MSHADER_H
#define MSHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
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
    void setUniform3F(const string &name, float value1, float value2, float value3) const;
    void setUniform3F(const string &name, glm::vec3 vec) const;
    void setUniforMatrix4fv(float *ptr, const char *name);
    void setUniformMatrix3fv(float *ptr, const char *name, bool transpose = false);

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

inline void MShader::setUniform3F(const std::string &name, float value1, float value2, float value3) const
{
    int loc = glGetUniformLocation(shaderProgramID, name.c_str());
    if(loc != -1)
        glUniform3f(loc, value1, value2, value3);
    else
        std::cerr << "No find uniform location of the " << name.c_str() << std::endl;
}

inline void MShader::setUniform3F(const std::string &name, glm::vec3 vec) const
{
    setUniform3F(name, vec.x, vec.y, vec.z);
}

inline void MShader::setUniforMatrix4fv(float *ptr, const char *name)
{
    int loc = glGetUniformLocation(shaderProgramID, name);
    if(loc != -1)
        glUniformMatrix4fv(loc, 1, GL_FALSE, ptr);
    else
        std::cerr << "No find uniform location " << name << std::endl;
}

inline void MShader::setUniformMatrix3fv(float *ptr, const char *name, bool transpose)
{
    int loc = glGetUniformLocation(shaderProgramID, name);
    if(loc != -1)
    {
        if(transpose)
            glUniformMatrix3fv(loc, 1, GL_TRUE, ptr);
        else
            glUniformMatrix3fv(loc, 1, GL_FALSE, ptr);
    }
    else
        std::cerr << "No find uniform location " << name << std::endl;
}

#endif // MSHADER_H
