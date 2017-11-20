#-------------------------------------------------
#
# Project created by QtCreator 2017-09-30T09:49:20
#
#-------------------------------------------------
TEMPLATE = app

CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp

HEADERS += \
    connector.h \
    container.h

LIBS += libodbc32
