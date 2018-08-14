#pragma once

#include <QObject>
#include <QTreeWidget>
#include <QJsonObject>

/*****************************************************************************
    Copyright	: Yaqian Group
    Author		: Mark_Huang ( hacker.do@163.com )
    Date		: 2018.07.26
    Description	: 配置文件
 *****************************************************************************/
class XConfig : public QObject
{
    Q_OBJECT

public:
    QJsonObject xconfig;
    QTreeWidget * tree;

    static XConfig * Get();
    bool setup();

    QString value(const char * key);
    void configure();

    //TODO
    void help() {}
};
