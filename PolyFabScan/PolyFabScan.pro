#-------------------------------------------------
#
# Project created by QtCreator 2015-07-20T17:18:20
#
#-------------------------------------------------

QT       += core gui opengl multimedia multimediawidgets

CONFIG += static noframework console

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH += /usr/local/include/pcl-1.8/
INCLUDEPATH += /usr/include/eigen3/

TARGET = PolyFabScan
TEMPLATE = app


CONFIG += precompile_header
PRECOMPILED_HEADER = staticHeaders.h

include(qextserialport-1.2rc/src/qextserialport.pri)

SOURCES += main.cpp\
        PolyFabScanWindow.cpp \
    mainwidget.cpp \
    fscontroller.cpp \
    fsconfiguration.cpp \
    fslaser.cpp \
    fsturntable.cpp \
    fsvision.cpp \
    fswebcam.cpp \
    geometryengine.cpp \
    glextensions.cpp \
    fsmodel.cpp \
    fsdialog.cpp \
    ComModule.cpp \
    MovementModule.cpp \
    SerialPort.cpp \
    Config.cpp

HEADERS  += PolyFabScanWindow.h \
    mainwidget.h \
    fscontroller.h \
    fsconfiguration.h \
    fsdefines.h \
    fsgeometries.h \
    fslaser.h \
    fsturntable.h \
    fsvision.h \
    fswebcam.h \
    geometryengine.h \
    glextensions.h \
    fsmodel.h \
    staticHeaders.h \
    fsdialog.h \
    ComModule.h \
    MovementModule.h \
    mcode.h \
    SerialPort.h \
    Config.h

FORMS    += PolyFabScanWindow.ui \
    fsdialog.ui


OTHER_FILES += \
    fshader.glsl \
    vshader.glsl

RESOURCES += \
    shaders.qrc \
    textures.qrc


    message("Buildng for Linux.")

    #CONFIG += link_pkgconfig
    #PKGCONFIG += opencv
    #INCLUDEPATH += /usr/local/include/opencv2/
    INCLUDEPATH += /usr/include
    INCLUDEPATH += /usr/include/boost/

    LIBS += -L/usr/lib/ \
     -lboost_system \
     -lboost_filesystem \

    DEFINES += LINUX

    INCLUDEPATH += /usr/include/eigen3/
    INCLUDEPATH += /usr/include/boost/

    LIBS += -L/usr/lib/ \
     -lboost_system \
     -lboost_filesystem \

    LIBS += -L/usr/local/lib/ \
    -lopencv_core \
    -lopencv_highgui \
    -lopencv_imgproc \
    -lopencv_features2d \
    -lopencv_calib3d

    INCLUDEPATH += /usr/local/include/

    INCLUDEPATH += /usr/local/include/pcl-1.8/
    LIBS += -L/usr/local/lib/ \
    -lpcl_common \
    -lpcl_io \
    -lpcl_filters \
    -lpcl_kdtree \
    -lpcl_registration \
    -lpcl_features \
    -lpcl_segmentation \
    -lpcl_surface \
    -lpcl_search
