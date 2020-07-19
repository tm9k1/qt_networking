QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qtNetworkingSample
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        myserver.cpp

HEADERS  += mainwindow.h \ \
    myserver.h

FORMS    += mainwindow.ui

#Uncomment to check qDebug messages in a separate console window.
#CONFIG += console
