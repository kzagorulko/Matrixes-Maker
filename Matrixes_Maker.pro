#-------------------------------------------------
#
# Project created by QtCreator 2018-09-14T14:29:19
#
#-------------------------------------------------

QT       += core gui
CONFIG += console

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Matrixes_Maker
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dialog.cpp \
    matrix.cpp \
    summation.cpp

HEADERS  += mainwindow.h \
    dialog.h \
    matrix.h \
    summation.h

FORMS    += mainwindow.ui \
    dialog.ui \
    summation.ui

DISTFILES += \
    notes+todo.txt
