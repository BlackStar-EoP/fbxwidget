QT       += core gui widgets

INCLUDEPATH += $$PWD/src

CONFIG += console

TARGET = fbxwidget
TEMPLATE = app

SOURCES += $$PWD/main.cpp \
           $$PWD/mainwindow.cpp \
           $$PWD/fbxtreewidget.cpp
           
HEADERS += $$PWD/mainwindow.h \
           $$PWD/fbxtreewidget.h