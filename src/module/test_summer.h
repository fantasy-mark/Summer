#pragma once

#include <QtTest>

class TestSummer : public QObject
{
    Q_OBJECT

public:
    TestSummer();

private Q_SLOTS:
    // ÿ��private slot�������ǻᱻ�Զ����õĲ��Ժ���
    void test_xserial();
    void test_xdev();
    // ��"_data"��β�ĺ������Ӧ�Ĳ��Ժ����ṩ��������
    void test_xserial_data();
};
