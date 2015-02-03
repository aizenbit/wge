#-------------------------------------------------
#
# Project created by QtCreator 2015-02-03T17:18:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = wge
TEMPLATE = app


SOURCES += main.cpp\
        ui.cpp \
    gamemechanics.cpp

HEADERS  += ui.h \
    gamemechanics.h

QMAKE_CXXFLAGS += -std=gnu++0x
