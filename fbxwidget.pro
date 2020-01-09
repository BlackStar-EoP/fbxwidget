QT       += core gui widgets

INCLUDEPATH += $$PWD/src

CONFIG += console

TARGET = fbxwidget
TEMPLATE = app

SOURCES += $$PWD/main.cpp \
           $$PWD/mainwindow.cpp \
           $$PWD/fbxtreewidget.cpp \
           $$PWD/fbxmemorystream.cpp \
           $$PWD/fbxloader.cpp
           
HEADERS += $$PWD/mainwindow.h \
           $$PWD/fbxtreewidget.h \
           $$PWD/fbxloader.h \ 
           $$PWD/fbxmemorystream.h

INCLUDEPATH += "C:/Program Files/Autodesk/FBX/FBX SDK/2019.0/include"
LIBS += "C:/Program Files/Autodesk/fbx/FBX SDK/2019.0/lib/vs2015/x64/debug/libfbxsdk.lib"