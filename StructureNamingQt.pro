#-------------------------------------------------
#
# Project created by QtCreator 2016-08-23T09:10:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = StructureNamingQt
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    aboutdialog.cpp \
    structurenaming.cpp \
    createplanningassistdialog.cpp \
    targetvolumedialog.cpp \
    addstructuregroup.cpp

HEADERS  += mainwindow.h \
    aboutdialog.h \
    structurenaming.h \
    createplanningassistdialog.h \
    targetvolumedialog.h \
    addstructuregroup.h

FORMS    += mainwindow.ui \
    aboutdialog.ui \
    structurenaming.ui \
    createplanningassistdialog.ui \
    targetvolumedialog.ui \
    addstructuregroup.ui
