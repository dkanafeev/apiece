#-------------------------------------------------
#
# Project created by QtCreator 2015-03-09T00:33:04
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = cv0903
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    cardriver.cpp \
    controller.cpp \
    linedetector.cpp \
    inputdata.cpp

HEADERS += \
    cardriver.h \
    controller.h \
    linedetector.h \
    inputdata.h

QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS +=-static

INCLUDEPATH += E:\Dev\Tools\opencv_build_mingw\install\include
LIBS += -L"E:/Dev/Tools/opencv_build_mingw/install/x64/mingw/bin"
LIBS += -lopencv_calib3d300 -lopencv_core300 -lopencv_features2d300 -lopencv_flann300 \
        -lopencv_highgui300 -lopencv_imgcodecs300 -lopencv_imgproc300 -lopencv_ml300 \
        -lopencv_objdetect300 -lopencv_photo300 -lopencv_shape300 -lopencv_stitching300 \
        -lopencv_superres300 -lopencv_video300 -lopencv_videoio300 -lopencv_videostab300

