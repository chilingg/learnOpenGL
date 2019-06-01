#ifndef MSHADER_H
#define MSHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class MShader
{
public:
    MShader(const GLchar* vertexPath, const GLchar* fragmentPath);
    void use() const;//激活并使用
    void setUniform1F(const std::string &name, float value) const;
    void setUniform1I(const std::string &name, int value) const;
    void setUniform3F(const std::string &name, float value1, float value2, float value3) const;
    void setUniform3F(const std::string &name, glm::vec3 vec) const;
    void setUniformMatrix4fv(float *ptr, const char *name) const;
    void setUniformMatrix3fv(float *ptr, const char *name, bool transpose = false) const;

    unsigned int ID;
};

inline void MShader::use() const
{
    glUseProgram(ID);
}

inline void MShader::setUniform1F(const std::string &name, float value) const
{
    int loc = glGetUniformLocation(ID, name.c_str());
    if(loc != -1)
        glUniform1f(loc, value);
    else
        std::cerr << "No find uniform location of the " << name.c_str() << std::endl;
}

inline void MShader::setUniform1I(const std::string &name, int value) const
{
    int loc = glGetUniformLocation(ID, name.c_str());
    if(loc != -1)
        glUniform1i(loc, value);
    else
        std::cerr << "No find uniform location of the " << name.c_str() << std::endl;
}

inline void MShader::setUniform3F(const std::string &name, float value1, float value2, float value3) const
{
    int loc = glGetUniformLocation(ID, name.c_str());
    if(loc != -1)
        glUniform3f(loc, value1, value2, value3);
    else
        std::cerr << "No find uniform location of the " << name.c_str() << std::endl;
}

inline void MShader::setUniform3F(const std::string &name, glm::vec3 vec) const
{
    setUniform3F(name, vec.x, vec.y, vec.z);
}

inline void MShader::setUniformMatrix4fv(float *ptr, const char *name) const
{
    int loc = glGetUniformLocation(ID, name);
    if(loc != -1)
        glUniformMatrix4fv(loc, 1, GL_FALSE, ptr);
    else
        std::cerr << "No find uniform location " << name << std::endl;
}

inline void MShader::setUniformMatrix3fv(float *ptr, const char *name, bool transpose) const
{
    int loc = glGetUniformLocation(ID, name);
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
