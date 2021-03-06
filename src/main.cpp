﻿#include "summer.h"
#include <QApplication>
#include <QTextCodec>

#ifdef USING_GTEST
#include "gtest/gtest.h"
#include "test_summer.h"
#endif

#include <QFileDialog>
#include <QMessageBox>
#include <QString>
#include <QObject>
#include <QtConcurrent>

#include "xconfig.h"
#include "xdev.h"
#include <QDebug>

#ifdef WIN32
#include "windows.h"
#pragma comment(lib,"user32.lib")
#endif

//void setup();
//void set_configFile();
#ifdef USING_GTEST
void run_gTest()
{
    QTime time;
    Pr("5秒后开始单元测试...");

    time.start();
    //非阻塞执行
    while(time.elapsed() < 5000) {
        QCoreApplication::processEvents(QEventLoop::AllEvents);
    }

    RUN_ALL_TESTS();
}
#endif

int main(int argc, char *argv[])
{
#if 0
#ifdef WIN32
    //用于调整系统分辨率
    DEVMODE DevMode;
    EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &DevMode);
    DevMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
    DevMode.dmPelsWidth = 1440;
    DevMode.dmPelsHeight = 900;
    ChangeDisplaySettings(&DevMode, CDS_FULLSCREEN);
#endif
#endif

    QApplication a(argc, argv);

    //setup();
    Summer w;

    //设置在某个屏幕显示UI
    //QDesktopWidget * deskTop = QApplication::desktop();
    //MainWindow::Get()->setGeometry(deskTop->screenGeometry(1));
    w.setWindowFlags(Qt::FramelessWindowHint);
    w.show();

#ifdef USING_GTEST
    testing::InitGoogleTest(&argc, argv);

    QtConcurrent::run(run_gTest);
#endif

    //qputenv("QTWEBENGINE_REMOTE_DEBUGGING", "9223");

    return a.exec();
}
