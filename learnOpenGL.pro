TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    glad.c

win32{
LIBS += -L$$PWD/../../bin/libs/ -lglfw3dll

INCLUDEPATH += $$PWD/../../bin/include
DEPENDPATH += $$PWD/../../bin/include
}

unix{

}
