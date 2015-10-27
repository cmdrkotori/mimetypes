#-------------------------------------------------
#
# Project created by QtCreator 2015-10-27T16:24:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mimetypes
TEMPLATE = app
CONFIG += C++11

SOURCES += main.cpp\
        mainwindow.cpp \
    mimetypes.cpp

HEADERS  += mainwindow.h \
    mimetypes.h

FORMS    += mainwindow.ui

DISTFILES += \
    README.md \
    LICENSE
