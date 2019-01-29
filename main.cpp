#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow *, int width, int height);
void processInput(GLFWwindow *window);

//Windows size
constexpr unsigned SCR_WIDTH = 800;
constexpr unsigned SCR_HEIGHT = 600;

int main()
{
    //初始化设置GLFW
    glfwInit();//初始化GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//set主版本号
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//set副版本号
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//use核心模式

    //窗口对象，存放了所有和窗口相关的数据
    GLFWwindow *window = glfwCreateWindow(800, 600, "Learn OpenGl", nullptr, nullptr);
    if(window == nullptr)//检查窗口创建是否成功
    {
        std::cout << "Failed!" << std::endl;
        glfwTerminate();
        return -1;
    }
    //通知GLFW将该窗口的上下文设置为当前线程的主上下文
    glfwMakeContextCurrent(window);

    //初始化GLAD，glfwGetProcAddress是GLAD用来加载系统相关的OpenGL函数指针地址的函数
    if(!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //设置窗口（视口）的维度
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);//lower left(-1 ~ 1)
    //对窗口注册一个回调函数，它在每次窗口大小被调整的时候被调用
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //设置清空屏幕所用的颜色（底色）
    glClearColor(0.1f, 0.2f, 0.2f, 1.0f);

    //创建一个顶点着色器对象，以ID来引用
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //着色器语言GLSL(OpenGL Shading Language)编写顶点着色器，存储在字符串中
    const GLchar *vertexShaderSource = "#version 330 core\n"
            "layout(location = 0) in vec3 aPos;\n"
            "void main()\n"
            "{\n"
            "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
            "}\0";
    //把顶点着色器源码附加到着色器对象上，然后编译它
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    //检测在调用glCompileShader后编译是否成功
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        //获取错误消息
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //创建一个片段着色器对象，以ID来引用
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    //着色器语言GLSL(OpenGL Shading Language)编写片段着色器，存储在字符串中
    const GLchar *fragmentShaderSource = "#version 330 core\n"
            "out vec4 fragColor;\n"
            "void main()\n"
            "{\n"
            "fragColor = vec4(1.0f, 0.5f, 0.0f, 1.0f);\n"
            "}\0";
    //把片段着色器源码附加到着色器对象上，然后编译它
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    //检测在调用glCompileShader后编译是否成功
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        //获取错误消息
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //创建一个程序对象
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    //着色器附加到程序对象上，并用glLinkProgram链接它们
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    //检测链接着色器程序是否失败
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success)
    {
        //获取错误消息
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //删除已无需使用着色器对象
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //以标准化设备坐标指定多个顶点
    float vertices[] = {
        0.8f, 0.5f, 0.0f,
        0.0f, 0.5f, 0.0f,
        -0.8f, 0.5f, 0.0f,
        0.0f, -0.5f, 0.0f
    };
    //指定绘制的索引顺序
    unsigned int indices[] = {
        0, 1, 3, // 第一个三角形
        1, 2, 3  // 第二个三角形
    };
    //一个以缓冲ID生成一个VBO和VAO、EBO对象，第一个参数为第二参数指向多少个连续对象
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    //绑定VAO，VBO缓冲绑定到GL_ARRAY_BUFFER，EBO缓冲绑定到GL_ELEMENT_ARRAY_BUFFER
    glBindVertexArray(VAO);//记录以下操作
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //顶点数据复制到缓冲的内存中
    glBufferData(GL_ARRAY_BUFFER, sizeof (vertices), vertices, GL_STATIC_DRAW);
    //索引数据复制到缓冲的内存中
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    //告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上）
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof (float), nullptr);
    glEnableVertexAttribArray(0);
    //显式解绑VAO
    glBindVertexArray(0);

    //渲染循环
    while(!glfwWindowShouldClose(window))
    {
        //输入----
        //ESC键退出
        processInput(window);

        //渲染指令----
        glClear(GL_COLOR_BUFFER_BIT);//清屏
        //指定绘制模式为线框模式，默认为(GL_FRONT_AND_BACK, GL_FILL)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        //指定着色器程序对象
        glUseProgram(shaderProgram);
        //指定该VAO中的解析顶点指针和存在的EBO
        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 6);//VBO内存储顶点顺序绘制
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);//以EBO存储的索引顺序绘制
        glBindVertexArray(0);

        //检查并调用事件，交换缓冲----
        //交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲），它在这一迭代中被用来绘制
        glfwSwapBuffers(window);
        //检查是否触发事件
        glfwPollEvents();
    }

    //正确释放/删除之前分配的所有资源
    glfwTerminate();

    return 0;
}

void framebuffer_size_callback(GLFWwindow *, int width, int height)
{
    //重置视口大小
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    //检查用户是否按下了返回键(Esc)，若是则退出
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
