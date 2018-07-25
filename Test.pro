#-------------------------------------------------
#
# Project created by QtCreator 2018-07-20T15:51:33
#
#-------------------------------------------------

#
# Add by Mark
# 用于Summer类单元测试
#

QT       += testlib serialport

QT       -= gui

TARGET = tst_testsummer
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    src/module/test_summer.cpp \
    src/module/xserial.cpp \
    src/module/xdev.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"

INCLUDEPATH += include

HEADERS += \
    src/module/xserial.h \
    src/module/test_summer.h \
    src/module/xdev.h \

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/ -lopencv_world340
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/ -lopencv_world340d

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

win32: LIBS += -L$$PWD/lib/ -lThermoGroupSDKLib

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include
