#pragma once

#include <QTest>

class Test_Summery: public QObject
{
	Q_OBJECT

private Q_SLOTS:
	// 每个private slot函数都是会被自动调用的测试函数  
	void test_xserial();
	// 以"_data"结尾的函数向对应的测试函数提供测试数据  
	void test_xserial_data();
};
