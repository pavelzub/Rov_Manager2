#-------------------------------------------------
#
# Project created by QtCreator 2018-04-07T01:28:33
#
#-------------------------------------------------

QT       += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Rov_Manager2
TEMPLATE = app

INCLUDEPATH += $$PWD/ffmpeg/include\
    $$PWD/OpenCV/include

LIBS += -L$$PWD/ffmpeg/lib \
    -lavcodec \
    -lavdevice \
    -lavutil \
    -lpostproc \
    -lavfilter \
    -lswresample \
    -lavformat \
    -lswscale \
    -L$$PWD/OpenCV/lib \
    -lopencv_highgui2413 \
    -lopencv_calib3d2413 \
    -lopencv_contrib2413 \
    -lopencv_core2413 \
    -lopencv_features2d2413 \
    -lopencv_flann2413 \
    -lopencv_gpu2413 \
    -lopencv_imgproc2413 \
    -lopencv_legacy2413 \
    -lopencv_ml2413 \
    -lopencv_nonfree2413 \
    -lopencv_objdetect2413 \
    -lopencv_ocl2413 \
    -lopencv_photo2413 \
    -lopencv_stitching2413 \
    -lopencv_superres2413 \
    -lopencv_video2413 \
    -lopencv_videostab2413
# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    mainwidget.cpp \
    sceenshot/screenlable.cpp \
    sceenshot/screenshotwindow.cpp \
    video/videostreamparser.cpp \
    video/videowidget.cpp \
    finder/finder.cpp \
    finder/imagedetector.cpp

HEADERS += \
    mainwidget.hpp \
    sceenshot/screenlable.hpp \
    sceenshot/screenshotwindow.hpp \
    video/videostreamparser.hpp \
    video/videowidget.hpp \
    mainwindow.hpp \
    finder/finder.hpp \
    finder/imagedetector.hpp
