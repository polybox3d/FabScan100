INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

QT += core gui opengl multimedia multimediawidgets


CONFIG += precompile_header
PRECOMPILED_HEADER = $$PWD/staticHeaders.h

INCLUDEPATH += /usr/local/include/pcl-1.8/
INCLUDEPATH += /usr/include/eigen3/

#include(qextserialport-1.2rc/src/qextserialport.pri)

SOURCES += $$PWD/PolyFabScanWindow.cpp \
    $$PWD/mainwidget.cpp \
    $$PWD/fscontroller.cpp \
    $$PWD/fsconfiguration.cpp \
    $$PWD/fslaser.cpp \
    $$PWD/fsturntable.cpp \
    $$PWD/fsvision.cpp \
    $$PWD/fswebcam.cpp \
    $$PWD/geometryengine.cpp \
    $$PWD/glextensions.cpp \
    $$PWD/fsmodel.cpp \
    $$PWD/fsdialog.cpp

HEADERS  += $$PWD/PolyFabScanWindow.h \
    $$PWD/mainwidget.h \
    $$PWD/fscontroller.h \
    $$PWD/fsconfiguration.h \
    $$PWD/fsdefines.h \
    $$PWD/fsgeometries.h \
    $$PWD/fslaser.h \
    $$PWD/fsturntable.h \
    $$PWD/fsvision.h \
    $$PWD/fswebcam.h \
    $$PWD/geometryengine.h \
    $$PWD/glextensions.h \
    $$PWD/fsmodel.h \
    $$PWD/staticHeaders.h \
    $$PWD/fsdialog.h

FORMS    += $$PWD/PolyFabScanWindow.ui \
    $$PWD/fsdialog.ui


OTHER_FILES += \
    $$PWD/fshader.glsl \
    $$PWD/vshader.glsl

RESOURCES += \
    $$PWD/shaders.qrc \
    $$PWD/textures.qrc \
    $$PWD/ressources.qrc


    message("==> PolyFabScan for Linux.")

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
