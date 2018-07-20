#pragma once

#include <QtTest>

class TestSummer : public QObject
{
    Q_OBJECT

public:
    TestSummer();

private Q_SLOTS:
    // 每个private slot函数都是会被自动调用的测试函数
    void test_xserial();
    void test_xdev();
    // 以"_data"结尾的函数向对应的测试函数提供测试数据
    void test_xserial_data();
};
