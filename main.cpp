#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

int main()
{
    glfwInit();//初始化
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//set主版本号
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//set副版本号
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//use核心模式

    GLFWwindow *window = glfwCreateWindow(800, 600, "Learn OpenGl", nullptr, nullptr);
    if(window == nullptr)
    {
        std::cout << "Failed!" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        GLADloadproc i =static_cast<GLADloadproc>(glfwGetProcAddress);
        gladLoadGLLoader(i);
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    return 0;
}
