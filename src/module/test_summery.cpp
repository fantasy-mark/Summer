#include "test_summery.h"
#include "xserial.h"

/*****************************************************************************
 �������� :
 1. ���ϴ���
 2. ɨ��bus���port
 3. ����port�򿪡��رա���д��֤
 *****************************************************************************/

void Test_Summery::test_xserial()
{
	int speed;
	QFETCH(QStringList, scan);
	QFETCH(bool, open);
	QVERIFY2(XSerial::Get()->open_serial(), "���Դ򿪴���ʧ�ܣ��������̨��ҡ�����״��ڲ���,����Ƿ�������Ѵ򿪴���");
	//QVERIFY2_WITH_TIMEOUT�ᵼ�²���ʧ��
	QVERIFY2(XSerial::Get()->get_poleSpeed(&speed), "ҡ�˿���ʧ��");
	QVERIFY2(speed > -0x201 && speed < 0x201, "�ٶȷ�ΧӦ������0x201����");
	QVERIFY2(XSerial::Get()->set_cradleSpeed(&speed), "������̨�ٶ�ʧ��");
}

void Test_Summery::test_xserial_data()
{
	// ���������
	QTest::addColumn<QStringList>("scan");
	QTest::addColumn<bool>("open");
	// ��Ӳ�������
	QTest::newRow("test_serial") << XSerial::Get()->scan_serial() << XSerial::Get()->open_serial();
}

// ���ڹ�����ִ�еĲ��Գ���  
QTEST_MAIN(Test_Summery)