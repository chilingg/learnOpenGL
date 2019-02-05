TEMPLATE = app
CONFIG += c++11
CONFIG -= console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    glad.c \
    mshader.cpp

win32{
LIBS += -L$$PWD/../../bin/libs/ -lglfw3dll

INCLUDEPATH += $$PWD/../../bin/include
DEPENDPATH += $$PWD/../../bin/include
}

unix{
unix:!macx: LIBS += -lglfw \
    -ldl #显式加载动态库的动态函数库
}

DISTFILES += \
    shader/vertex.vert \
    shader/fragment.frag

HEADERS += \
    mshader.h \
    stb_image.h \
    stb_image_implementation.h

RESOURCES +=

