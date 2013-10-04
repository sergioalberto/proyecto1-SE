#-------------------------------------------------
#
# Project created by QtCreator 2013-09-26T16:32:37
#
#-------------------------------------------------

QT       += core gui

TARGET = ClientPlayer
TEMPLATE = app

CONFIG += link_pkgconfig
PKGCONFIG += gstreamer-0.10

SOURCES += main.cpp\
        mainwindow.cpp \
    streamer.cpp

HEADERS  += mainwindow.h \
    streamer.h

FORMS    += mainwindow.ui


