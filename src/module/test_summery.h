#pragma once

#include <QTest>

class Test_Summery: public QObject
{
	Q_OBJECT

private Q_SLOTS:
	// ÿ��private slot�������ǻᱻ�Զ����õĲ��Ժ���  
	void test_xserial();
	// ��"_data"��β�ĺ������Ӧ�Ĳ��Ժ����ṩ��������  
	void test_xserial_data();
};
