#-------------------------------------------------
#
# Project created by QtCreator 2017-08-04T11:14:25
#
#-------------------------------------------------

QT       += core gui widgets svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ProtoTest
TEMPLATE = app
INCLUDEPATH += usr/local/include


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
    mainwindow.cpp \
    fldata.pb.cc \
    fljobwindow.cpp \
    flfloorview.cpp \
    fljob.cpp \
    flpropertytable.cpp \
    fltreewidget.cpp \
    flwio.cpp \
    flzonecontrollor.cpp \
    flfloor.cpp \
    flobject.cpp \
    fldeviceitem.cpp \
    flgraphicssvgitem.cpp \
    flheatmap.cpp \
    flobjectlineitem.cpp \
    gradientpalette.cpp \
    edge.cpp \
    heatmapper.cpp \
    fljobprotol.cpp \
    flcreatefloordlg.cpp \
    flcreatejobdlg.cpp \
    flcreatewiodlg.cpp \
    flcreatezcdlg.cpp \
    fllineitem.cpp \
    flglobal.cpp \
    fl_job.pb.cc \
    fl_defs.pb.cc \
    macexplore.cpp \
    filetablemodel.cpp \
    mytableview.cpp

HEADERS += \
        mainwindow.h \
    fldata.pb.h \
    fljobwindow.h \
    flfloorview.h \
    fljob.h \
    flpropertytable.h \
    fltreewidget.h \
    flwio.h \
    flzonecontrollor.h \
    flfloor.h \
    flobject.h \
    fldeviceitem.h \
    flgraphicssvgitem.h \
    flheatmap.h \
    flobjectlineitem.h \
    gradientpalette.h \
    edge.h \
    heatmapper.h \
    global.h \
    fljobprotol.h \
    flcreatefloordlg.h \
    flcreatejobdlg.h \
    flcreatewiodlg.h \
    flcreatezcdlg.h \
    fllineitem.h \
    flglobal.h \
    fl_job.pb.h \
    fl_defs.pb.h \
    macexplore.h \
    filetablemodel.h \
    mytableview.h \
    filetablemodel.h

FORMS += \
        mainwindow.ui \
    fljobwindow.ui \
    flcreatefloordlg.ui \
    flcreatejobdlg.ui \
    flcreatewiodlg.ui \
    flcreatezcdlg.ui \
    macexplore.ui

RESOURCES += \
    protoRC.qrc


win32:CONFIG(release, debug|release): LIBS += -L/usr/local/lib/release/ -lprotobuf.13
else:win32:CONFIG(debug, debug|release): LIBS += -L/usr/local/lib/debug/ -lprotobuf.13
else:unix: LIBS += -L/usr/local/lib/ -lprotobuf.13

INCLUDEPATH += /usr/local/include
DEPENDPATH += /usr/local/include
