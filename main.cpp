#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>
#include <fstream>
#include "mshader.h"
#include "stb_image_implementation.h"
#include "mmatrix.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Window size
constexpr unsigned SCR_WIDTH = 800;
constexpr unsigned SCR_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow *, int width, int height);
void processInput(GLFWwindow *window);

int main()
{
    //初始化设置GLFW
    glfwInit();//初始化GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//set主版本号
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//set副版本号
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//use核心模式

    //窗口对象，存放了所有和窗口相关的数据
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Learn OpenGl", nullptr, nullptr);
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

    //初始化GLAD，glfwGetProcAddress是GLAD用来加载系统相关的OpenGL函数指针地址的函数
    if(!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    //设置窗口（视口）的维度
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);//lower left(-1 ~ 1)
    //设置清空屏幕所用的颜色（底色）
    glClearColor(0.1f, 0.2f, 0.2f, 1.0f);
    //忽略不必要的Z轴片段
    glEnable(GL_DEPTH_TEST);

    MShader myShader("../learnOpenGL/shader/vertex.vert",
                     "../learnOpenGL/shader/fragment.frag");

    //生成纹理ID并绑定到2D纹理
    unsigned int texture[2];
    glGenTextures(2, texture);
    const char *file[2] = {"../learnOpenGL/texture/container.jpg",
                           "../learnOpenGL/texture/awesomeface.jpg"};
    stbi_set_flip_vertically_on_load(true);//翻转图像
    for(unsigned i = 0; i < 2; ++i)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, texture[i]);
        //为当前绑定纹理对象设置环绕、过滤方式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //使用stb_image加载图片
        int width, height, nrChannels;//宽、高、颜色通道
        unsigned char *data = stbi_load(file[i], &width, &height, &nrChannels, 0);
        if(data)
        {
            //依据图片生成纹理，其参数为（纹理目标，指定多级渐远纹理级别，纹理存储模式，图宽，高，0，图片模式，图数据类型，图数据）
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);//自动生成所以需要的多级渐远纹理
        }
        else{
            std::cerr << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);//释放图像内存
    }

    //指定着色器程序对象
    myShader.use();
    //glUniform1i(glGetUniformLocation(myShader.shaderProgramID, "texture1"), 0); // 手动设置
    myShader.setUniform1I("ourTexture1", 0);
    myShader.setUniform1I("ourTexture2", 1);

    //以标准化设备坐标指定多个顶点
    float vertices[] = {
        -.5f, .5f, .5f, .0f, 1.0f,//l-t-n
        .5f, .5f, .5f, 1.0f, 1.0f,//r-t-n
        -.5f, -.5f, .5f, .0f, .0f,//l-b-n
        .5f, -.5f, .5f, 1.0f, .0f,//r-b-n

        -.5f, .5f, -.5f, 1.0f, 1.0f,//l-t-f
        .5f, .5f, -.5f, .0f, 1.0f,//r-t-f
        -.5f, -.5f, -.5f, 1.0f, .0f,//l-b-f
        .5f, -.5f, -.5f, .0f, .0f,//r-b-f
    };
    //指定绘制的索引顺序
    unsigned int indices[] = {
        0, 1, 2,
        1, 2, 3,//
        0, 1, 4,
        1, 4, 5,//
        0, 2, 4,
        2, 4, 6,//
        1, 3, 5,
        3, 5, 7,//
        2, 3, 6,
        3, 6, 7,//
        4, 5, 6,
        5, 6, 7//
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof (float), nullptr);//位置属性
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof (float), reinterpret_cast<void*>(3 * sizeof (float)));//纹理属性
    glEnableVertexAttribArray(1);
    //显式解绑VAO
    glBindVertexArray(0);

    MVec3 cubePositions[] = {
        MVec3{ 0.0f,  0.0f,  0.0f},
        MVec3{ 2.0f,  5.0f, -15.0f},
        MVec3{-1.5f, -2.2f, -2.5f},
        MVec3{-3.8f, -2.0f, -12.3f},
        MVec3{ 2.4f, -0.4f, -3.5f},
        MVec3{-1.7f,  3.0f, -7.5f},
        MVec3{ 1.3f, -2.0f, -2.5f},
        MVec3{ 1.5f,  2.0f, -2.5f},
        MVec3{ 1.5f,  0.2f, -1.5f},
        MVec3{-1.3f,  1.0f, -1.5f}
      };

    //渲染循环
    while(!glfwWindowShouldClose(window))
    {
        //输入----
        //ESC键退出
        processInput(window);

        //渲染指令----
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清屏 清除颜色缓冲和深度缓冲
        //指定该VAO中的解析顶点指针和存在的EBO
        glBindVertexArray(VAO);
        for(size_t i = 0; i < 1; i++)
        {
            //更新变换矩阵
            MMat4 model = makeIdentityMatrix();
            //model = scale(model, {0.5f, 0.5f, 0.5f});
            //model = rotation(model, {0.0f, i*20.0f, 0.0f});
            model = translate(model, {0.0f, 0.0f, 0.0f});
            model = translate(model, cubePositions[i]);
            //glm::mat4 model = glm::scale(glm::mat4(1.0f), {0.5f, 0.5f, 0.5f});
            //model = glm::rotate(model, glm::radians(static_cast<float>(glfwGetTime() * i)), {0.0f, 1.0f, 0.0f});
            //model = glm::translate(model, {0.0f, 0.0f, -3.0f});
            int transLoc = glGetUniformLocation(myShader.shaderProgramID, "model");
            if(transLoc != -1)
                glUniformMatrix4fv(transLoc, 1, GL_TRUE, model.matrixPtr());
                //glUniformMatrix4fv(transLoc, 1, GL_FALSE, glm::value_ptr(model));
            else
                std::cerr << "No find uniform location" << std::endl;

            MMat4 view = makeIdentityMatrix();
            view = camera(view, {static_cast<float>(glfwGetTime()), 0.0f, static_cast<float>(glfwGetTime())});
            view = lookAt(view, {0.0f, 0.0f, 0.0f});
            //view = rotation(view, {radians(90.0f), radians(135.0f), 0.0f});
            //view = rotation(view, {0.0f, radians(180.0f), 0.0f});
            int viewLoc = glGetUniformLocation(myShader.shaderProgramID, "view");
            if(viewLoc != -1)
                glUniformMatrix4fv(viewLoc, 1, GL_TRUE, view.matrixPtr());
            else
                std::cerr << "No find uniform location" << std::endl;

            MMat4 projection = projective(1.0f);
            /*glm::mat4 projection = glm::perspective(glm::radians(45.0f),
                                                    static_cast<float>(SCR_WIDTH/SCR_HEIGHT),
                                                    0.1f, 100.0f);*/
            int proLoc = glGetUniformLocation(myShader.shaderProgramID, "projection");
            if(proLoc != -1)
                glUniformMatrix4fv(proLoc, 1, GL_TRUE, projection.matrixPtr());
                //glUniformMatrix4fv(proLoc, 1, GL_FALSE, glm::value_ptr(projection));
            else
                std::cerr << "No find uniform location" << std::endl;

            glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(unsigned), GL_UNSIGNED_INT, nullptr);//以EBO存储的索引顺序绘制三角形
        }
        //glDrawArrays(GL_TRIANGLES, 0, 6);//VBO内存储顶点顺序绘制

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
