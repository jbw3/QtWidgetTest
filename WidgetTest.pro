#-------------------------------------------------
#
# Project created by QtCreator 2013-02-01T19:19:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = bin/WidgetTest
TEMPLATE = app

CONFIG += c++11

SOURCES += \
        main.cpp \
        dialog.cpp \
    validatingitemdelegate.cpp \
    valuelistwidget.cpp \
    comboboxitemdelegate.cpp

HEADERS  += dialog.h \
    validatingitemdelegate.h \
    valuelistwidget.h \
    comboboxitemdelegate.h

FORMS    += dialog.ui

OTHER_FILES += \
    dark.qss \
    roundButtons.qss \
    listWidget.qss \
    blue.qss

RESOURCES += \
    resources.qrc

# Keep auto-generated source and object files in separate directory
OBJECTS_DIR = build
MOC_DIR = build
RCC_DIR = build
UI_DIR = build
