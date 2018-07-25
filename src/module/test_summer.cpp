#include <QString>
#include "xserial.h"
#include "xdev.h"
#include "test_summer.h"

TestSummer::TestSummer()
{
}

void TestSummer::test_xserial()
{
    int speed;
    QFETCH(QStringList, scan);
    QFETCH(bool, open);
    QVERIFY2(open, "plz open crade/pole serial && close all soft if opening the serial");
    ////QVERIFY2_WITH_TIMEOUT会导致测试失败
    QVERIFY2(XSerial::Get()->get_poleSpeed(&speed), "get poleSpeed failed");
    QVERIFY2(speed > -0x201 && speed < 0x201, "-0x201 < speed < 0x201 verify failed");
    QVERIFY2(XSerial::Get()->set_cradleSpeed(&speed), "set cradle speed failed");
}

void TestSummer::test_xserial_data()
{
    // 添加数据列
    QTest::addColumn<QStringList>("scan");
    QTest::addColumn<bool>("open");
    // 添加测试数据
    QTest::newRow("test_serial") << XSerial::Get()->scan_serial() << XSerial::Get()->open_serial();
}

void TestSummer::test_xdev()
{
    QVERIFY2(XDev::Get()->create_irDev(), "create ir dev failure");
    QVERIFY2(XDev::Get()->refresh_irDev(), "refresh ir dev failure");
    QVERIFY2(XDev::Get()->connect_irDev(), "connect ir dev failure");
    //MAG_StartProcessImage持续播放
    //QVERIFY2(XDev::Get()->play_irDev(), "play bmpdata stream failure");
}

QTEST_APPLESS_MAIN(TestSummer)

#include "test_summer.moc"
