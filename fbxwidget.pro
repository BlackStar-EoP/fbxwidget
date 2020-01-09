QT       += core gui widgets

INCLUDEPATH += $$PWD/src

CONFIG += console

TARGET = fbxwidget
TEMPLATE = app

SOURCES += $$PWD/main.cpp \
           $$PWD/mainwindow.cpp \
           $$PWD/fbxtreewidget.cpp \
           $$PWD/fbxmemorystream.cpp
           
HEADERS += $$PWD/mainwindow.h \
           $$PWD/fbxtreewidget.h \
           $$PWD/fbxmemorystream.h

INCLUDEPATH += "C:/Program Files/Autodesk/FBX/FBX SDK/2019.0/include"