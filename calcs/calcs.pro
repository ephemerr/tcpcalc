#-------------------------------------------------
#
# Project created by QtCreator 2016-11-04T11:48:21
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = calcs
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    calc.cpp \
    calcserver.cpp

HEADERS += \
    calc.h \
    calcserver.h
