QT          += core gui widgets opengl

TEMPLATE    = app
CONFIG      += c++17

TARGET      = 3D-programmering

SOURCES += main.cpp \
    TQuadtre.cpp \
    beziercurve.cpp \
    camera.cpp \
    interactiveobject.cpp \
    logger.cpp \
    mainwindow.cpp \
    modelobject.cpp \
    octahedronball.cpp \
    pointlight.cpp \
    quad.cpp \
    renderwindow.cpp \
    rollingball.cpp \
    shader.cpp \
    texture.cpp \
    texturetriangle.cpp \
    trianglesurface.cpp \
    trophy.cpp \
    underlag.cpp \
    vertex.cpp \
    visualobject.cpp \
    xyz.cpp

HEADERS += \
    Bitmap/bitmap_image.hpp \
    Input.h \
    TQuadtre.h \
    beziercurve.h \
    camera.h \
    interactiveobject.h \
    logger.h \
    mainwindow.h \
    modelobject.h \
    octahedronball.h \
    pointlight.h \
    quad.h \
    renderwindow.h \
    rollingball.h \
    shader.h \
    texture.h \
    texturetriangle.h \
    trianglesurface.h \
    trophy.h \
    underlag.h \
    vertex.h \
    visualobject.h \
    xyz.h

FORMS += \
    mainwindow.ui

DISTFILES += \
    Bitmap/Makefile \
    PhongPointLight.frag \
    PhongPointLight.vert \
    plainshader.frag \
    plainshader.vert \
    textureshader.frag \
    textureshader.vert
