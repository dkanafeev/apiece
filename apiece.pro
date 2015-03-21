#-------------------------------------------------
#
# Project created by QtCreator 2015-03-09T00:33:04
#
#-------------------------------------------------

QT       += core
QT       -= gui

TARGET = apiece
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SUBDIRS += src
SUBDIRS += include
SOURCES += \
    src/main.cpp \
    src/cardriver.cpp \
    src/controller.cpp \
    src/linedetector.cpp \
    src/inputdata.cpp

HEADERS += \
    include/cardriver.h \
    include/controller.h \
    include/linedetector.h \
    include/inputdata.h

QMAKE_CXXFLAGS += -std=c++11

INCLUDEPATH += $$PWD/3rdparty/opencv/install/include
LIBS += -L"$$PWD/3rdparty/opencv/install/x64/mingw/bin"

LIBS += -lopencv_calib3d300 -lopencv_core300 -lopencv_features2d300 -lopencv_flann300 \
        -lopencv_highgui300 -lopencv_imgcodecs300 -lopencv_imgproc300 -lopencv_ml300 \
        -lopencv_objdetect300 -lopencv_photo300 -lopencv_shape300 -lopencv_stitching300 \
        -lopencv_superres300 -lopencv_video300 -lopencv_videoio300 -lopencv_videostab300

