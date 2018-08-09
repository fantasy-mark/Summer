#include "gtest/gtest.h"
#include <qDebug>
#include "xserial.h"
#include "xdev.h"
#include "logsys.h"
#include "xpro.h"
#include "xview.h"

#include <QObject>
#include <QString>
#include <QTime>

#include "opencv2/opencv.hpp"
using namespace cv;

namespace {
QWidget * w;
/*****************************************************************************
    Date		: 2018.08.07
    Description	: 日志打印
 *****************************************************************************/
TEST(LogSys, pr)
{
    LogSys::Get()->ChangeLogLevel(LogLevel_Info);
    PrFatal("a fatal log");
    PrError("a error log");
    PrWarning("a warning log");
    Pr();
    QString strMessage = QStringLiteral("我是中文编码,我是QString类型\n");
    LogSys::Get()->Info(strMessage);
#if 0
    qDebug()<< typeid(strMessage).name();
    qDebug() << typeid(a).name();
#endif
}
TEST(LogSys, get_appPath)
{
    qDebug() << QString::fromStdString(LogSys::Get()->GetAppPathA());
}
TEST(LogSys, get_formatString)
{
    qDebug() << QString::fromStdString(LogSys::Get()->FormatString("%04d%02d%02d_%02d%02d.md", 2018, 8, 11, 17, 59));
}
/*****************************************************************************
    Date		: 2018.08.06
    Description	: 测试XSerial类
 *****************************************************************************/
int speed = 0;
TEST(XSerial, scan)
{
    qDebug() << XSerial::Get()->scan_serial();
}
TEST(XSerial, close)
{
    XSerial::Get()->close_serial();
}
TEST(XSerial, open)
{
    EXPECT_TRUE(XSerial::Get()->open_serial());
}
TEST(XSerial, get_poleSpeed)
{
    EXPECT_TRUE(XSerial::Get()->get_poleSpeed(&speed)) <<  "some time can't get pole speed, maybe qserial module have same bug?\n";
}
TEST(XSerial, set_cradleSpeed)
{
    //设置定时器和信号
    QTime time;

    int speed = 0x180;
    EXPECT_TRUE(XSerial::Get()->set_cradleSpeed(&speed));

    time.start();
    //非阻塞执行
    while(time.elapsed() < 2000) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }

    speed = -0x180;
    EXPECT_TRUE(XSerial::Get()->set_cradleSpeed(&speed));
    //非阻塞执行
    while(time.elapsed() < 4000) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }

    speed = 0x0;
    EXPECT_TRUE(XSerial::Get()->set_cradleSpeed(&speed));
}
/*****************************************************************************
    Date		: 2018.08.06
    Description	: 测试XDev类
 *****************************************************************************/
TEST(XDev, init)
{
    EXPECT_TRUE(XDev::Get()->create_irDev()) << "can't create irDev, confirm connect lan\n";
}
#if 1
TEST(XDev, connect)
{
    qDebug() << XDev::Get()->refresh_irDev();
    EXPECT_TRUE(XDev::Get()->connect_irDev());
}
TEST(XDev, play)
{
    EXPECT_TRUE(XDev::Get()->play_irDev());
}
TEST(XDev, autoFocus)
{
    XDev::Get()->auto_focus();
}
TEST(XDev, farFocus)
{
    XDev::Get()->far_focus();
}
TEST(XDev, nearFocus)
{
    XDev::Get()->near_focus();
}
#endif
/*****************************************************************************
    Date		: 2018.08.06
    Description	: 测试LogSys模块
 *****************************************************************************/
TEST(XPro, rainbow)
{
    XPro::Get()->rainbowColorMap();
    //imshow("rainbom", XPro::Get()->get_colormap());
}
TEST(XView, set_colorMap)
{
    UCHAR const * pBarData;
    BITMAPINFO const * pBarInfo;
    QTime time;

    //!!! 此处测试得播放后需要延时才能获取正确的色卡
    time.start();
    //非阻塞执行
    while(time.elapsed() < 200) {
        QCoreApplication::processEvents(QEventLoop::AllEvents);
    }
    EXPECT_TRUE(XDev::Get()->GetOutputColorBardata(&pBarData, &pBarInfo));
}
#if 0
TEST(ALL, exit)
{
    XDev::Get()->stop_irDev();
    XDev::Get()->disconnect_irDev();
    XDev::Get()->free_irDev();
}
#endif
}
