#-------------------------------------------------
#
# Project created by QtCreator 2016-02-23T18:34:19
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = slide2016
TEMPLATE = app
CONFIG += c++11 -w

SOURCES += main.cpp\
    edge.cpp \
    vertex.cpp \
    face.cpp \
    mesh.cpp \
    polyline.cpp \
    marge.cpp \
    transformation.cpp \
    makeMesh.cpp \
    subdivision.cpp \
    offset.cpp \
    stl.cpp \
    zipper.cpp \
    makePolyline.cpp \
    myselection.cpp \
    slideglwidget.cpp \
    controlpanel.cpp \
    mainwindow.cpp

HEADERS  += \
    edge.h \
    vertex.h \
    face.h \
    mesh.h \
    polyline.h \
    transformation.h \
    merge.h \
    makeMesh.h \
    subdivison.h \
    offset.h \
    stl.h \
    zipper.h \
    makePolyline.h \
    myselection.h \
    slideglwidget.h \
    controlpanel.h \
    mainwindow.h

linux{
    LIBS += -lglut -lGLU
}

mac{
    INCLUDEPATH = /usr/local/include
    LIBS += -framework GLUT -framework OpenGL
}

win32{
    LIBS += -lopengl32 -lglu32 -lglut32
    INCLUDEPATH = $$PWD/../include
}

FORMS +=
