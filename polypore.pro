#-------------------------------------------------
#
# Project created by QtCreator 2010-07-08T15:00:37
#
#-------------------------------------------------

QT       += core gui svg
CONFIG += console
CONFIG += staticlib
TARGET = polypore
#INCLUDEPATH = /home/nelson/qtsdk-2010.04/qt/lib/
TEMPLATE = app
QMAKE_CXXFLAGS = -O3 -pipe -fomit-frame-pointer
QMAKE_CFLAGS = -O3 -pipe -fomit-frame-pointer
#QMAKE_CXXFLAGS_DEBUG += -pg
#QMAKE_LFLAGS_DEBUG += -pg

SOURCES += main.cpp\
        include/avector3d.cpp \
        include/cycle.cpp \
        include/constants.cpp \
        include/window.cpp \
        include/console.cpp \
        include/field.cpp \
        include/renderer.cpp


HEADERS  += include/avector3d.h \
    include/mtwist.h \
    include/cycle.h \
    include/constants.h \
    include/particle.h \
    include/solver.h \
    include/forces.h \
    include/window.h \
    include/console.h \
    include/field.h \
    include/renderer.h

