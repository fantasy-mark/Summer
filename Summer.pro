#-------------------------------------------------
#
# Project created by QtCreator 2018-07-20T10:25:37
#
#-------------------------------------------------

QT       += core gui serialport printsupport network webenginewidgets webengine

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Summer
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

INCLUDEPATH += \
    src/ui/ \
    src/module/ \
    src

SOURCES += \
    src/ui/summer.cpp \
    src/main.cpp \
    src/module/xserial.cpp \
    src/module/xdev.cpp \
    src/module/xreport.cpp \
    src/module/logsys.cpp \
    src/module/xview.cpp \
    src/module/xpro.cpp \
    src/module/xsec.cpp \
    src/module/ximage.cpp

HEADERS += \
    src/ui/summer.h \
    src/module/xserial.h \
    src/module/xdev.h \
    src/module/xreport.h \
    src/module/logsys.h \
    src/module/xview.h \
    src/module/xpro.h \
    src/module/xsec.h \
    src/kitconfig.h \
    src/module/ximage.h

FORMS += \
    src/ui/summer.ui \
    src/ui/irExam.ui \
    src/ui/assessReport.ui \
    src/ui/baseInfo.ui \
    src/ui/customerSearch.ui \
    src/ui/recuperatePlan.ui \
    src/ui/selfCheck.ui \
    src/ui/userManager.ui \
    src/ui/t3l0.ui \
    src/ui/t3l1.ui \
    src/ui/t3l2.ui \
    src/ui/t3l3.ui \
    src/ui/t3l4.ui \
    src/ui/t3l5.ui \
    src/ui/t3l6.ui \
    src/ui/t3l7.ui \
    src/ui/t3l8.ui \
    src/ui/t3l9.ui \
    src/ui/t3l10.ui \
    src/ui/t3l11.ui \
    src/ui/t3l12.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/ -lopencv_world340
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/ -lopencv_world340d

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

win32: LIBS += -L$$PWD/lib/ -lThermoGroupSDKLib

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include
