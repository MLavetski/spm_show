#-------------------------------------------------
#
# Project created by QtCreator 2017-07-17T15:57:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets datavisualization

TARGET = spm_show
TEMPLATE = app


SOURCES += main.cpp\
        sshow.cpp \
    sshowsub.cpp \
    showprofile.cpp \
    imlabel.cpp \
    surface.cpp

HEADERS  += sshow.h \
    sshowsub.h \
    showprofile.h \
    imlabel.h \
    surface.h \
    triangles.h

FORMS    += sshow.ui \
    sshowsub.ui \
    showprofile.ui \
    surface.ui


QMAKE_LFLAGS_WINDOWS = /SUBSYSTEM:WINDOWS,5.1

RC_ICONS += "icons\\qr code.ico"

RESOURCES += \
    resources.qrc

DISTFILES +=
