#-------------------------------------------------
#
# Project created by QtCreator 2014-06-22T08:13:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = raytrace
TEMPLATE = app


SOURCES += main.cpp\
        screen.cpp \
    common.cpp \
    vec3f.cpp \
    raytracer.cpp \
    object.cpp \
    simpleobject.cpp \
    kdtree.cpp

HEADERS  += screen.h \
    common.h \
    vec3f.h \
    raytracer.h \
    object.h \
    simpleobject.h \
    kdtree.h \
    thread.h

FORMS    += screen.ui
