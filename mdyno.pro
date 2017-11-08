#-------------------------------------------------
#
# Project created by QtCreator 2017-10-07T01:56:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mdyno
TEMPLATE = app

FORMS    += modelingdynamicobject.ui

HEADERS += \
    common.h \
    dynamicdron.h \
    dynamicobjmngr.h \
    dynamicship.h \
    idynamicobject.h \
    modelingdynamicobject.h \
    varea.h \
    vpaintline.h \
    vpaintmultiline.h \
    vpaintpoint.h \
    vpaintpolygon.h \
    vroute.h \
    vroutedron.h \
    vrouteship.h

SOURCES += \
    dynamicdron.cpp \
    dynamicobjmngr.cpp \
    dynamicship.cpp \
    idynamicobject.cpp \
    main.cpp \
    modelingdynamicobject.cpp \
    varea.cpp \
    vpaintline.cpp \
    vpaintmultiline.cpp \
    vpaintpoint.cpp \
    vpaintpolygon.cpp \
    vroute.cpp \
    vroutedron.cpp \
    vrouteship.cpp
