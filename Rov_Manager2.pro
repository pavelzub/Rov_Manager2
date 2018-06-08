#-------------------------------------------------
#
# Project created by QtCreator 2018-04-07T01:28:33
#
#-------------------------------------------------

QT       += core gui multimedia multimediawidgets printsupport websockets network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Rov_Manager2
TEMPLATE = app

INCLUDEPATH += $$PWD/ffmpeg/include \
    $$PWD/OpenCV/include \
    $$PWD/Python27/include

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
    -lopencv_videostab2413 \
    -L$$PWD/Python27 \
    -lpython27
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
    video/videowidget.cpp \
    finder/finder.cpp \
    finder/imagedetector.cpp \
    camerawidget.cpp \
    calculationwidget.cpp \
    obswidget.cpp \
    qcustomplot/qcustomplot.cpp \
    ethernet/datastore.cpp \
    ethernet/tcpconnetor.cpp \
    rov_data_types/base_packet_t.cpp \
    rov_data_types/binary_stream.cpp \
    rov_data_types/crc.cpp \
    rov_data_types/rov_data_types.cpp \
    rov_data_types/serializable.cpp \
    configwidget.cpp \
    ctkrangeslider.cpp \
    clickablelabel.cpp \
    settings.cpp \
    map.cpp \
    video/websocket.cpp \
    obsconfigwidget.cpp \
    moshnyiwidget.cpp

HEADERS += \
    mainwidget.hpp \
    sceenshot/screenlable.hpp \
    sceenshot/screenshotwindow.hpp \
    video/videowidget.hpp \
    mainwindow.hpp \
    finder/finder.hpp \
    finder/imagedetector.hpp \
    camerawidget.hpp \
    calculationwidget.hpp \
    obswidget.hpp \
    qcustomplot/qcustomplot.hpp \
    ethernet/datastore.hpp \
    ethernet/tcpconnetor.hpp \
    rov_data_types/base_packet_t.hpp \
    rov_data_types/binary_stream.hpp \
    rov_data_types/crc.hpp \
    rov_data_types/meta.hpp \
    rov_data_types/rov_data_types.hpp \
    rov_data_types/serializable.hpp \
    configwidget.hpp \
    ctkrangeslider.hpp \
    clickablelabel.hpp \
    settings.hpp \
    map.hpp \
    video/websocket.hpp \
    obsconfigwidget.hpp \
    moshnyiwidget.hpp
