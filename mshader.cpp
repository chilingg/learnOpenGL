#include "mshader.h"
#include <fstream>

MShader::MShader(const GLchar *vertexPath, const GLchar *fragmentPath, const GLchar *geometryPath)
{
    //创建着色器程序
    ID = glCreateProgram();
    int  success;
    char infoLog[512];

    std::string vertexCode = openShaderFile(vertexPath);
    const char* vShaderCode = vertexCode.c_str();
    //创建编译顶点着色器
    unsigned vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vShaderCode, nullptr);
    glCompileShader(vertexShader);
    // 打印编译错误（如果有的话）
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    std::string fragmentCode = openShaderFile(fragmentPath);
    const char* fShaderCode = fragmentCode.c_str();
    //创建编译片段着色器
    unsigned fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShaderCode, nullptr);
    glCompileShader(fragmentShader);
    // 打印编译错误（如果有的话）
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned geometryShader = 0;
    if(geometryPath)
    {
        std::string geometryCode = openShaderFile(geometryPath);
        const char* gShaderCode = geometryCode.c_str();
        //创建编译几何着色器
        geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometryShader, 1, &gShaderCode, nullptr);
        glCompileShader(geometryShader);
        // 打印编译错误（如果有的话）
        glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(geometryShader, 512, nullptr, infoLog);
            std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        glAttachShader(ID, geometryShader);
    }

    //连接顶点&片段着色器
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);
    // 打印编译错误（如果有的话）
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(ID, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;

    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    if(geometryPath)
        glDeleteShader(geometryShader);
}

std::string MShader::openShaderFile(const GLchar *path)
{
    std::string code;
    std::ifstream shaderFile;
    //若状态被置为failbit或badbit，则抛出异常
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        //打开文件
        shaderFile.open(path);
        //读取文件的缓冲内容到数据流中
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        //关闭文件处理器
        shaderFile.close();
        //转换数据流到string
        code = shaderStream.str();
    }
    catch(std::ifstream::failure &)
    {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    return code;
}
