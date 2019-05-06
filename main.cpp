#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>
#include <fstream>
#include "mshader.h"
#include "stb_image_implementation.h"
//#include "mmatrix.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Window size
constexpr float SCR_WIDTH = 800;
constexpr float SCR_HEIGHT = 600;

static glm::vec3 cameraMove = { 0.0f, 0.0f, 0.0f };//摄像机平移
static float cameraSpeed = 0.05f;//移动速度
static float deltaTime = 0.0f; // 当前帧与上一帧的时间差
static float lastTime= 0.0f; // 上一帧的时间
static float currentTime = 0.0f;//当前时间

static glm::vec3 rotateView = {0.0f, 0.0f, 0.0f};
static float sensitivity = 0.001f;
static float oldX = SCR_WIDTH / 2;
static float oldY = SCR_HEIGHT / 2;

static bool firstMouse = true;//定位第一次移动时的光标位置

void framebuffer_size_callback(GLFWwindow *, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

int main()
{
    //初始化设置GLFW
    glfwInit();//初始化GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//set主版本号
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//set副版本号
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//use核心模式

    //窗口对象，存放了所有和窗口相关的数据
    GLFWwindow *window = glfwCreateWindow(static_cast<int>(SCR_WIDTH), static_cast<int>(SCR_HEIGHT),
                                          "Learn OpenGl", nullptr, nullptr);
    if(window == nullptr)//检查窗口创建是否成功
    {
        std::cerr << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        return -1;
    }
    //通知GLFW将该窗口的上下文设置为当前线程的主上下文
    glfwMakeContextCurrent(window);
    //对窗口注册一个回调函数，它在每次窗口大小被调整的时候被调用
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //注册一个鼠标移动事件回调函数
    glfwSetCursorPosCallback(window,mouse_callback);
    //隐藏并捕捉光标
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //初始化GLAD，glfwGetProcAddress是GLAD用来加载系统相关的OpenGL函数指针地址的函数
    if(!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    //设置窗口（视口）的维度
    glViewport(0, 0, static_cast<int>(SCR_WIDTH), static_cast<int>(SCR_HEIGHT));//lower left(-1 ~ 1)
    //设置清空屏幕所用的颜色（底色）
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    //忽略不必要的Z轴片段
    glEnable(GL_DEPTH_TEST);

    //以标准化设备坐标指定多个顶点
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };
    //一个以缓冲ID生成一个VBO和VAO对象，第一个参数为第二参数指向多少个连续对象
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    //绑定VAO，VBO缓冲绑定到GL_ARRAY_BUFFER，EBO缓冲绑定到GL_ELEMENT_ARRAY_BUFFER
    glBindVertexArray(VAO);//记录以下操作
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //顶点数据复制到缓冲的内存中
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上）
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof (float), nullptr);//位置属性
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof (float), reinterpret_cast<void*>(3*sizeof(float)));//位置属性
    glEnableVertexAttribArray(1);
    //显式解绑VAO
    glBindVertexArray(0);

    //管理光源的VAO
    unsigned lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    //使用相同的顶点缓冲对象
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //设置灯的顶点属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof (float), nullptr);//位置属性
    glEnableVertexAttribArray(0);
    //显式解绑VAO
    glBindVertexArray(0);

    //光源坐标
    glm::vec3 lightPos(1.2f, 1.0f, -4.0f);
    MShader myShader("../learnOpenGL/shader/vertex.vert",
                     "../learnOpenGL/shader/fragment.frag");
    //glUniform3f(glGetUniformLocation(myShader.shaderProgramID, "objectColor"), 1.0f, 0.5f, 0.31f); // 手动设置

    MShader lightShader("../learnOpenGL/shader/vertex.vert",
                     "../learnOpenGL/shader/lamp.frag");

    //视图矩阵
    glm::mat4 view(1.0f);
    //投影矩阵
    glm::mat4 projection(1.0f);
    //变换矩阵
    glm::mat4 model(1.0f);
    //法线矩阵
    glm::mat3 normalMat(1.0f);

    //渲染循环
    while(!glfwWindowShouldClose(window))
    {
        //输入----
        //ESC键退出
        processInput(window);

        //渲染指令----
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清屏 清除颜色缓冲和深度缓冲

        //修正视图移动速度
        currentTime = static_cast<float>(glfwGetTime());
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        cameraSpeed = 2.5f * deltaTime;

        //指定着色器程序对象
        myShader.use();
        myShader.setUniform3F("lightPos", lightPos);
        myShader.setUniform3F("lightColor", 1.0f, 1.0f, 1.0f);

        projection = glm::mat4(1.0);
        projection = glm::perspective(glm::radians(45.0f), SCR_WIDTH/SCR_HEIGHT, 0.1f, 100.f);
        myShader.setUniforMatrix4fv(glm::value_ptr(projection), "projection");

        view = glm::mat4(1.0);
        view = glm::rotate(view, rotateView.x, {1.0f, 0.0f, 0.0f});
        view = glm::rotate(view, rotateView.y, {0.0f, 1.0f, 0.0f});
        view = glm::translate(view, cameraMove);
        myShader.setUniforMatrix4fv(glm::value_ptr(view), "view");

        model = glm::mat4(1.0);
        model = glm::translate(model, {0.0f, -1.0f, -4.0f});
        myShader.setUniforMatrix4fv(glm::value_ptr(model), "model");

        normalMat = glm::mat3(model);
        myShader.setUniforMatrix3fv(glm::value_ptr(normalMat), "normalMat");

        //指定该VAO中的解析顶点指针和存在的EBO
        glBindVertexArray(VAO);
        //glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(unsigned), GL_UNSIGNED_INT, nullptr);//以EBO存储的索引顺序绘制三角形
        myShader.setUniform3F("objectColor", 1.0f, 0.5f, 0.3f);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices)/sizeof(float)/2);//VBO内存储顶点顺序绘制

        //绘制灯
        lightShader.use();
        lightShader.setUniforMatrix4fv(glm::value_ptr(projection), "projection");
        lightShader.setUniforMatrix4fv(glm::value_ptr(view), "view");
        model = glm::mat4(1.0);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        lightShader.setUniforMatrix4fv(glm::value_ptr(model), "model");
        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices)/sizeof(float));//VBO内存储顶点顺序绘制

        //检查并调用事件，交换缓冲----
        //交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲），它在这一迭代中被用来绘制
        glfwSwapBuffers(window);
        //检查是否触发事件
        glfwPollEvents();
    }

    //正确释放/删除之前分配的所有资源
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &lightVAO);
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

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        cameraMove.z += cosf(rotateView.y) * cameraSpeed;
        cameraMove.x -= sinf(rotateView.y) * cameraSpeed;
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        cameraMove.z -= cosf(rotateView.y) * cameraSpeed;
        cameraMove.x += sinf(rotateView.y) * cameraSpeed;
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        cameraMove.z += sinf(rotateView.y) * cameraSpeed;
        cameraMove.x += cosf(rotateView.y) * cameraSpeed;
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        cameraMove.z -= sinf(rotateView.y) * cameraSpeed;
        cameraMove.x -= cosf(rotateView.y) * cameraSpeed;
    }
}

void mouse_callback(GLFWwindow*, double xpos, double ypos)
{
    float newX = static_cast<float>(xpos);
    float newY = static_cast<float>(ypos);

    //校正第一次鼠标位置
    if(firstMouse)
    {
        oldX = newX;
        oldY = newY;
        firstMouse = false;
    }

    rotateView.y += (newX - oldX) * sensitivity;
    rotateView.x += (newY - oldY) * sensitivity;

    //仰角限制
    if(rotateView.x > 0.5f)
        rotateView.x = 0.5f;
    if(rotateView.x < -0.5f)
        rotateView.x = -0.5f;

    oldX = newX;
    oldY = newY;
}
