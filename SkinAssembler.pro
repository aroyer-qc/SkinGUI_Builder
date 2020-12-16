#-------------------------------------------------
#
# Project created by QtCreator 2014-03-04T08:10:16
#
#-------------------------------------------------

QT       += core gui xml multimedia
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SkinAssembler
TEMPLATE = app


SOURCES += main.cpp\
    mainwindow.cpp \
    Converter.cpp \
    Utility.cpp \
    qxmlputget.cpp \
    file.cpp \
    option.cpp \
    widget.cpp \
    Image.cpp \
    setsize.cpp \
    progress.cpp \
    compression.cpp \
    SkinSave.cpp \
    SkinOpen.cpp \
    lzw.cpp \
    ComboBoxDelegate.cpp \
    SpinBoxDelegate.cpp \
    ImageSpinBox.cpp \
    setEndianess.cpp \
    Font.cpp \
    Simulator.cpp \
    AddingImage.cpp \
    AddingAudio.cpp

HEADERS  += mainwindow.h \
    Utility.h \
    qxmlputget.h \
    typedef.h \
    setsize.h \
    progress.h \
    compression.h \
    SkinSave.h \
    SkinOpen.h \
    ComboBoxDelegate.h \
    SpinBoxDelegate.h \
    ImageSpinBox.h \
    setEndianess.h \
    AddingImage.h \
    AddingAudio.h

FORMS    += mainwindow.ui \
    editdefine.ui \
    setsize.ui \
    progress.ui \
    setEndianess.ui \
    AddingAudio.ui \
    AddingImage.ui

RESOURCES += \
    SkinRessource.qrc

QMAKE_CXXFLAGS += -std=c++11
QMAKE_CFLAGS += -Wall -Wextra
QMAKE_CFLAGS += -Wno-comment
