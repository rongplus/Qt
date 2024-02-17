#-------------------------------------------------
#
# Project created by QtCreator 2017-08-04T09:58:24
#
#-------------------------------------------------

QT       += widgets network serialport

TARGET = SniffLib
TEMPLATE = lib

DEFINES += SNIFFLIB_LIBRARY

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
        snifflib.cpp \
    APSFrame.cpp \
    APSFrameInfo.cpp \
    MACFrame.cpp \
    MACFrameInfo.cpp \
    NWKFrame.cpp \
    NWKFrameInfo.cpp \
    SavedFrames.cpp \
    SavedPreference.cpp \
    ZBType.cpp \
    ZCLFrame.cpp \
    ZCLFrameInfo.cpp \
    ZTCFrame.cpp \
    ZTCFrameInfo.cpp \
    Helper.cpp

HEADERS += \
        snifflib.h \
        snifflib_global.h \ 
    APSFrame.h \
    APSFrameDef.h \
    APSFrameInfo.h \
    MACFrame.h \
    MACFrameDef.h \
    MACFrameInfo.h \
    NWKFrame.h \
    NWKFrameDef.h \
    NWKFrameInfo.h \
    SavedFrames.h \
    SavedPreference.h \
    ZBType.h \
    ZCLFrame.h \
    ZCLFrameDef.h \
    ZCLFrameInfo.h \
    ZTCFrame.h \
    ZTCFrameDef.h \
    ZTCFrameInfo.h \
    Helper.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
