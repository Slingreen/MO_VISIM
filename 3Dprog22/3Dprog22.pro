QT          += core gui widgets opengl

TEMPLATE    = app
CONFIG      += c++17

TARGET      = 3D-programmering

SOURCES += main.cpp \
    TQuadtre.cpp \
    beziercurve.cpp \
    bspline.cpp \
    camera.cpp \
    interactiveobject.cpp \
    lasmap.cpp \
    logger.cpp \
    mainwindow.cpp \
    modelobject.cpp \
    octahedronball.cpp \
    pointlight.cpp \
    points.cpp \
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
    bspline.h \
    camera.h \
    interactiveobject.h \
    lasmap.h \
    logger.h \
    mainwindow.h \
    modelobject.h \
    octahedronball.h \
    pointlight.h \
    points.h \
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
    PhongNoTexture.frag \
    PhongNoTexture.vert \
    PhongPointLight.frag \
    PhongPointLight.vert \
    plainshader.frag \
    plainshader.vert \
    textureshader.frag \
    textureshader.vert
