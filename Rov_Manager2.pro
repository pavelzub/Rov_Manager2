#-------------------------------------------------
#
# Project created by QtCreator 2018-04-07T01:28:33
#
#-------------------------------------------------

QT       += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Rov_Manager2
TEMPLATE = app

INCLUDEPATH += $$PWD/ffmpeg/include

LIBS += -L$$PWD/ffmpeg/lib \
    -lavcodec \
    -lavdevice \
    -lavutil \
    -lpostproc \
    -lavfilter \
    -lswresample \
    -lavformat \
    -lswscale
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
    screenlable.cpp \
    screenshotwindow.cpp \
    videostreamparser.cpp \
    videowidget.cpp

HEADERS += \
    mainwidget.hpp \
    screenlable.hpp \
    screenshotwindow.hpp \
    videostreamparser.hpp \
    videowidget.hpp \
    mainwindow.hpp
