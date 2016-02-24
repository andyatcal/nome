#-------------------------------------------------
#
# Project created by QtCreator 2016-02-23T18:34:19
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = slide2016
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

linux-g++{
    LIBS += -lglut -lGLU -lGL
}

mac{
    INCLUDEPATH = /usr/local/include
    LIBS += -framework GLUT -framework OpenGL
}
