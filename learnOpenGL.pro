TEMPLATE = app
CONFIG += c++11
CONFIG -= console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    glad.c \
    mshader.cpp \
    mmatrix.cpp \
    mesh.cpp \
    model3d.cpp

win32{
LIBS += -L$$PWD/../bin/libs/ -lglfw3 \
    -L$$PWD/../bin/libs/ -lassimp

INCLUDEPATH += $$PWD/../bin/include
DEPENDPATH += $$PWD/../bin/include
}

unix{
unix:!macx: LIBS += -lglfw \
    -ldl #显式加载动态库的动态函数库
}

DISTFILES += \
    shader/vertex.vert \
    shader/fragment.frag \
    shader/lamp.frag \
    shader/transparent.frag

HEADERS += \
    mshader.h \
    mvec.h \
    mmatrix.h \
    mesh.h \
    model3d.h \
    stb_image.h \
    stb_image_implementation.h

RESOURCES +=

