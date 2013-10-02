#-------------------------------------------------
#
# Project created by QtCreator 2013-09-26T16:29:59
#
#-------------------------------------------------

QT       += core gui

TARGET = HostManager
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    reproductor.cpp

HEADERS  += mainwindow.h \
    reproductor.h

FORMS    += mainwindow.ui


unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += gstreamer-0.10
}
