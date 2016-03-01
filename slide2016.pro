#-------------------------------------------------
#
# Project created by QtCreator 2016-02-23T18:34:19
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = slide2016
TEMPLATE = app
QMAKE_CXXFLAGS = -std=c++11 -w
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.11
SOURCES += main.cpp\
        mainwindow.cpp \
    edge.cpp \
    vertex.cpp \
    face.cpp \
    mesh.cpp \
    marge.cpp \
<<<<<<< HEAD
    transformation.cpp
=======
    makeMesh.cpp
>>>>>>> origin/master

HEADERS  += mainwindow.h \
    edge.h \
    vertex.h \
    face.h \
    mesh.h \
    merge.h \
<<<<<<< HEAD
    transformation.h
=======
    makeMesh.h
>>>>>>> origin/master

FORMS    += mainwindow.ui

linux{
    LIBS += -lglut -lGLU
}

mac{
    INCLUDEPATH = /usr/local/include
    LIBS += -framework GLUT -framework OpenGL
}

win32{
    LIBS += -lopengl32 -lglu32 -lglut32
}
