#include <RGame.h>
#include <RWindow.h>
#include <RResource.h>

using namespace Redopera;

size_t typeSize(GLenum type)
{
    size_t size;

    switch (type)
    {
#define CASE(Enum, Count, Type) case Enum: size = Count * sizeof(Type); break

        CASE(GL_FLOAT, 1, GLfloat);
        CASE(GL_FLOAT_VEC2, 2, GLfloat);
        CASE(GL_FLOAT_VEC3, 3, GLfloat);
        CASE(GL_FLOAT_VEC4, 4, GLfloat);
        CASE(GL_INT, 1, GLint);
        CASE(GL_INT_VEC2, 2, GLint);
        CASE(GL_INT_VEC3, 3, GLint);
        CASE(GL_INT_VEC4, 4, GLint);
        CASE(GL_UNSIGNED_INT, 1, GLuint);
        CASE(GL_UNSIGNED_INT_VEC2, 2, GLuint);
        CASE(GL_UNSIGNED_INT_VEC3, 3, GLuint);
        CASE(GL_UNSIGNED_INT_VEC4, 4, GLuint);
        CASE(GL_BOOL, 1, GLboolean);
        CASE(GL_BOOL_VEC2, 2, GLboolean);
        CASE(GL_BOOL_VEC3, 3, GLboolean);
        CASE(GL_BOOL_VEC4, 4, GLboolean);
        CASE(GL_FLOAT_MAT2, 4, GLfloat);
        CASE(GL_FLOAT_MAT2x3, 6, GLfloat);
        CASE(GL_FLOAT_MAT2x4, 8, GLfloat);
        CASE(GL_FLOAT_MAT3, 9, GLfloat);
        CASE(GL_FLOAT_MAT3x2, 6, GLfloat);
        CASE(GL_FLOAT_MAT3x4, 12, GLfloat);
        CASE(GL_FLOAT_MAT4, 16, GLfloat);
        CASE(GL_FLOAT_MAT4x2, 8, GLfloat);
        CASE(GL_FLOAT_MAT4x3, 12, GLfloat);

#undef CASE

        default:
            fprintf(stderr, "unknown type: 0x%x\n", type);
            exit(EXIT_FAILURE);
            break;
    }
    return size;
}

void start()
{
    glClearColor(1.f, 0.f, 0.f, 1.f);

    RShaders shaders = { RShader(":/shaders/uniform.vert", RShader::Type::Vertex),
                         RShader(":/shaders/uniform.frag", RShader::Type::Fragment) };
    // rpi保持shaders的use状态直到析构
    RRPI rpi = shaders.use();

    GLuint uboIndex;
    GLuint ubo;
    GLint uboSize;
    std::unique_ptr<RData[]> buf;

    // 查找Uniforms的uniform缓存索引，并判断整个块的大小
    uboIndex = glGetUniformBlockIndex(shaders.id(), "Uniforms");
    glGetActiveUniformBlockiv(shaders.id(), uboIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &uboSize);

    buf = std::make_unique<RData[]>(uboSize);

    // 准备存储在缓存对象中的值
    GLfloat scale = .5f;
    GLfloat translation[] = { .1f, .1f, .0f };
    GLfloat rotation[] = { 90.f, 0.f, 0.f, 1.f };
    GLboolean enabled = GL_TRUE;

    // 建立一个变量名称数组，对应块中已知的uniform变量
    enum {
        Translation = 0,
        Scale,
        Rotation,
        Enabled,
        NumUniforms
    };

    const char* names[NumUniforms] = {
        "translation",
        "scale",
        "rotation",
        "enabled"
    };

    // 查询对应的属性，判断向数据缓存写入数值的位置
    GLuint indices[NumUniforms];
    GLint size[NumUniforms];
    GLint offset[NumUniforms];
    GLint type[NumUniforms];

    glGetUniformIndices(shaders.id(), NumUniforms, names, indices);
    glGetActiveUniformsiv(shaders.id(), NumUniforms, indices, GL_UNIFORM_OFFSET, offset);
    glGetActiveUniformsiv(shaders.id(), NumUniforms, indices, GL_UNIFORM_SIZE, size);
    glGetActiveUniformsiv(shaders.id(), NumUniforms, indices, GL_UNIFORM_TYPE, type);

    // 将uniform变量值拷贝到buf中
    std::memcpy(buf.get() + offset[Scale], &scale, size[Scale] * typeSize(type[Scale]));
    std::memcpy(buf.get() + offset[Translation], &translation, size[Translation] * typeSize(type[Translation]));
    std::memcpy(buf.get() + offset[Rotation], &rotation, size[Rotation] * typeSize(type[Rotation]));
    std::memcpy(buf.get() + offset[Enabled], &enabled, size[Enabled] * typeSize(type[Enabled]));

    // 建立uniform缓存对象，初始化存储内容，并与着色器程序建立联系
    glGenBuffers(1, &ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferData(GL_UNIFORM_BUFFER, uboSize, buf.get(), GL_STATIC_DRAW);

    glBindBufferBase(GL_UNIFORM_BUFFER, uboIndex, ubo);

    /*
     *
     * ......
     *
     */
}

int main()
{
    RResource::setResourcePath(ROOT_DIR);

    RGame game;

    RWindow::Format fmt;
    fmt.versionMajor = 4;
    fmt.versionMinor = 5;

    RWindow window(800, 540, "OpenGL", fmt);
    window.node.setStartFunc(start);

    window.show();
    return window.node.exec();
}
