#-------------------------------------------------
#
# Project created by QtCreator 2017-08-14T11:03:11
#
#-------------------------------------------------

QT       += core gui webenginewidgets script svg network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MyTool
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
    models/apptablemodel.cpp \
    models/backuptablemodel.cpp \
    models/contactmodel.cpp \
    models/filetablemodel.cpp \
    models/logcatmodel.cpp \
    models/messagemodel.cpp \
    models/messagethreadmodel.cpp \
    animation.cpp \
    mainwindow.cpp \
    settingswidget.cpp \
    mysettingwidget.cpp \
    mymodel.cpp \
    mystyledelegate.cpp \
    dialog.cpp \
    myinputdialog.cpp \
    mydatadb.cpp \
    jswidget.cpp \
    mtthread.cpp \
    testclass.cpp \
    drawwidget.cpp \
    mygraphicsview.cpp \
    edcwidget.cpp \
    edcwindow.cpp \
    splitphotodialog.cpp


HEADERS += \
    models/apptablemodel.h \
    models/backuptablemodel.h \
    models/contactmodel.h \
    models/filetablemodel.h \
    models/logcatmodel.h \
    models/messagemodel.h \
    models/messagethreadmodel.h \
    animation.h \
    mainwindow.h \
    settingswidget.h \
    mysettingwidget.h \
    mymodel.h \
    mystyledelegate.h \
    dialog.h \
    myinputdialog.h \
    mydatadb.h \
    jswidget.h \
    mtthread.h \
    testclass.h \
    drawwidget.h \
    mygraphicsview.h \
    edcwidget.h \
    edcwindow.h \
    splitphotodialog.h


FORMS += \
    mainwindow.ui \
    settingswidget.ui \
    mysettingwidget.ui \
    dialog.ui \
    myinputdialog.ui \
    jswidget.ui \
    edcwidget.ui \
    edcwindow.ui \
    splitphotodialog.ui


INCLUDEPATH += ../TestLib
DEPENDPATH += $$PWD/.

RESOURCES += \
    tool.qrc
TRANSLATIONS += myapp.ts
