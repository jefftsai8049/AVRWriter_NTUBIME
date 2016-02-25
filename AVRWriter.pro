#-------------------------------------------------
#
# Project created by QtCreator 2016-02-25T10:16:12
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AVRWriter
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    avrthread.cpp

HEADERS  += mainwindow.h \
    avrthread.h

FORMS    += mainwindow.ui

RESOURCES += \
    icon.qrc
