#include <qDebug>
#include "gtest/gtest.h"
#include "xserial.h"
#include "xdev.h"
#include "logsys.h"

namespace {
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
TEST(XSerial, getSpeed)
{
    EXPECT_TRUE(XSerial::Get()->get_poleSpeed(&speed)) <<  "some time can't get pole speed, maybe qserial module have same bug?\n";
}
TEST(XSerial, setSpeed)
{
    speed = 0x180;
    EXPECT_TRUE(XSerial::Get()->set_cradleSpeed(&speed));
}
/*****************************************************************************
    Date		: 2018.08.06
    Description	: 测试XDev类
 *****************************************************************************/
TEST(XDev, create)
{
    EXPECT_TRUE(XDev::Get()->create_irDev()) << "can't create irDev, confirm connect lan\n";
}
TEST(XDev, refresh)
{
    qDebug() << XDev::Get()->refresh_irDev();
}
TEST(XDev, connect)
{
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
#if 0
TEST(XDev, farFocus)
{
    XDev::Get()->far_focus();
}
TEST(XDev, nearFocus)
{
    XDev::Get()->near_focus();
}
#endif
TEST(XDev, stop)
{
    XDev::Get()->stop_irDev();
}
TEST(XDev, disconnect)
{
    XDev::Get()->disconnect_irDev();
}
TEST(XDev, free)
{
    XDev::Get()->free_irDev();
}
/*****************************************************************************
    Date		: 2018.08.06
    Description	: 测试LogSys模块
 *****************************************************************************/
TEST(LogSys, get_appPath)
{
    qDebug() << QString::fromStdString(LogSys::Get()->GetAppPathA());
}
TEST(LogSys, get_formatString)
{
    qDebug() << QString::fromStdString(LogSys::Get()->FormatString("%04d%02d%02d_%02d%02d.md", 2018, 8, 11, 17, 59));
}
}

/*****************************************************************************
    Copyright	: Yaqian Group
    Author		: Mark_Huang ( hacker.do@163.com )
    Date		: 2018.08.06
    Description	: 单元测试总入口
 *****************************************************************************/
int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();
}
