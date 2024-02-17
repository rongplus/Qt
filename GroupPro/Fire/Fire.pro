#-------------------------------------------------
#
# Project created by QtCreator 2017-08-04T10:10:18
#
#-------------------------------------------------

QT       += core gui network serialport svg xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Fire
TEMPLATE = app

INCLUDEPATH += usr/local/include/google

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
    bargraph.cpp \
    edge.cpp \
    firelink.cpp \
    fl_defs.pb.cc \
    fl_job.pb.cc \
    flchooseoutputdialog.cpp \
    flcreatefloordlg.cpp \
    flcreatejobdlg.cpp \
    flcreatewiodlg.cpp \
    flcreatezcdlg.cpp \
    FLData.pb.cc \
    fldaylightdlg.cpp \
    fldeviceitem.cpp \
    fldlgchooseserialport.cpp \
    fldlgserialwrite.cpp \
    flfloor.cpp \
    flfloorpropertydlg.cpp \
    flfloorview.cpp \
    flglobal.cpp \
    flgraphicssvgitem.cpp \
    flheatmap.cpp \
    flinoutnode.cpp \
    fljob.cpp \
    fljobpropertydlg.cpp \
    fljobprotol.cpp \
    fljobwindow.cpp \
    fllineitem.cpp \
    flmessageview.cpp \
    flnetview.cpp \
    flobject.cpp \
    flobjectlineitem.cpp \   
    flowlayout.cpp \
    flprogressdialog.cpp \
    flpropertytable.cpp \
    flserialportio.cpp \
    flserialportwidget.cpp \
    flserialread.cpp \
    flserialwidget.cpp \
    flstartpage.cpp \
    fltreewidget.cpp \
    flundocommand.cpp \
    flusbdebug.cpp \
    flwidgetpcapview.cpp \
    flwidgetzigbeenetworkmonitor.cpp \
    flwidgetzigbeenetworkview.cpp \
    flwio.cpp \
    flwiopropertydlg.cpp \
    flxmlcommandmanager.cpp \
    flzcpropertydlg.cpp \
    flzcserialport.cpp \
    flzctimedlg.cpp \
    flzigbeenetworkmonitor.cpp \
    flzigbeeterminal.cpp \
    flzigbeethread.cpp \
    flzonecontrollor.cpp \
    gradientpalette.cpp \
    heatmapper.cpp \
    ledindicator.cpp \
    main.cpp \
    mainwindow.cpp \
    mgc_checkupdate.cpp \
    mgc_upgradefiledialog.cpp \
    PCAPReader.cpp \
    qx_dlg_about.cpp \
    qx_dlg_port.cpp \
    qx_link.cpp \
    qx_xmodem.cpp \
    SiteSurveyWindow.cpp \
    Spoiler.cpp \
    Switcher.cpp \
    wirelessmonitor.cpp \
    zc.pb.cc \
    zc.pb.c

HEADERS += \
        mainwindow.h \
    bargraph.h \
    edge.h \
    firelink.h \
    fl_defs.pb.h \
    fl_job.pb.h \
    flchooseoutputdialog.h \
    flcreatefloordlg.h \
    flcreatejobdlg.h \
    flcreatewiodlg.h \
    flcreatezcdlg.h \
    FLData.pb.h \
    fldaylightdlg.h \
    fldeviceitem.h \
    fldlgchooseserialport.h \
    fldlgserialwrite.h \
    flfloor.h \
    flfloorpropertydlg.h \
    flfloorview.h \
    flglobal.h \
    flgraphicssvgitem.h \
    flheatmap.h \
    flinoutnode.h \
    fljob.h \
    fljobpropertydlg.h \
    fljobprotol.h \
    fljobwindow.h \
    fllineitem.h \
    flmessageview.h \
    flnetview.h \
    flobject.h \
    flobjectlineitem.h \
    flowlayout.h \
    flprogressdialog.h \
    flpropertytable.h \
    flserialportio.h \
    flserialportwidget.h \
    flserialread.h \
    flserialwidget.h \
    flstartpage.h \
    fltreewidget.h \
    flundocommand.h \
    flusbdebug.h \
    flwidgetpcapview.h \
    flwidgetzigbeenetworkmonitor.h \
    flwidgetzigbeenetworkview.h \
    flwio.h \
    flwiopropertydlg.h \
    flxmlcommandmanager.h \
    flzcpropertydlg.h \
    flzcserialport.h \
    flzctimedlg.h \
    flzigbeenetworkmonitor.h \
    flzigbeeterminal.h \
    flzigbeethread.h \
    flzonecontrollor.h \
    global.h \
    gradientpalette.h \
    heatmapper.h \
    ledindicator.h \
    mainwindow.h \
    mgc_checkupdate.h \
    mgc_upgradefiledialog.h \
    pcap.h \
    PCAPReader.h \
    qx_dlg_about.h \
    qx_dlg_port.h \
    qx_link.h \
    qx_xmodem.h \
    resource.h \
    scudefs.h \
    SiteSurveyWindow.h \
    Spoiler.h \
    svudefs.h \
    Switcher.h \
    wirelessmonitor.h

FORMS += \
        mainwindow.ui \
    firelink.ui \
    flchooseoutputdialog.ui \
    flcreatefloordlg.ui \
    flcreatejobdlg.ui \
    flcreatewiodlg.ui \
    flcreatezcdlg.ui \
    fldaylightdlg.ui \
    fldlgchooseserialport.ui \
    fldlgserialwrite.ui \
    flfloorpropertydlg.ui \
    fljobpropertydlg.ui \
    fljobwindow.ui \
    flmessageview.ui \
    flnetview.ui \
    flprogressdialog.ui \
    flserialportwidget.ui \
    flserialwidget.ui \
    flserialwrite.ui \
    flstartpage.ui \
    flusbdebug.ui \
    flwidgetpcapview.ui \
    flwidgetzigbeenetworkmonitor.ui \
    flwidgetzigbeenetworkview.ui \
    flwiopropertydlg.ui \
    flwirelessmonitor.ui \
    flzcpropertydlg.ui \
    flzctimedlg.ui \
    flzigbeemonitor.ui \
    MainWindow.ui \
    networkview.ui \
    PageJobInfor.ui \
    qx_about.ui \
    qx_port.ui \
    qx_upgradefiledialog.ui \
    wirelessmonitor.ui

DISTFILES += \
    zc.pb \
    FireLink.rc

SUBDIRS += \
    Fire.pro

RESOURCES += \
    firelink.qrc
