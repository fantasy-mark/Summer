#include "test_summery.h"
#include "xserial.h"

/*****************************************************************************
 测试流程 :
 1. 插上串口
 2. 扫描bus获得port
 3. 根据port打开、关闭、读写验证
 *****************************************************************************/

void Test_Summery::test_xserial()
{
	int speed;
	QFETCH(QStringList, scan);
	QFETCH(bool, open);
	QVERIFY2(XSerial::Get()->open_serial(), "测试打开串口失败！需插上云台、摇杆配套串口测试,检查是否有软件已打开串口");
	//QVERIFY2_WITH_TIMEOUT会导致测试失败
	QVERIFY2(XSerial::Get()->get_poleSpeed(&speed), "摇杆控速失败");
	QVERIFY2(speed > -0x201 && speed < 0x201, "速度范围应在正负0x201以内");
	QVERIFY2(XSerial::Get()->set_cradleSpeed(&speed), "设置云台速度失败");
}

void Test_Summery::test_xserial_data()
{
	// 添加数据列
	QTest::addColumn<QStringList>("scan");
	QTest::addColumn<bool>("open");
	// 添加测试数据
	QTest::newRow("test_serial") << XSerial::Get()->scan_serial() << XSerial::Get()->open_serial();
}

// 用于构建可执行的测试程序  
QTEST_MAIN(Test_Summery)