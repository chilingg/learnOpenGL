#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>
#include <fstream>
#include "mshader.h"
#include "model3d.h"
//#include "mmatrix.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Window size
constexpr float SCR_WIDTH = 800;
constexpr float SCR_HEIGHT = 600;

static glm::vec3 cameraPos = { 0.0f, 0.0f, 0.0f };//摄像机平移
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

int createCube(unsigned *lightVAO, unsigned *lightVBO);

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
    //启用半透明混合
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//设置源与目标的混个因子
    //glDisable(GL_BLEND);

    //创建点光源
    unsigned lightVAO, lightVBO;
    int pointeNumber;
    pointeNumber = createCube(&lightVAO, &lightVBO);

    //不透明玻璃
    float vertices[] = {
        // positions
        -0.5f, -0.5f,  0.0f,
         0.5f, -0.5f,  0.0f,
         0.5f,  0.5f,  0.0f,
         0.5f,  0.5f,  0.0f,
        -0.5f,  0.5f,  0.0f,
        -0.5f, -0.5f,  0.0f,
    };

    MShader myShader("../learnOpenGL/shader/vertex.vert",
                     "../learnOpenGL/shader/fragment.frag");
    MShader lightShader("../learnOpenGL/shader/vertex.vert",
                     "../learnOpenGL/shader/lamp.frag");
    MShader transparentShader("../learnOpenGL/shader/vertex.vert",
                     "../learnOpenGL/shader/transparent.frag");

    Model3d mModel("../learnOpenGL/model/nanosuit/nanosuit.obj");

    //视图矩阵
    glm::mat4 view(1.0f);
    //投影矩阵
    glm::mat4 projection(1.0f);
    //变换矩阵
    glm::mat4 model(1.0f);
    //法线矩阵
    glm::mat3 normalMat(1.0f);

    glm::vec3 ambientStrength;//环境光量
    glm::vec3 diffuse;//漫反射量（保持本身颜色）
    myShader.use();
    //平行光
    glm::vec3 lightStrength(0.3f, 0.3f, 0.3f);//高光（光源）量
    ambientStrength = lightStrength * 0.3f;//环境光量
    diffuse = lightStrength - ambientStrength;//漫反射量（保持本身颜色）
    glm::vec3 parallelDir(-0.2f, -1.0f, -0.3f);//平行光源方向
    myShader.setUniform3F("SunLight.direction", parallelDir);
    myShader.setUniform3F("SunLight.ambient", ambientStrength);
    myShader.setUniform3F("SunLight.diffuse",  diffuse);
    myShader.setUniform3F("SunLight.specular", lightStrength);
    //点光源
    glm::vec3 pointlightStrength = glm::vec3(1.0f, 1.0f, 1.0f);
    ambientStrength = pointlightStrength * 0.1f;//环境光量
    diffuse = pointlightStrength - ambientStrength;//漫反射量（保持本身颜色）
    glm::vec3 lightPos(0.0f, 0.0f, -1.0f);//光源坐标
    myShader.setUniform1F("LuminousBody.constant", 1.0f);
    myShader.setUniform1F("LuminousBody.linear", 0.09f);
    myShader.setUniform1F("LuminousBody.quadratic", 0.032f);
    myShader.setUniform3F("LuminousBody.ambient", ambientStrength);
    myShader.setUniform3F("LuminousBody.diffuse",  diffuse);
    myShader.setUniform3F("LuminousBody.specular", pointlightStrength);
    myShader.setUniform3F("LuminousBody.position", lightPos);

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

        //移动光源
        //lightPos.x = static_cast<float>(sin(glfwGetTime()));

        //指定着色器程序对象

        projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), SCR_WIDTH/SCR_HEIGHT, 0.1f, 100.f);

        view = glm::mat4(1.0f);
        view = glm::rotate(view, rotateView.x, {1.0f, 0.0f, 0.0f});
        view = glm::rotate(view, rotateView.y, {0.0f, 1.0f, 0.0f});
        view = glm::translate(view, -cameraPos);

        //剔除背面（仅针对闭合形状）
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);//定义顺时针的面为正向面

        //绘制模型
        myShader.use();
        myShader.setUniform3F("LuminousBody.position", lightPos);
        myShader.setUniform3F("CameraPos", cameraPos);
        myShader.setUniforMatrix4fv(glm::value_ptr(projection), "projection");
        myShader.setUniforMatrix4fv(glm::value_ptr(view), "view");
        model = glm::mat4(1.0f);
        model = glm::translate(model, {0.0f, -2.0f, -3.0f});
        model = glm::scale(model, glm::vec3(0.2f));
        myShader.setUniforMatrix4fv(glm::value_ptr(model), "model");
        myShader.setUniform1F("OneMaterial.shininess", 32.0f);
        mModel.draw(myShader);

        //绘制灯
        lightShader.use();
        lightShader.setUniform3F("lightColor", 0.3f+pointlightStrength);
        lightShader.setUniforMatrix4fv(glm::value_ptr(projection), "projection");
        lightShader.setUniforMatrix4fv(glm::value_ptr(view), "view");
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        lightShader.setUniforMatrix4fv(glm::value_ptr(model), "model");
        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, pointeNumber);//VBO内存储顶点顺序绘制
        glBindVertexArray(0);

        //取消背面剔除
        glDisable(GL_CULL_FACE);

        //检查并调用事件，交换缓冲----
        //交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲），它在这一迭代中被用来绘制
        glfwSwapBuffers(window);
        //检查是否触发事件
        glfwPollEvents();
    }

    //正确释放/删除之前分配的所有资源
    glDeleteBuffers(1, &lightVBO);
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
        cameraPos.z -= cosf(rotateView.y) * cameraSpeed;
        cameraPos.x += sinf(rotateView.y) * cameraSpeed;
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        cameraPos.z += cosf(rotateView.y) * cameraSpeed;
        cameraPos.x -= sinf(rotateView.y) * cameraSpeed;
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        cameraPos.z -= sinf(rotateView.y) * cameraSpeed;
        cameraPos.x -= cosf(rotateView.y) * cameraSpeed;
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        cameraPos.z += sinf(rotateView.y) * cameraSpeed;
        cameraPos.x += cosf(rotateView.y) * cameraSpeed;
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

int createCube(unsigned *lightVAO, unsigned *lightVBO)
{
    //以标准化设备坐标指定多个顶点
    float vertices[] = {
        // positions
        // Back face
        -0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        // Front face
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        // Left face
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        // Right face
         0.5f,  0.5f,  0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        // Bottom face
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,
        // Top face
        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f,  0.5f,
    };
    //管理光源的VAO
    glGenVertexArrays(1, lightVAO);
    glGenBuffers(1, lightVBO);
    glBindVertexArray(*lightVAO);//记录以下操作
    glBindBuffer(GL_ARRAY_BUFFER, *lightVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //设置灯的顶点属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), nullptr);//位置属性
    glEnableVertexAttribArray(0);
    //显式解绑VAO
    glBindVertexArray(0);

    return sizeof(vertices)/sizeof(float)/3;
}
