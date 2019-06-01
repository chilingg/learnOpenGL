#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>
#include <fstream>
#include "mshader.h"
#include "model3d.h"
#include "stb_image.h"
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

int createCube(unsigned *VAO, unsigned *VBO);
unsigned loadCubmap(std::vector<std::string> faces);

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
    glDepthFunc(GL_LEQUAL);//小于或等于时通过
    //启用半透明混合
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//设置源与目标的混个因子
    //glDisable(GL_BLEND);

    //创建点光源
    unsigned lightVAO, lightVBO;
    int pointeNumber = createCube(&lightVAO, &lightVBO);

    //渲染到纹理
    unsigned framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    //生成纹理
    unsigned texColorBuffer;
    glGenTextures(1, &texColorBuffer);
    glBindTexture(GL_TEXTURE_2D, texColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    //附加到当前绑定的帧缓冲对象
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);
    //创建渲染缓冲对象
    unsigned rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    //附加到当前绑定的帧缓冲对象
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    //检查帧缓冲是否完整
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //平面
    float plane[] = {
        // positions
        -0.5f, -0.25f,  0.0f,    0.0f,   0.0f,
         0.5f, -0.25f,  0.0f,    1.0f,   0.0f,
         0.5f,  0.5f,  0.0f,    1.0f,   1.0f,
         0.5f,  0.5f,  0.0f,    1.0f,   1.0f,
        -0.5f,  0.5f,  0.0f,    0.0f,   1.0f,
        -0.5f, -0.25f,  0.0f,    0.0f,   0.0f
    };
    unsigned photoVAO, photoVBO;
    glGenVertexArrays(1, &photoVAO);
    glGenBuffers(1, &photoVBO);
    glBindVertexArray(photoVAO);
    glBindBuffer(GL_ARRAY_BUFFER, photoVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(plane), plane, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float),
                          reinterpret_cast<void*>(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    std::vector<std::string> faces
    {
        "../learnOpenGL/texture/skybox/right.jpg",
        "../learnOpenGL/texture/skybox/left.jpg",
        "../learnOpenGL/texture/skybox/top.jpg",
        "../learnOpenGL/texture/skybox/bottom.jpg",
        "../learnOpenGL/texture/skybox/front.jpg",
        "../learnOpenGL/texture/skybox/back.jpg"
    };
    unsigned cubemapTexture = loadCubmap(faces);
    unsigned skyboxVAO, skyboxVBO;
    int skyPNumber = createCube(&skyboxVAO, &skyboxVBO);

    MShader myShader("../learnOpenGL/shader/vertex.vert",
                     "../learnOpenGL/shader/fragment.frag");
    unsigned uBlockIndexMy = glad_glGetUniformBlockIndex(myShader.ID, "Matrices");
    glUniformBlockBinding(myShader.ID, uBlockIndexMy, 0);

    MShader lightShader("../learnOpenGL/shader/vertex.vert",
                     "../learnOpenGL/shader/lamp.frag");
    unsigned uBlockIndexLight = glad_glGetUniformBlockIndex(lightShader.ID, "Matrices");
    glUniformBlockBinding(lightShader.ID, uBlockIndexLight, 0);

    MShader skyboxShader("../learnOpenGL/shader/skybox.vert",
                     "../learnOpenGL/shader/skybox.frag");
    unsigned uBlockIndexSky = glad_glGetUniformBlockIndex(skyboxShader.ID, "Matrices");
    glUniformBlockBinding(skyboxShader.ID, uBlockIndexSky, 0);

    MShader photoShader("../learnOpenGL/shader/photo.vert",
                     "../learnOpenGL/shader/photo.frag");
    unsigned uBlockIndexPhoto = glad_glGetUniformBlockIndex(photoShader.ID, "Matrices");
    glUniformBlockBinding(photoShader.ID, uBlockIndexPhoto, 0);

    MShader reflection("../learnOpenGL/shader/vertex.vert",
                     "../learnOpenGL/shader/reflection.frag");
    unsigned uBlockIndexRef = glad_glGetUniformBlockIndex(reflection.ID, "Matrices");
    glUniformBlockBinding(reflection.ID, uBlockIndexRef, 0);

    //创建Uniform缓冲对象
    unsigned uboMatrices;
    glGenBuffers(1, &uboMatrices);
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferData(GL_UNIFORM_BUFFER, 2*sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    //绑定到绑定点0
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboMatrices);

    //视图矩阵
    glm::mat4 view(1.0f);
    //投影矩阵
    glm::mat4 projection(1.0f);
    //变换矩阵
    glm::mat4 model(1.0f);
    //法线矩阵
    glm::mat3 normalMat(1.0f);
    //单位矩阵
    glm::mat4 identityMat(1.0f);

    glm::vec3 ambientStrength;//环境光量
    glm::vec3 diffuse;//漫反射量（保持本身颜色）
    myShader.use();
    //平行光
    glm::vec3 lightStrength(0.6f, 0.6f, 0.6f);//高光（光源）量
    ambientStrength = lightStrength * 0.3f;//环境光量
    diffuse = lightStrength - ambientStrength;//漫反射量（保持本身颜色）
    glm::vec3 parallelDir(-0.2f, -1.0f, 0.3f);//平行光源方向
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

    //加载模型
    Model3d mModel("../learnOpenGL/model/nanosuit/nanosuit.blend");

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
        lightPos.x = static_cast<float>(sin(glfwGetTime()));

        projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), SCR_WIDTH/SCR_HEIGHT, 0.1f, 100.f);

        view = glm::mat4(1.0f);
        view = glm::rotate(view, rotateView.x, {1.0f, 0.0f, 0.0f});
        view = glm::rotate(view, rotateView.y, {0.0f, 1.0f, 0.0f});
        view = glm::translate(view, -cameraPos);

        //填充Uniform缓冲
        glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
        //sub->偏移量和大小
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        //剔除背面（仅针对闭合形状）
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);//定义顺时针的面为正向面

        //使用帧缓冲
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        //使用稍亮的背景色
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //绘制模型至帧缓冲
        myShader.use();
        myShader.setUniform3F("LuminousBody.position", lightPos);
        myShader.setUniform3F("CameraPos", cameraPos);
        myShader.setUniformMatrix4fv(glm::value_ptr(projection), "projection");
        myShader.setUniformMatrix4fv(glm::value_ptr(identityMat), "view");
        model = glm::mat4(1.0f);
        model = glm::translate(model, {0.0f, -2.0f, -3.0f});
        model = glm::scale(model, glm::vec3(0.2f));
        myShader.setUniformMatrix4fv(glm::value_ptr(model), "model");
        myShader.setUniform1F("OneMaterial.shininess", 32.0f);
        mModel.draw(myShader, GL_TRIANGLES);
        //返回默认帧缓冲
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //绘制模型
        myShader.use();
        myShader.setUniform3F("LuminousBody.position", lightPos);
        myShader.setUniform3F("CameraPos", cameraPos);
        myShader.setUniformMatrix4fv(glm::value_ptr(projection), "projection");
        myShader.setUniformMatrix4fv(glm::value_ptr(view), "view");
        model = glm::mat4(1.0f);
        model = glm::translate(model, {0.0f, -2.0f, -3.0f});
        model = glm::scale(model, glm::vec3(0.2f));
        myShader.setUniformMatrix4fv(glm::value_ptr(model), "model");
        myShader.setUniform1F("OneMaterial.shininess", 32.0f);
        mModel.draw(myShader, GL_TRIANGLES);

        //绘制反射模型
        reflection.use();
        //std::cout << cameraPos.x << cameraPos.y << cameraPos.z << std::endl;
        reflection.setUniform3F("CameraPos", cameraPos);
        reflection.setUniformMatrix4fv(glm::value_ptr(projection), "projection");
        reflection.setUniformMatrix4fv(glm::value_ptr(view), "view");
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        model = glm::mat4(1.0f);
        model = glm::translate(model, {2.0f, -2.0f, -3.0f});
        model = glm::scale(model, glm::vec3(0.2f));
        reflection.setUniformMatrix4fv(glm::value_ptr(model), "model");
        mModel.draw(myShader, GL_TRIANGLES, true);

        //绘制灯
        glFrontFace(GL_CW);//定义顺时针的面为正向面
        lightShader.use();
        lightShader.setUniform3F("lightColor", 0.3f+pointlightStrength);
        lightShader.setUniformMatrix4fv(glm::value_ptr(projection), "projection");
        lightShader.setUniformMatrix4fv(glm::value_ptr(view), "view");
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.05f));
        lightShader.setUniformMatrix4fv(glm::value_ptr(model), "model");
        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, pointeNumber);//VBO内存储顶点顺序绘制
        glBindVertexArray(0);
        glFrontFace(GL_CCW);//定义顺时针的面为正向面

        //取消背面剔除
        glDisable(GL_CULL_FACE);
        //绘制照片平面
        glBindVertexArray(photoVAO);
        photoShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, {-1.0f, 0.0f, -2.0f});
        model = glm::rotate(model, glm::radians(30.0f), {0.0f, 1.0f, 0.0f});
        photoShader.setUniformMatrix4fv(glm::value_ptr(model), "model");
        glBindTexture(GL_TEXTURE_2D, texColorBuffer);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        glEnable(GL_CULL_FACE);

        //绘制天空盒
        glDepthMask(GL_FALSE);
        skyboxShader.use();
        glm::mat3 skybox(view);
        glBindVertexArray(skyboxVAO);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, skyPNumber);
        glBindVertexArray(0);
        glDepthMask(GL_TRUE);

        //检查并调用事件，交换缓冲----
        //交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲），它在这一迭代中被用来绘制
        glfwSwapBuffers(window);
        //检查是否触发事件
        glfwPollEvents();
    }

    //正确释放/删除之前分配的所有资源
    mModel.deleteBuffer();
    glDeleteBuffers(1, &lightVBO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &photoVBO);
    glDeleteVertexArrays(1, &photoVAO);
    glDeleteBuffers(1, &skyboxVBO);
    glDeleteVertexArrays(1, &skyboxVAO);
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

unsigned loadCubmap(std::vector<std::string> faces)
{
    unsigned textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for(unsigned i = 0; i < faces.size() && i < 6; ++i)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if(data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 0, GL_RGB, width, height, 0,
                         GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else {
            std::cerr << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

int createCube(unsigned *VAO, unsigned *VBO)
{
    //以标准化设备坐标指定多个顶点
    float vertices[] = {
        // positions
        // Back face
        // positions
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
    //管理光源的VAO
    glGenVertexArrays(1, VAO);
    glGenBuffers(1, VBO);
    glBindVertexArray(*VAO);//记录以下操作
    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //设置灯的顶点属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), nullptr);//位置属性
    glEnableVertexAttribArray(0);
    //显式解绑VAO
    glBindVertexArray(0);

    return sizeof(vertices)/sizeof(float)/3;
}
