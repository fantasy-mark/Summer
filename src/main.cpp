#include "summer.h"
#include <QApplication>

#ifdef USING_GTEST
#include "gtest/gtest.h"
#include "test_summer.h"
#endif

#include <QFileDialog>
#include <QMessageBox>
#include <QString>
#include <QObject>

#include "xconfig.h"
#include "xdev.h"
#include <QDebug>

#ifdef WIN32
#include "windows.h"
#pragma comment(lib,"user32.lib")
#endif

void setup();
void set_configFile();

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

    RUN_ALL_TESTS();
#endif

    //qputenv("QTWEBENGINE_REMOTE_DEBUGGING", "9223");

    return a.exec();
}

void setup()
{
    set_configFile();
}

void set_configFile()
{
    QStringList fileNames;
    QString path = QDir::currentPath() + "/../Summer/resource/config.xml";
    if (! XConfig::Get()->load_XML(path)) {
        QFileDialog * fileDlg = new QFileDialog();
        fileDlg->setWindowFilePath(QDir::currentPath());
        fileDlg->setWindowTitle(QStringLiteral("打开配置文件"));
        fileDlg->setNameFilter(QStringLiteral("DOM(*.xml)"));
        //can select only one file
        fileDlg->setFileMode(QFileDialog::ExistingFile);
        if (fileDlg->exec()) {
            fileNames = fileDlg->selectedFiles();
            if (! XConfig::Get()->load_XML(fileNames[0]))
                QMessageBox::warning(NULL, QStringLiteral("配置"), QStringLiteral("配置文件有误"));
        }
    }
    //qDebug() << XConfig::Get()->globalConfig;
}
